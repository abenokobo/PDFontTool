#pragma once
#include <atlstr.h>
#include <vector>


/*
	LCSCharConv
*/





class CharConv
{
public:



	/// 
	static bool WCharToUtf8(const WCHAR* szSrc, std::vector<BYTE>& utf8Buffer);
};


