#include <assert.h>
#include "PDFontToolOption.h"
#include "GdipPNGWriter.h"
#include "PlaydateFntFileWriter.h"
#include "FontRenderer.h"



#pragma warning(disable : 6385)



/// https://docs.microsoft.com/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
int GdipPNGWriter::GetEncoderClsid
(
	const wchar_t* format,
	CLSID* pClsid
)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;  // Failure
	}

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;  // Failure
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		// #pragma warning(disable : 6385)
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);

	return -1;  // Failure
}



///
bool GdipPNGWriter::CreateStreamFromFile
(
	const wchar_t* szFilePath,
	CComPtr<IStream>& stream
)
{
	if (!::PathFileExists(szFilePath))
	{
		return false;
	}

	HANDLE hFile = NULL;
	HGLOBAL hGlobal = NULL;
	CComPtr< IStream > spStreamNow;
	bool bRet = false;

	do {
		hFile = ::CreateFile(
			szFilePath
			, GENERIC_READ
			, FILE_SHARE_READ | FILE_SHARE_WRITE
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			assert(false);
			break;
		}

		DWORD dwSize = ::GetFileSize(hFile, NULL);
		if (dwSize == 0)
		{
			bRet = true;
			break;
		}

		hGlobal = ::GlobalAlloc(GHND, dwSize);
		if (hGlobal == NULL)
		{
			break;
		}

		void* pvData = ::GlobalLock(hGlobal);
		if (pvData == NULL)
		{
			::GlobalUnlock(hGlobal);
			break;
		}

		DWORD dwRead = 0;
		if (!::ReadFile(hFile, pvData, dwSize, &dwRead, NULL))
		{
			::GlobalUnlock(hGlobal);
			assert(false);
			break;
		}

		::GlobalUnlock(hGlobal);

		if (dwRead != dwSize)
		{
			assert(false);
			break;
		}

		if (FAILED(::CreateStreamOnHGlobal(hGlobal, TRUE, &spStreamNow)))
		{
			assert(false);
			break;
		}

		if (spStreamNow == NULL)
		{
			break;
		}

		// hGlobal need not be released
		hGlobal = NULL;
		stream = spStreamNow;

		bRet = true;

	} while (false);

	if (hFile != NULL)
	{
		::CloseHandle(hFile);
	}

	if (hGlobal != NULL)
	{
		::GlobalFree(hGlobal);
	}

	return bRet;
}


///
bool GdipPNGWriter::LoadImageFromStream
(
	const CComPtr<IStream>& stream,
	std::shared_ptr<Gdiplus::Image>& image
)
{
	bool ret = false;

	auto bitmap = Gdiplus::Bitmap::FromStream(stream);
	if (bitmap == NULL)
	{
		return false;
	}

	if (bitmap->GetLastStatus() != Gdiplus::Ok)
	{
		delete bitmap;
		return false;
	}

	image = std::shared_ptr<Gdiplus::Image>(bitmap);

	return true;
}


///
bool GdipPNGWriter::LoadImageWithoutFileLock
(
	const wchar_t* szFilePath,
	std::shared_ptr<Gdiplus::Image>& image
)
{
	if (szFilePath == NULL)
	{
		assert(false);
		return NULL;
	}

	CComPtr<IStream> stream;
	if (!CreateStreamFromFile(szFilePath, stream))
	{
		//assert( false );
		return false;
	}

	return LoadImageFromStream(stream, image);
}



///
bool GdipPNGWriter::CreateBitmapFromImage
(
	const std::shared_ptr<Gdiplus::Image>& srcImage,
	std::shared_ptr<Gdiplus::Bitmap>& dstBitmap
)
{
	if (srcImage == NULL)
	{
		return false;
	}

	std::shared_ptr<Gdiplus::Bitmap> bitmap =
		std::make_shared<Gdiplus::Bitmap>(srcImage->GetWidth(), srcImage->GetHeight());
	if (bitmap == NULL)
	{
		return false;
	}

	std::shared_ptr <Gdiplus::Graphics> graphics =
		std::make_shared<Gdiplus::Graphics>(bitmap.get());
	if (graphics == NULL)
	{
		return false;
	}

	graphics->Clear(Gdiplus::Color::White);
	graphics->DrawImage(srcImage.get(), 0, 0);

	dstBitmap = bitmap;

	return true;
}


///
bool GdipPNGWriter::RecalcCharacterWidth
(
	std::shared_ptr<Gdiplus::Image>& srcImage,
	PDFontToolOption* option,
	const std::vector<UINT32>& vecUnicodeChars,
	std::shared_ptr<PlaydateFntFileWriter> writer,
	std::shared_ptr<Gdiplus::Image>& dstImage
)
{
	if (!option->enableRecalcCharacterWidth)
	{
		assert(false);
		return false;
	}

	std::shared_ptr<Gdiplus::Bitmap> srcBitmap;
	if (!CreateBitmapFromImage(srcImage, srcBitmap) || (srcBitmap == NULL))
	{
		return false;
	}

	std::shared_ptr<Gdiplus::Bitmap> dstBitmap =
		std::make_shared<Gdiplus::Bitmap>(srcImage->GetWidth(), srcImage->GetHeight());
	if (dstBitmap == NULL)
	{
		return false;
	}

	std::shared_ptr <Gdiplus::Graphics> dstGraphics =
		std::make_shared<Gdiplus::Graphics>(dstBitmap.get());
	if (dstGraphics == NULL)
	{
		return false;
	}
	dstGraphics->Clear(Gdiplus::Color::Transparent);

	int left = 0;
	int top = 0;
	const int charRightMargin = 2;
	for (auto ite = vecUnicodeChars.begin(); ite != vecUnicodeChars.end(); ite++)
	{
		int charMargin = option->marginOthers;
		if (*ite <= 127)
		{
			charMargin = option->marginAscii;
		}

		UINT32 uniChar = *ite;
		CRect rcText(CPoint(left, top), option->gridSize);
		bool bFindPixel = false;
		unsigned int leftX;
		unsigned int rightX;
		for (int x = rcText.left; x < rcText.right; x++)
		{
			for (int y = rcText.top; y < rcText.bottom; y++)
			{
				Gdiplus::Color color;
				srcBitmap->GetPixel(x, y, &color);
				if (color.ToCOLORREF() != 0x00ffffff)
				{
					if (!bFindPixel)
					{
						leftX = x;
						rightX = x;
						bFindPixel = true;
					}
					else
					{
						rightX = x;
					}
					break;
				}
			}
		}
		
		if (bFindPixel)
		{
			unsigned int charWidth = rightX - leftX + 1;
			writer->UpdateCharWidth(uniChar, charWidth + charMargin);
		
			dstGraphics->DrawImage(
				srcImage.get(),
				rcText.left,			// dst-left
				rcText.top,				// dst-top
				leftX,					// src-left
				rcText.top,				// src-top
				charWidth,				// src-width
				rcText.Height(),		// src-height
				Gdiplus::UnitPixel);
		}
		else
		{
			dstGraphics->DrawImage(
				srcImage.get(),
				rcText.left,			// dst-left
				rcText.top,				// dst-top
				rcText.left,			// src-left
				rcText.top,				// src-top
				rcText.Width(),			// src-width
				rcText.Height(),		// src-height
				Gdiplus::UnitPixel);
		}

	#ifdef ENABLE_UPDATE_SPACE_WIDTH
		// space : 0x20
		if (*ite == 0x20)
		{
			writer->UpdateCharWidth(*ite, option->spaceWidth);
		}
	#endif

		left += option->gridSize.cx;
		if (left >= option->gridSize.cx * GRID_COLUMN_COUNT)
		{
			left = 0;
			top += option->gridSize.cy;
		}
	}

	dstImage = dstBitmap;

	return true;
}


#ifdef ENABLE_ANTIALIASING
///
bool GdipPNGWriter::ConvertMonochrome
(
	std::shared_ptr<Gdiplus::Image>& srcImage,
	unsigned char threshold,
	std::shared_ptr<Gdiplus::Image>& dstImage
)
{
	std::shared_ptr<Gdiplus::Bitmap> srcBitmap;
	if (!CreateBitmapFromImage(srcImage, srcBitmap) || (srcBitmap == NULL))
	{
		return false;
	}

	std::shared_ptr<Gdiplus::Bitmap> thresholdBitmap =
		std::make_shared<Gdiplus::Bitmap>(
			srcBitmap->GetWidth(),
			srcBitmap->GetHeight(),
			PixelFormat32bppARGB);
	if (thresholdBitmap == NULL)
	{
		return false;
	}

	std::shared_ptr <Gdiplus::Graphics> thresholdGraphics =
		std::make_shared<Gdiplus::Graphics>(thresholdBitmap.get());
	if (thresholdGraphics == NULL)
	{
		return false;
	}

	thresholdGraphics->Clear(Gdiplus::Color::Transparent);
	for (UINT y = 0; y < srcBitmap->GetHeight(); y++)
	{
		for (UINT x = 0; x < srcBitmap->GetWidth(); x++)
		{
			Gdiplus::Color color;
			srcBitmap->GetPixel(x, y, &color);
			if ((color.GetR() + color.GetG() + color.GetB()) / 3 < threshold)
			{
				thresholdBitmap->SetPixel(x, y, Gdiplus::Color::Black);
			}
		}
	}

	dstImage = thresholdBitmap;

	return true;
}
#endif


///
bool GdipPNGWriter::WritePNGFile
(
	const wchar_t* pngPath,
	std::shared_ptr<Gdiplus::Image>& srcImage
)
{
#if 0
	std::shared_ptr<Gdiplus::Bitmap> gp1BitBitmap =
		std::make_shared<Gdiplus::Bitmap>(
			srcImage->GetWidth(),
			srcImage->GetHeight(),
			PixelFormat1bppIndexed);
	if (gp1BitBitmap == NULL)
	{
		return false;
	}

	std::shared_ptr <Gdiplus::Graphics> gp1BitGraphics =
		std::make_shared<Gdiplus::Graphics>(gp1BitBitmap.get());
	if (gp1BitGraphics == NULL)
	{
		return false;
	}
	gp1BitGraphics->DrawImage(srcImage.get(), 0, 0);

	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	return gp1BitBitmap->Save(pngPath, &encoderClsid) == Gdiplus::Ok;
#endif

	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	return srcImage->Save(pngPath, &encoderClsid) == Gdiplus::Ok;
}


///
bool GdipPNGWriter::RecreatePNGFileAccordingToOption
(
	const wchar_t* pngPath,
	PDFontToolOption* option,
	const std::vector<UINT32>& vecUnicodeChars,
	std::shared_ptr<PlaydateFntFileWriter> writer
)
{
	// Open without locking to re-save in Gdiplus.
	// PNGs generated by Gdiplus are smaller in size than PNGs generated by WIC.
	std::shared_ptr<Gdiplus::Image> image;
	if (!LoadImageWithoutFileLock(pngPath, image) || (image == NULL))
	{
		return false;
	}

	if (option->enableRecalcCharacterWidth)
	{
		std::shared_ptr<Gdiplus::Image> recalcImage;
		if (!RecalcCharacterWidth(image, option, vecUnicodeChars, writer, recalcImage))
		{
			return false;
		}

		if (!recalcImage)
		{
			return false;
		}

		image = recalcImage;
	}

#ifdef ENABLE_ANTIALIASING
	if (option->useAntialiasing)
	{
		std::shared_ptr<Gdiplus::Image> monoImage;
		if (!ConvertMonochrome(image, option->threshold, monoImage))
		{
			return false;
		}

		if (!monoImage)
		{
			return false;
		}

		image = monoImage;
	}
#endif

	return WritePNGFile(pngPath, image);
}


