#include "WICPNGWriter.h"
#include <iostream>



///
bool WICPNGWriter::WritePNGFile
(
	const wchar_t* path,
	const CComPtr<IWICImagingFactory>& WICIFactory,
	const CComPtr<IWICBitmap>& bitmap
)
{
	CComPtr<IWICStream> stream;
	HRESULT hr = WICIFactory->CreateStream(&stream);
	if (FAILED(hr) || !stream)
	{
		return false;
	}

	hr = stream->InitializeFromFilename(path, GENERIC_WRITE);
	if (FAILED(hr))
	{
		return false;
	}

	CComPtr<IWICBitmapEncoder> encoder;
	hr = WICIFactory->CreateEncoder(
		  GUID_ContainerFormatPng
		, NULL	// No preferred codec vendor.
		, &encoder);
	if (FAILED(hr) || !encoder)
	{
		return false;
	}

	hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
	if (FAILED(hr))
	{
		return false;
	}

	CComPtr<IWICBitmapFrameEncode> frame;
	hr = encoder->CreateNewFrame(
		  &frame
		, NULL	// No encoder options.
	);
	if (FAILED(hr) || !frame)
	{
		return false;
	}

	hr = frame->Initialize(NULL);
	if (FAILED(hr))
	{
		return false;
	}

	UINT w, h;
	bitmap->GetSize(&w, &h);
	frame->SetSize(w, h);

	WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
	hr = frame->SetPixelFormat(&format);

	hr = frame->WriteSource(bitmap, NULL);
	if (FAILED(hr))
	{
		if (hr == WINCODEC_ERR_IMAGESIZEOUTOFRANGE)
		{
			std::wcout << L"The image size is outside the valid range." << std::endl;
		}

		return false;
	}

	hr = frame->Commit();
	if (FAILED(hr))
	{
		return false;
	}

	hr = encoder->Commit();
	if (FAILED(hr))
	{
		return false;
	}

	hr = stream->Commit(STGC_DEFAULT);
	if (FAILED(hr))
	{
		return false;
	}

	frame = NULL;
	encoder = NULL;
	stream = NULL;

	return true;
}


