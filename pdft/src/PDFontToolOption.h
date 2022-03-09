#pragma once
#include <atltypes.h>
#include <atlstr.h>
#include <vector>



class PDFontToolOption
{
public:

	
	/*
		Mandatory
	*/


	///
	int fontSize;

	///
	CString fontPath;
	
	///
	CString dstPath;

	///
	CString dstFileName;



	/*
		Optional
	*/

	///
	CSize fontBoxSize;



	/*
		RecalcCharacterWidth
	*/


	///
	bool enableRecalcCharacterWidth;

	///
	int marginAscii;

	///
	int marginOthers;

#ifdef ENABLE_UPDATE_SPACE_WIDTH
	///
	int spaceWidth;
#endif

#ifdef ENABLE_ANTIALIASING
	///
	bool useAntialiasing;

	///
	int threshold;
#endif




	///
	PDFontToolOption(int nFontSize, const wchar_t* szFontPath, const wchar_t* szDstPath, const wchar_t* szDstFileName);

	///
	~PDFontToolOption();
};


