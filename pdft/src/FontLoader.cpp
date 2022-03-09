#include <assert.h>
#include "FontLoader.h"



///
bool FontLoader::PreCreateFontResources
(
	CComPtr<IDWriteFactory3>& factory,
	const wchar_t* fontPath
)
{
	CComPtr<IDWriteFontSetBuilder> builder;
	HRESULT hr = factory->CreateFontSetBuilder(&builder);
	if (FAILED(hr) || !builder)
	{
		return false;
	}

	CComPtr<IDWriteFontFile> file;
	hr = factory->CreateFontFileReference(fontPath, NULL, &file);
	if (FAILED(hr) || !file)
	{
		return false;
	}

#ifdef _DEBUG
	// check multiple fontset
	BOOL isSupportedFontType;
	DWRITE_FONT_FILE_TYPE fileType;
	DWRITE_FONT_FACE_TYPE faceType;
	UINT numberOfFonts;
	hr = file->Analyze(&isSupportedFontType, &fileType, &faceType, &numberOfFonts);
	if (FAILED(hr))
	{
		return false;
	}
	assert(numberOfFonts == 1);
#endif

	CComPtr<IDWriteFontFaceReference> faceRef;
	hr = factory->CreateFontFaceReference(file, 0, DWRITE_FONT_SIMULATIONS_NONE, &faceRef);
	if (FAILED(hr) || !faceRef)
	{
		return false;
	}

	hr = builder->AddFontFaceReference(faceRef);
	if (FAILED(hr))
	{
		return false;
	}

	hr = faceRef->CreateFontFace(&m_fontFace);
	if (FAILED(hr) || !m_fontFace)
	{
		return false;
	}

	CComPtr<IDWriteFontSet> fontSet;
	builder->CreateFontSet(&fontSet);
	if (FAILED(hr) || !fontSet)
	{
		return false;
	}

	CComPtr<IDWriteFontCollection1> fontCol;
	hr = factory->CreateFontCollectionFromFontSet(fontSet, &fontCol);
	if (FAILED(hr) || !fontCol)
	{
		return false;
	}
	assert(fontCol->GetFontFamilyCount() == 1);

	CComPtr<IDWriteFontCollection> fontColIF;
	hr = fontCol->QueryInterface(&fontColIF);
	if (FAILED(hr) || !fontColIF)
	{
		return false;
	}
	m_fontCollection = fontColIF;

	return true;
}


///
bool FontLoader::CreateFontResources
(
	CComPtr<IDWriteFactory3>& factory,
	const wchar_t* fontPath
	, int fontSize
)
{
	CComPtr<IDWriteFontFamily> family;
	m_fontCollection->GetFontFamily(0, &family);
	CComPtr<IDWriteLocalizedStrings> strFamily;
	HRESULT hr = family->GetFamilyNames(&strFamily);
	if (FAILED(hr) || !strFamily)
	{
		return false;
	}

//	assert(strFamily->GetCount() == 1);
	WCHAR szFamilyName[1024];
	hr = strFamily->GetString(0, szFamilyName, 1024);
	if (FAILED(hr))
	{
		return false;
	}

	hr = factory->CreateTextFormat(
		szFamilyName,
		m_fontCollection,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize * 1.0f,
		L"",
		&m_format);
	if (FAILED(hr) || !m_format)
	{
		return false;
	}
	m_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	WCHAR szBuf[255];
	m_format->GetFontFamilyName(szBuf, 255);

	UINT32 actualRangeCount;
	m_fontFace->GetUnicodeRanges(0, NULL, &actualRangeCount);

	m_unicodeRanges.resize(actualRangeCount);
	m_fontFace->GetUnicodeRanges(actualRangeCount, &(m_unicodeRanges[0]), &actualRangeCount);

	if (m_fontFace->HasKerningPairs())
	{
		// TODO: 
		// Kerning Pairs
		// https://docs.microsoft.com/en-us/windows/win32/api/dwrite_1/nf-dwrite_1-idwritefontface1-getkerningpairadjustments
	}

	return true;
}


///
FontLoader::FontLoader
(
)
{
}


///
FontLoader::~FontLoader
(
)
{
}


///
bool FontLoader::LoadFont
(
	CComPtr<IDWriteFactory3>& factory,
	const wchar_t* fontPath,
	int fontSize
)
{
	if (!PreCreateFontResources(factory, fontPath))
	{
		return false;
	}

	if (!CreateFontResources(factory, fontPath, fontSize))
	{
		return false;
	}

	return true;
}


