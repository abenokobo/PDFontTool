#include "CharConv.h"
#include <assert.h>




///
bool CharConv::WCharToUtf8
(
	const WCHAR* szSrc,
	std::vector<BYTE>& utf8Buffer
)
{
	if (szSrc == NULL)
	{
		assert(false);
		return false;
	}

	int nSize = ::WideCharToMultiByte(CP_UTF8, 0, szSrc, -1, NULL, 0, NULL, NULL);
	utf8Buffer.resize(nSize);
	
	if (::WideCharToMultiByte(
		CP_UTF8, 0, szSrc, -1,
		reinterpret_cast<LPSTR>(&utf8Buffer[0]), nSize,
		NULL, NULL ) == 0)
	{
		return false;
	}

	if (utf8Buffer.back() == L'\0')
	{
		utf8Buffer.pop_back();
	}

	return true;
}





