#pragma once
#include <atlcomcli.h>
#include <wincodec.h>


class WICPNGWriter
{
private:




public:


	///
	static bool WritePNGFile(const wchar_t* path, const CComPtr<IWICImagingFactory>& WICIFactory, const CComPtr<IWICBitmap>& bitmap);
};



