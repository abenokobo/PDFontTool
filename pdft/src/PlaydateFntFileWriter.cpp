#include <assert.h>
#include "PlaydateFntFileWriter.h"
#include "CharConv.h"



///
bool PlaydateFntFileWriter::WriteUTF16LineWithConvertUTF8
(
	FILE* fp,
	const WCHAR* szUTF16Line
)
{
	std::vector<BYTE> utf8Line;
	if (!CharConv::WCharToUtf8(szUTF16Line, utf8Line))
	{
		return false;
	}

	fwrite(&utf8Line[0], sizeof(BYTE), utf8Line.size(), fp);
	fputc('\n', fp);

	return true;
}


///
PlaydateFntFileWriter::PlaydateFntFileWriter
(
)
	: m_file(NULL)
{
}


///
PlaydateFntFileWriter::~PlaydateFntFileWriter
(
)
{
}


///
bool PlaydateFntFileWriter::OpenFntFile
(
	const wchar_t* path
)
{
	if ((_wfopen_s(&m_file, path, L"wb") != 0) || !m_file)
	{
		return false;
	}

	WriteUTF16LineWithConvertUTF8(m_file, L"tracking=0");

	return true;
}


///
bool PlaydateFntFileWriter::AppendCharWidth
(
	UINT32 unicodeChar,
	unsigned int width
)
{
	m_charWidth.push_back(std::make_pair(unicodeChar, width));
	return true;
}


///
bool PlaydateFntFileWriter::UpdateCharWidth
(
	UINT32 unicodeChar,
	unsigned int width
)
{
	for (auto ite = m_charWidth.begin(); ite != m_charWidth.end(); ite++)
	{
		if (ite->first == unicodeChar)
		{
			ite->second = width;
			return true;
		}
	}

	assert(false);
	return false;
}


///
bool PlaydateFntFileWriter::AppendKerningPair
(
	UINT16 first,
	UINT16 second,
	int kernAmount
)
{
	if (kernAmount == 0)
	{
		assert(false);
		return false;
	}
	m_kerningPairs.push_back(KerningPair(first, second, kernAmount));
	return true;
}


///
bool PlaydateFntFileWriter::CloseFntFile
(
)
{
	if (!m_file)
	{
		return false;
	}

	while (!m_charWidth.empty())
	{
		auto ite = m_charWidth.begin();

		CString utf16Char;
		UTF32CharToUtf16Char(ite->first, utf16Char);

		CString strBuf;
		strBuf.Format(L"%s\t%d", utf16Char.GetBuffer(), ite->second);
		WriteUTF16LineWithConvertUTF8(m_file, strBuf.GetBuffer());

		m_charWidth.erase(ite);
	}

	while (!m_kerningPairs.empty())
	{
		auto ite = m_kerningPairs.begin();

		CString strBuf;
		strBuf.Format(L"%c%c\t%d", ite->first, ite->second, ite->kernAmount);
		WriteUTF16LineWithConvertUTF8(m_file, strBuf.GetBuffer());

		m_kerningPairs.erase(ite);
	}
	
	fclose(m_file);
	m_file = NULL;

	return true;
}



///
void PlaydateFntFileWriter::UTF32CharToUtf16Char
(
	UINT32 utf32Char,
	CString& utf16Char
)
{
	WCHAR szBuf[3];
	szBuf[1] = L'\0';
	szBuf[2] = L'\0';
	if (utf32Char <= 0xffff)
	{
		szBuf[0] = utf32Char;
		szBuf[1] = L'\0';
	}
	else
	{
		// Support surrogate pair
		szBuf[0] = (WCHAR)((utf32Char - 0x10000) / 0x400 + 0xd800);
		szBuf[1] = (WCHAR)((utf32Char - 0x10000) % 0x400 + 0xdc00);
		szBuf[2] = L'\0';
	}
	utf16Char = szBuf;
}


