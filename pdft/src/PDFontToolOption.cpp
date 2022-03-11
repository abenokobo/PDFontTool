#include "PDFontToolOption.h"



///
PDFontToolOption::PDFontToolOption
(
	int nFontSize,
	const wchar_t* szFontPath,
	const wchar_t* szDstPath,
	const wchar_t* szDstFileName
) :
fontSize(nFontSize),
fontPath(szFontPath),
dstPath(szDstPath),
dstFileName(szDstFileName),
gridSize(0, 0),
enableRecalcCharacterWidth(false),
marginAscii(2),
marginOthers(4)
#ifdef ENABLE_UPDATE_SPACE_WIDTH
, spaceWidth(6)
#endif
#ifdef ENABLE_ANTIALIASING
, useAntialiasing(false),
threshold(-1)
#endif
{
}


///
PDFontToolOption::~PDFontToolOption
(
)
{
}




