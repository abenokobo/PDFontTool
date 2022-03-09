#pragma once
#include <memory>
#include "PDFontToolOption.h"



class FontLoader;
class FontRenderer;
class PlaydateFntFileWriter;




class PDFontTool
{
private:


	///
	std::shared_ptr<FontLoader> m_loader;

	///
	std::shared_ptr<FontRenderer> m_renderer;

	///
	ULONG_PTR m_gt;



private:





public:


	///
	PDFontToolOption m_option;

	///
	PDFontTool(int nFontSize, const wchar_t* szFontPath, const wchar_t* szDstPath, const wchar_t* szDstFileName);

	///
	~PDFontTool();

	///
	bool CreatePDFont();
};



