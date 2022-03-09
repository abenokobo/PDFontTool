#pragma once

#include <memory>
#include <gdiplus.h>
#include <vector>



class PDFontToolOption;
class PlaydateFntFileWriter;



class GdipPNGWriter
{
protected:


	///
	static int GetEncoderClsid(const wchar_t* format, CLSID* pClsid);

	///
	static bool CreateStreamFromFile(const wchar_t* szFilePath, CComPtr<IStream>& stream);

	///
	static bool LoadImageFromStream(const CComPtr<IStream>& stream, std::shared_ptr<Gdiplus::Image>& image);

	///
	static bool LoadImageWithoutFileLock(const wchar_t* szFilePath, std::shared_ptr<Gdiplus::Image>& image);

	///
	static bool CreateBitmapFromImage(const std::shared_ptr<Gdiplus::Image>& srcImage, std::shared_ptr<Gdiplus::Bitmap>& dstBitmap);

	///
	static bool RecalcCharacterWidth(std::shared_ptr<Gdiplus::Image>& srcImage, PDFontToolOption* option, const std::vector<UINT32>& vecUnicodeChars, std::shared_ptr<PlaydateFntFileWriter> writer, std::shared_ptr<Gdiplus::Image>& dstImage);

	///
	static bool ConvertMonochrome(std::shared_ptr<Gdiplus::Image>& srcImage, unsigned char threshold, std::shared_ptr<Gdiplus::Image>& dstImage);

	///
	static bool WritePNGFile(const wchar_t* pngPath, std::shared_ptr<Gdiplus::Image>& srcImage);



public:


	///
	static bool RecreatePNGFileAccordingToOption(const wchar_t* pngPath, PDFontToolOption* option, const std::vector<UINT32>& vecUnicodeChars, std::shared_ptr<PlaydateFntFileWriter> writer);
};


