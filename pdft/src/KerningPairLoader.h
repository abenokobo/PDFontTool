#pragma once
#include <memory>
#include <atlstr.h>
#include <atlcomcli.h>
#include <dwrite_3.h>


class PlaydateFntFileWriter;
class FontLoader;



class KerningPairLoader
{
private:


	///
	CString m_fontPath;

	///
	std::shared_ptr<FontLoader> m_loader;

	///
	std::shared_ptr<PlaydateFntFileWriter> m_writer;



	///
	HFONT CreateGDIFont();

	///
	HDC CreateFontDC();

	///
	bool LoadKerningPairs(HDC hDC);


public:


	///
	KerningPairLoader(std::shared_ptr<FontLoader>& loader, std::shared_ptr<PlaydateFntFileWriter>& writer);

	///
	~KerningPairLoader();

	///
	bool LoadKerningPairs();
};

