#pragma once
#include <dwrite_3.h>
#include <atlstr.h>
#include <atlcomcli.h>
#include <vector>



class FontLoader
{
private:


	///
	bool PreCreateFontResources(CComPtr<IDWriteFactory3>& factory, const wchar_t* fontPath);

	///
	bool CreateFontResources(CComPtr<IDWriteFactory3>& factory, const wchar_t* fontPath, int fontSize);



private:


	///
	CComPtr<IDWriteFontCollection> m_fontCollection;



public:


	///
	CComPtr<IDWriteTextFormat> m_format;

	///
	CComPtr<IDWriteFontFace3> m_fontFace;

	///
	std::vector<DWRITE_UNICODE_RANGE> m_unicodeRanges;



public:


	///
	FontLoader();

	///
	~FontLoader();

	///
	bool LoadFont(CComPtr<IDWriteFactory3>& factory, const wchar_t* fontPath, int fontSize);
};

