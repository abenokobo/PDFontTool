#include <assert.h>
#include <iostream>
#include <string>
#include "PDFontTool.h"


const std::wstring man = L"\
Usage: pdft [OPTIONS]... [SOURCE] [OUTPUT FOLDER] [OUTPUT FILENAME] [FONT SIZE]\n\
Font tool for Playdate.\n\
\n\
[SOURCE]            Source font file path.\n\
[OUTPUT FOLDER]     Output folder path.\n\
[OUTPUT FILENAME]   Output filename (without file extension).\n\
[FONT SIZE]         Output font size (pixel).\n\
\n\
[OPTIONS]\n\
-R\tEnable recalc character width. (Recalculation may takes longer.)\n\
-ma\tMargin for ASCII range (Must be specified with '-R')\n\
-mo\tMargin for outside ASCII range (Must be specified with '-R')\n\
";



///
static bool ParseArg
(
    int argc,
    wchar_t* argv[],
    std::wstring& inFilePath,
    std::wstring& outFolderPath,
    std::wstring& outFileName,
    unsigned int& fontsize,
    std::vector<std::wstring>& vecOption
)
{
    if (argc < 5)
    {
        std::wcout << L"Invalid arguments." << std::endl << std::endl;
        return false;
    }

    for (int i = 1; i < argc - 4; i++)
    {
        vecOption.push_back(argv[i]);
    }

    inFilePath = argv[argc - 4];
    if (!::PathFileExists(inFilePath.c_str()))
    {
        std::wcout << inFilePath << L" is not exist file." << std::endl << std::endl;
        return false;
    }

    outFolderPath = argv[argc - 3];
    if (!::PathIsDirectory(outFolderPath.c_str()))
    {
        std::wcout << outFolderPath << L" is not exist folder." << std::endl << std::endl;
        return false;
    }

    outFileName = argv[argc - 2];
    fontsize = _wtoi(argv[argc - 1]);
 
    return true;
}


///
static bool ParseOption
(
    const std::vector<std::wstring>& vecOption,
    PDFontToolOption& option
)
{
    for (auto ite = vecOption.begin(); ite != vecOption.end(); ite++)
    {
#ifdef ENABLE_UPDATE_SPACE_WIDTH
        if (*ite == L"-s")
        {
            ite++;
            if (ite == vecOption.end())
            {
                std::wcout << L"-s OPTION require a numeric argument." << std::endl << std::endl;
                return false;
            }
            option.spaceWidth = _wtoi(ite->c_str());
        }
        else
#endif
        if (*ite == L"-R")
        {
            option.enableRecalcCharacterWidth = true;
        }
        else
        if (*ite == L"-ma")
        {
            ite++;
            if (ite == vecOption.end())
            {
                std::wcout << L"'-ma' require a numeric argument." << std::endl << std::endl;
                return false;
            }
            option.marginAscii = _wtoi(ite->c_str());
        }
        else
        if (*ite == L"-mo")
        {
            ite++;
            if (ite == vecOption.end())
            {
                std::wcout << L"'-mo ' require a numeric argument." << std::endl << std::endl;
                return false;
            }
            option.marginOthers = _wtoi(ite->c_str());
        }
#ifdef ENABLE_ANTIALIASING
        else
        if (*ite == L"-a")
        {
            option.useAntialiasing = true;
        }
        else
        if (*ite == L"-t")
        {
            ite++;
            if (ite == vecOption.end())
            {
                std::wcout << L"'-t ' require a numeric argument." << std::endl << std::endl;
                return false;
            }
            option.threshold = _wtoi(ite->c_str());
            if (option.threshold < 0 or option.threshold > 255)
            {
                std::wcout << L"'-t ' must be in the range 1-255" << std::endl << std::endl;
                return false;
            }
        }
#endif
        else
        {
            std::wcout << *ite << L" is invalid argument." << std::endl << std::endl;
            return false;
        }
    }

    return true;
}


///
static bool DoCreate
(
    const std::wstring& inFilePath,
    const std::wstring& outFolderPath,
    const std::wstring& outFileName,
    unsigned int fontsize,
    const std::vector<std::wstring>& vecOption
)
{
    PDFontTool tool(fontsize, inFilePath.c_str(), outFolderPath.c_str(), outFileName.c_str());
    if (!ParseOption(vecOption, tool.m_option))
    {
        return false;
    }

    std::wcout << L"Input File Path: " << inFilePath << std::endl;
    std::wcout << L"Output Folder Path: " << outFolderPath << std::endl;
    std::wcout << L"Output File Name: " << outFileName << std::endl << std::endl;
    std::wcout << L"Creating Playdate font ..." << std::endl;

    if (!tool.CreatePDFont())
    {
        return false;
    }

    return true;
}


///
int wmain
(
    int argc,
    wchar_t* argv[]
)
{
    if (argc == 1)
    {
        std::wcout << man;
        return 0;
    }

    std::wstring inFilePath;
    std::wstring outFolderPath;
    std::wstring outFileName;
    unsigned int fontSize;
    std::vector<std::wstring> vecOption;
    if (!ParseArg(argc, argv, inFilePath, outFolderPath, outFileName, fontSize, vecOption))
    {
        std::wcout << man;
        return -1;
    }

    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (!SUCCEEDED(hr))
    {
        std::wcout << L"Failed to initialize COM." << std::endl;
        assert(false);
        return -1;
    }

    if (!DoCreate(inFilePath, outFolderPath, outFileName, fontSize, vecOption))
    {
        return -1;
    }

    std::cout << "Create Playdate font finished!" << std::endl;

    ::CoUninitialize();

    return 0;
}



