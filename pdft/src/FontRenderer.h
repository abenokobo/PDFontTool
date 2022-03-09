#pragma once
#include <dwrite_3.h>
#include <d2d1.h>
#include <wincodec.h>
#include <vector>



class FontLoader;
class PlaydateFntFileWriter;
class PDFontToolOption;



class FontRenderer
{
public:


	///
	std::shared_ptr<FontLoader> m_loader;

	///
	std::shared_ptr<PlaydateFntFileWriter> m_writer;

	///
	PDFontToolOption* m_option;

	///
	std::vector<UINT32> m_vecUnicodeChars;

	///
	CSize m_sizeBitmap;

	///
	CComPtr<IDWriteFactory3> m_DWriteFactory;

	///
	CComPtr<ID2D1Factory> m_D2D1Factory;

	///
	CComPtr<IWICImagingFactory> m_WICIFactory;

	///
	CComPtr<IWICBitmap> m_bitmap;

	///
	CComPtr<ID2D1RenderTarget> m_renderTarget;

	///
	CComPtr<ID2D1SolidColorBrush> m_brush;

#ifdef _DEBUG
#ifdef DRAW_DEBUG_BOX
	///
	CComPtr<ID2D1SolidColorBrush> m_brushDebug[3];
#endif
#endif

	///
	bool DrawUnicodeChars();

	///
	bool WritePNGFile();



public:


	///
	FontRenderer();

	///
	~FontRenderer();

	/// 
	bool CreateResources(CComPtr<IDWriteFactory3>& DWriteFactory, std::shared_ptr<FontLoader>& loader, PDFontToolOption* option);

	///
	bool RenderFont();
};


