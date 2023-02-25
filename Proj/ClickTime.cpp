//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("..\MainForm.cpp", FrmMain);
//---------------------------------------------------------------------------
#include "MainForm.h"

std::wstring GetAppPathW();
bool GetAppVersionW(const wchar_t* appOrDLLPath, WORD* outMajorVer,
    WORD* outMinorVer, WORD* outBuild, WORD* outRevision);
std::wstring File_ExtractDirW(const std::wstring &fileName, bool keepTrailSlash=true);
std::wstring Path_Combine(const std::wstring& path1, const std::wstring& path2);

using namespace Subroutines;
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    const wchar_t *codeSectStr = L"_tWinMain";
    //_TCHAR** argvP = argv; //for _tmain //_TCHAR** argvP = __targv; //for _tWinMain
    _TCHAR** argvP = __targv;
    int nArgs = __argc;//argc;
    unsigned short major = 0, minor = 0, build = 0, revision = 0;
    const size_t buffer_t_size = 999;
    TCHAR buffer_t[buffer_t_size];
    std::wstring exePathW;

    try
    {
        //get full path, not local path, and not DOS path either
        exePathW = GetAppPathW();

        if (exePathW.length() == 0)
            exePathW = argvP[0]; //fall back to local

        if (GetAppVersionW(exePathW.c_str(), &major, &minor, &build, &revision))
            TFrmMain::AppVersion.SetVersion(major, minor, build, revision);

        //set up dir and file locations
		TFrmMain::DirApp = File_ExtractDirW(exePathW).c_str();
        TFrmMain::FileSettingsINI = Path_Combine(TFrmMain::DirApp.w_str(), TFrmMain::FileSettingsINI.w_str());

        //load ini (future)

        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->Title = "ClickTime";
        TStyleManager::TrySetStyle("Windows10 SlateGray");
        Application->CreateForm(__classid(TFrmMain), &FrmMain);
        Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
//Get application directory (gets correct dir, even when run from ZIP)
std::wstring GetAppPathW()
{
    const size_t maxFileNameLen = 32767; //see: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlongpathnamea
    int numCharsWritten;
    wchar_t* appNamePath = new wchar_t[maxFileNameLen + 1];
    DWORD lenCopied;
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

    *appNamePath = L'\0';
	lenCopied = ::GetModuleFileNameW(hInstance, appNamePath, static_cast<DWORD>(maxFileNameLen));

    if (maxFileNameLen == lenCopied || 0 == lenCopied)
    {
        delete [] appNamePath;
        return L"";
    }

    wchar_t* longDir = new wchar_t[maxFileNameLen + 1]; //stores expanded directory path (no ~)

    *longDir = L'\0';
    //make sure not DOS path
	lenCopied = ::GetLongPathNameW(appNamePath, longDir, static_cast<DWORD>(maxFileNameLen));

    //buffer to small - resize and try again (should never happen, given the original buffer size)
    if (lenCopied >= maxFileNameLen)
    {
        size_t newlongDirSize = lenCopied;
        delete [] longDir;
        longDir = new wchar_t[newlongDirSize];
        *longDir = L'\0';
        lenCopied = ::GetLongPathNameW(appNamePath, longDir, static_cast<DWORD>(newlongDirSize));
    }

    std::wstring resultPath;

    if (0 == lenCopied) //error condition
        resultPath = L"";
    else
    	resultPath = longDir;

	delete [] appNamePath;
    delete [] longDir;

	return resultPath;
}

//---------------------------------------------------------------------------
bool GetAppVersionW(const wchar_t* appOrDLLPath, WORD* outMajorVer,
    WORD* outMinorVer, WORD* outBuild, WORD* outRevision)
{
    if (nullptr == appOrDLLPath || nullptr == outMajorVer || nullptr == outMinorVer || nullptr == outBuild || nullptr == outRevision)
        return false;

    //Note: code from MSDN, with a few modifications. Uses Winver.h.

    DWORD dwDummy=0, dwLen;
    UINT bufLen;
    LPVOID lpData;
    VS_FIXEDFILEINFO *pFileInfo;

	dwLen = ::GetFileVersionInfoSizeW(appOrDLLPath, &dwDummy);
	if (!dwLen)
		return false;

	lpData = (LPVOID)malloc(dwLen);
	if (!lpData)
		return false;

	bool result = ::GetFileVersionInfoW(appOrDLLPath, 0, dwLen, lpData);

    if(result && ::VerQueryValueW(lpData, L"\\",(LPVOID *)&pFileInfo,(PUINT)&bufLen ))
	{
		*outMajorVer = HIWORD(pFileInfo->dwFileVersionMS);
		*outMinorVer = LOWORD(pFileInfo->dwFileVersionMS);
		*outBuild = HIWORD(pFileInfo->dwFileVersionLS);
		*outRevision = LOWORD(pFileInfo->dwFileVersionLS);
	}

	free (lpData);
	return result;
}
//---------------------------------------------------------------------------
std::wstring File_ExtractDirW(const std::wstring &fileName, bool keepTrailSlash)
{
    size_t splitIdx = fileName.find_last_of(L"/\\");
	if (splitIdx == std::wstring::npos)
		return L"";
	return fileName.substr(0, splitIdx + (keepTrailSlash ? 1 : 0));
}
//---------------------------------------------------------------------------
std::wstring Path_Combine(const std::wstring& path1, const std::wstring& path2)
{
    if (path1.length() == 0)
		return path2;
	else if (path2.length() == 0)
		return path1;
	else if (((path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/') && (path2[0] != L'\\' && path2[0] != L'/')) ||
		((path1[path1.length() - 1] != L'\\' && path1[path1.length() - 1] != L'/') && (path2[0] == L'\\' || path2[0] == L'/')))
		return path1 + path2; //one of the paths has a slash in the proper spot, so combine and return
	else if (path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/')
		return path1 + path2.substr(1, path2.length() - 1); //path1 ends with slash and path2 begins with slash, return combined with just one slash
	return path1 + L"\\" + path2; //neither path has a slash at the join location, return combined with a slash
}
//---------------------------------------------------------------------------

