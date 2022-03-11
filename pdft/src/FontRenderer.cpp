#include <iostream>
#include "PDFontTool.h"
#include "FontRenderer.h"
#include "FontLoader.h"
#include "WICPNGWriter.h"
#include "GdipPNGWriter.h"
#include "PlaydateFntFileWriter.h"



///
FontRenderer::FontRenderer
(
)
{
	m_writer = std::make_shared<PlaydateFntFileWriter>();
}


///
FontRenderer::~FontRenderer
(
)
{
}


bool FontRenderer::CreateResources
(
	CComPtr<IDWriteFactory3>& DWriteFactory,
	std::shared_ptr<FontLoader>& loader,
	PDFontToolOption* option
)
{
	m_DWriteFactory = DWriteFactory;
	m_loader = loader;
	m_option = option;

	for (auto ite = m_loader->m_unicodeRanges.begin(); ite != m_loader->m_unicodeRanges.end(); ite++)
	{
		for (UINT32 code = ite->first; code <= ite->last; code++)
		{
			m_vecUnicodeChars.push_back(code);
		}
	}

	if (!CalcRenderSize())
	{
		std::wcout << L"Font size calculation failed." << std::endl;
		return false;
	}

	if (m_option->gridSize.cx == 0 && m_option->gridSize.cy == 0)
	{
		m_option->gridSize = m_sizeRender;
	}

	m_sizeBitmap.cx = m_option->gridSize.cx * GRID_COLUMN_COUNT;
	m_sizeBitmap.cy = static_cast<LONG>(
		((m_vecUnicodeChars.size() / GRID_COLUMN_COUNT) * m_option->gridSize.cy) +
		((m_vecUnicodeChars.size() % GRID_COLUMN_COUNT) ? m_option->gridSize.cy : 0));

	HRESULT hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2D1Factory);
	if (FAILED(hr) || !m_D2D1Factory)
	{
		return false;
	}

	hr = ::CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&m_WICIFactory));
	if (FAILED(hr) || !m_WICIFactory)
	{
		return false;
	}

	hr = m_WICIFactory->CreateBitmap(
		m_sizeBitmap.cx,
		m_sizeBitmap.cy,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapCacheOnDemand,
		&m_bitmap
	);
	if (FAILED(hr) || !m_bitmap)
	{
		return false;
	}

	D2D1_RENDER_TARGET_PROPERTIES prop = D2D1::RenderTargetProperties();
	hr = m_D2D1Factory->CreateWicBitmapRenderTarget(
		m_bitmap,
		&prop,
		&m_renderTarget);
	if (FAILED(hr) || !m_renderTarget)
	{
		return false;
	}

	hr = m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.f, 0.f, 0.f, 1.f), &m_brush);
	if (FAILED(hr) || !m_brush)
	{
		return false;
	}

#ifdef _DEBUG
#ifdef DRAW_DEBUG_BOX
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(1.f, 0.f, 0.f, 0.25f), &m_brushBackground[0]);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.f, 1.f, 0.f, 0.25f), &m_brushBackground[1]);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.f, 0.f, 1.f, 0.25f), &m_brushBackground[2]);
#endif
#endif

#ifdef ENABLE_ANTIALIASING
	if (option->useAntialiasing)
	{
		m_renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
	}
	else
	{
		m_renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
	}
#else
	m_renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
#endif

	if (m_writer == NULL)
	{
		return false;
	}

	wchar_t szPath[MAX_PATH];
	::lstrcpy(szPath, m_option->dstPath.GetBuffer());
	::PathAppend(szPath, m_option->dstFileName.GetBuffer());
	::PathAddExtension(szPath, L".fnt");

	if (!m_writer->OpenFntFile(szPath))
	{
		return false;
	}

	return true;
}


///
bool FontRenderer::CalcCharacterSize
(
	CString utf16Char,
	CSize& size
)
{
	CComPtr<IDWriteTextLayout> layout;
	HRESULT hr = m_DWriteFactory->CreateTextLayout(
		utf16Char.GetBuffer(), utf16Char.GetLength(),
		m_loader->m_format, 0, 0, &layout);
	if (FAILED(hr) || !layout)
	{
		return false;
	}

	DWRITE_TEXT_METRICS textMetrics = { 0 };
	hr = layout->GetMetrics(&textMetrics);
	if (FAILED(hr))
	{
		return false;
	}

	size.cx = static_cast<LONG>(ceil(textMetrics.widthIncludingTrailingWhitespace));
	size.cy = static_cast<LONG>(ceil(textMetrics.height));

	return true;
}


///
bool FontRenderer::CalcRenderSize
(
)
{
	CString utf16Char;
	CSize size;

	// Height is calculated with empty characters
	if (!CalcCharacterSize(utf16Char, size))
	{
		return false;
	}
	m_sizeRender.cy = size.cy;

	// Width is the largest of all letters
	LONG width = 0;
	for (auto ite = m_vecUnicodeChars.begin(); ite != m_vecUnicodeChars.end(); ite++)
	{
		PlaydateFntFileWriter::UTF32CharToUtf16Char(*ite, utf16Char);
		if (!CalcCharacterSize(utf16Char, size))
		{
			return false;
		}

		if (!m_writer->AppendCharWidth(*ite, size.cx))
		{
			return false;
		}

		if (m_sizeRender.cx < size.cx)
		{
			m_sizeRender.cx = size.cx;
		}
	}

	return true;
}


///
bool FontRenderer::DrawUnicodeChars
(
)
{
	m_renderTarget->BeginDraw();

	static const D2D1_COLOR_F transparent = { 1.f, 1.f, 1.f, 0.f };
	m_renderTarget->Clear(&transparent);

	int rows = static_cast<int>(ceil(m_vecUnicodeChars.size() / (GRID_COLUMN_COUNT * 1.0f)));
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < GRID_COLUMN_COUNT; x++)
		{
			CString utf16Char;
			unsigned int index = x + y * GRID_COLUMN_COUNT;
			if (index < m_vecUnicodeChars.size())
			{
				PlaydateFntFileWriter::UTF32CharToUtf16Char(m_vecUnicodeChars[index], utf16Char);
			}
			else
			{
				utf16Char = L"";
			}

			CComPtr<IDWriteTextLayout> layout;
			HRESULT hr = m_DWriteFactory->CreateTextLayout(
				utf16Char.GetBuffer(), utf16Char.GetLength(),
				m_loader->m_format,
				static_cast<float>(m_sizeRender.cx),
				static_cast<float>(m_sizeRender.cy),
				&layout);
			if (FAILED(hr) || !layout)
			{
				return false;
			}

			D2D1_POINT_2F ptText = {
				static_cast<float>(x * m_option->gridSize.cx),
				static_cast<float>(y * m_option->gridSize.cy),
			};

			D2D1_RECT_F rcText =
				D2D1::RectF(
					ptText.x, ptText.y,
					ptText.x + m_option->gridSize.cx, ptText.y + m_option->gridSize.cy);

			m_renderTarget->PushAxisAlignedClip(rcText, D2D1_ANTIALIAS_MODE_ALIASED);

			// DrawTextLayout() sometimes ignored the clipping area, so we made sure to clear it.
			m_renderTarget->Clear(D2D1::ColorF(0, 0));

			m_renderTarget->DrawTextLayout(ptText, layout, m_brush);
			m_renderTarget->PopAxisAlignedClip();

#ifdef _DEBUG
#ifdef DRAW_DEBUG_BOX
			m_renderTarget->FillRectangle(
				rcText,
				m_brushBackground[(x + (y *GRID_COLUMN_COUNT)) % 3]);
#endif
#endif
		}
	}

	HRESULT hr = m_renderTarget->EndDraw();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


///
bool FontRenderer::WritePNGFile()
{
	CString strImageFileName;
	strImageFileName.Format(
		L"%s-table-%d-%d.png",
		m_option->dstFileName.GetBuffer(),
		m_option->gridSize.cx,
		m_option->gridSize.cy);

	wchar_t szPath[MAX_PATH];
	::lstrcpy(szPath, m_option->dstPath.GetBuffer());
	::PathAppend(szPath, strImageFileName.GetBuffer());

	if (!WICPNGWriter::WritePNGFile(szPath, m_WICIFactory, m_bitmap))
	{
		return false;
	}

	if (!GdipPNGWriter::RecreatePNGFileAccordingToOption(szPath, m_option, m_vecUnicodeChars, m_writer))
	{
		return false;
	}

	m_writer->CloseFntFile();
	m_writer = NULL;

	return true;
}


///
bool FontRenderer::RenderFont
(
)
{
	if (!DrawUnicodeChars())
	{
		return false;
	}

	if (!WritePNGFile())
	{
		return false;
	}

	return true;
}

