#include "PDFontTool.h"
#include "FontLoader.h"
#include "FontRenderer.h"
#include <iostream>
#include <gdiplus.h>
#include <assert.h>



#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment( lib, "WindowsCodecs.lib" )
#pragma comment( lib, "gdiplus.lib" )




///
PDFontTool::PDFontTool
(
	int nFontSize,
	const wchar_t* szFontPath,
	const wchar_t* szDstPath,
	const wchar_t* szDstFileName
)
: m_option(nFontSize, szFontPath, szDstPath, szDstFileName)
, m_gt(NULL)
{
	m_loader = std::make_shared<FontLoader>();
	m_renderer = std::make_shared<FontRenderer>();

	Gdiplus::GdiplusStartupInput si = {0};
	if (Gdiplus::GdiplusStartup(&m_gt, &si, 0) != Gdiplus::Ok)
	{
		assert(false);
	}
}


///
PDFontTool::~PDFontTool
(
)
{
	Gdiplus::GdiplusShutdown(m_gt);
}


///
bool PDFontTool::CreatePDFont
(
)
{
	if (!m_loader || !m_renderer)
	{
		return false;
	}

	CComPtr<IDWriteFactory3> DWriteFactory;
	HRESULT hr = ::DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory3),
		(IUnknown**)&DWriteFactory);
	if (FAILED(hr) || !DWriteFactory)
	{
		std::wcout << L"Failed to create DirectWrite objects." << std::endl << L"Windows 10 or later is required." << std::endl;
		return false;
	}


	if (!m_loader->LoadFont(DWriteFactory, m_option.fontPath,  m_option.fontSize))
	{
		return false;
	}

	if (!m_renderer->CreateResources(DWriteFactory, m_loader, &m_option))
	{
		return false;
	}

	if (!m_renderer->RenderFont())
	{
		return false;
	}

	return true;
}




