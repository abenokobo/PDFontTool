#include "KerningPairLoader.h"
#include "PlaydateFntFileWriter.h"
#include "FontLoader.h"
#include <assert.h>
#include <vector>
#include <iostream>



///
HFONT KerningPairLoader::CreateGDIFont
(
)
{
	CComPtr<IDWriteGdiInterop> gdiInterop;
	HRESULT hr = m_loader->m_factory->GetGdiInterop(&gdiInterop);
	if (FAILED(hr) || (gdiInterop == NULL))
	{
		return NULL;
	}

	LOGFONT lf = {};
	hr = gdiInterop->ConvertFontFaceToLOGFONT(m_loader->m_fontFace, &lf);
	if (FAILED(hr))
	{
		return NULL;
	}

	return ::CreateFontIndirect(&lf);
}


///
HDC KerningPairLoader::CreateFontDC
(
)
{
	HDC hDesktop = ::GetDC(NULL);
	if (hDesktop == NULL)
	{
		return NULL;
	}

	HDC hFontDC = ::CreateCompatibleDC(hDesktop);
	if (hFontDC != NULL)
	{
		::ReleaseDC(NULL, hDesktop);
	}

	return hFontDC;
}


///
bool KerningPairLoader::LoadKerningPairs
(
	HDC hDC
)
{
	TEXTMETRIC metrics;
	if (!::GetTextMetrics(hDC, &metrics))
	{
		assert(false);
		return false;
	}
	float per = (m_loader->m_fontSize / (metrics.tmHeight * 1.0f));

	std::vector<KERNINGPAIR> keringPairs;
	DWORD cnt = ::GetKerningPairs(hDC, INT_MAX, NULL);
	if (cnt == 0)
	{
		return true;
	}

	keringPairs.resize(cnt);

	if (::GetKerningPairs(hDC, cnt, &keringPairs[0]) != cnt)
	{
		assert(false);
		return false;
	}

	for (auto ite = keringPairs.begin(); ite != keringPairs.end(); ite++)
	{
		if (ite->iKernAmount == 0)
		{
			continue;
		}

		int amount = 0;
		if (ite->iKernAmount > 0)
		{
			amount = static_cast<int>(floor(ite->iKernAmount * per));
		}
		else
		{
			amount = static_cast<int>(ceilf(ite->iKernAmount * per));
		}

		if (amount != 0)
		{
			m_writer->AppendKerningPair(ite->wFirst, ite->wSecond, amount);
		}
	}

	return true;
}


///
KerningPairLoader::KerningPairLoader
(
	std::shared_ptr<FontLoader>& loader,
	std::shared_ptr<PlaydateFntFileWriter>& writer
)
: m_loader(loader),
  m_writer(writer)
{
	assert(loader);
	assert(writer);
}


///
KerningPairLoader::~KerningPairLoader
(
)
{
}


///
bool KerningPairLoader::LoadKerningPairs
(
)
{
	if (!::AddFontResourceEx(m_loader->m_fontPath, FR_PRIVATE, NULL))
	{
		return false;
	}

	HFONT hFont = NULL;
	HDC hDC = NULL;
	HGDIOBJ hOld = NULL;
	bool ret = false;

	do {
		hFont = CreateGDIFont();
		if (hFont == NULL)
		{
			break;
		}

		///
		hDC = CreateFontDC();
		if (hDC == NULL)
		{
			break;
		}

		hOld = ::SelectObject(hDC, static_cast<HGDIOBJ>(hFont));
		if (hOld == NULL)
		{
			break;
		}

		ret = LoadKerningPairs(hDC);

	} while (false);


	if (hOld != NULL)
	{
		::SelectObject(hDC, hOld);
	}

	if (hDC != NULL)
	{
		::DeleteDC(hDC);
	}

	if (hFont != NULL)
	{
		::DeleteObject(hFont);
	}

	return ret;
}



