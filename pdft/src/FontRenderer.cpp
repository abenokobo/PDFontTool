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

	CComPtr<IDWriteTextLayout> layout;
	HRESULT hr = m_DWriteFactory->CreateTextLayout(
		L"", 1, m_loader->m_format, 0, 0,
		&layout);
	if (FAILED(hr) || !layout)
	{
		return false;
	}

	if (m_option->fontBoxSize.cx == 0 && m_option->fontBoxSize.cy == 0)
	{
		DWRITE_TEXT_METRICS metrix;
		layout->GetMetrics(&metrix);
		m_option->fontBoxSize.cx = static_cast<int>(ceil(metrix.height));
		m_option->fontBoxSize.cy = m_option->fontBoxSize.cx;
	}

	for (auto ite = m_loader->m_unicodeRanges.begin(); ite != m_loader->m_unicodeRanges.end(); ite++)
	{
		for (UINT32 code = ite->first; code <= ite->last; code++)
		{
			m_vecUnicodeChars.push_back(code);
		}
	}

	m_sizeBitmap.cx = m_option->fontBoxSize.cx * 16;
	m_sizeBitmap.cy = static_cast<LONG>(
		((m_vecUnicodeChars.size() / 16) * m_option->fontBoxSize.cy) +
		((m_vecUnicodeChars.size() % 16) ? m_option->fontBoxSize.cy : 0));

	hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_D2D1Factory);
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
		D2D1::ColorF(1.f, 0.f, 0.f, 0.25f), &m_brushDebug[0]);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.f, 1.f, 0.f, 0.25f), &m_brushDebug[1]);
	m_renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0.f, 0.f, 1.f, 0.25f), &m_brushDebug[2]);
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
bool FontRenderer::DrawUnicodeChars
(
)
{
	m_renderTarget->BeginDraw();

	static const D2D1_COLOR_F transparent = { 1.f, 1.f, 1.f, 0.f };
	m_renderTarget->Clear(&transparent);

	float left = 0;
	float top = 0;
	for (auto ite = m_vecUnicodeChars.begin(); ite != m_vecUnicodeChars.end(); ite++)
	{
		D2D1_RECT_F rcText = {
			  left, top
			, left + m_option->fontBoxSize.cx, top + m_option->fontBoxSize.cy };

		CString utf16Char;
		PlaydateFntFileWriter::UTF32CharToUtf16Char(*ite, utf16Char);

		CComPtr<IDWriteTextLayout> layout;
		HRESULT hr = m_DWriteFactory->CreateTextLayout(
			utf16Char.GetBuffer(), utf16Char.GetLength(),
			m_loader->m_format,
			static_cast<float>(m_option->fontBoxSize.cx),
			static_cast<float>(m_option->fontBoxSize.cy),
			&layout);
		if (FAILED(hr) || !layout)
		{
			return false;
		}

		DWRITE_TEXT_METRICS textMetrics = {0};
		layout->GetMetrics(&textMetrics);
		if (!m_writer->AppendCharWidth(*ite, static_cast<unsigned int>(ceil(textMetrics.widthIncludingTrailingWhitespace))))
		{
			return false;
		}

		D2D1_POINT_2F pt = { left, top };
		m_renderTarget->DrawTextLayout(pt, layout, m_brush);

	#ifdef _DEBUG
	#ifdef DRAW_DEBUG_BOX
		D2D1_RECT_F rcDebug = {
			  left, top
			, left + m_option->fontBoxSize.cx, top + m_option->fontBoxSize.cy };
		m_renderTarget->FillRectangle(
			rcDebug,
			m_brushDebug[
				std::distance(m_vecUnicodeChars.begin(), ite) % 3]);
	#endif
	#endif

		left += m_option->fontBoxSize.cx;
		if (left >= m_option->fontBoxSize.cx * 16)
		{
			left = 0;
			top += m_option->fontBoxSize.cy;
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
		m_option->fontBoxSize.cx,
		m_option->fontBoxSize.cy);

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

