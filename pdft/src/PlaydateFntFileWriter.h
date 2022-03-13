#pragma once
#include <vector>
#include <atlstr.h>


struct KerningPair
{
	UINT16 first;
	UINT16 second;
	int kernAmount;

	KerningPair(UINT16 fst, UINT16 snd, int amount)
		: first(fst), second(snd), kernAmount(amount)
	{
	}
};


///
class PlaydateFntFileWriter
{
private:


	///
	FILE* m_file;

	///
	std::vector<std::pair<UINT32, unsigned int>> m_charWidth;

	///
	std::vector<KerningPair> m_kerningPairs;



private:


	///
	static bool WriteUTF16LineWithConvertUTF8(FILE* fp, const WCHAR* szUTF16Line);



public:


	///
	PlaydateFntFileWriter();

	///
	~PlaydateFntFileWriter();

	///
	bool OpenFntFile(const wchar_t* path);

	///
	bool AppendCharWidth(UINT32 unicodeChar, unsigned int width);

	///
	bool UpdateCharWidth(UINT32 unicodeChar, unsigned int width);

	///
	bool AppendKerningPair(UINT16 first, UINT16 second, int kernAmount);

	///
	bool CloseFntFile();

	///
	static void UTF32CharToUtf16Char(UINT32 utf32Char, CString& utf16Char);
};

