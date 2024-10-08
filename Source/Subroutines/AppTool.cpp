//---------------------------------------------------------------------------
// AppTool.cpp
// Author: Anthony West - ASW Software
//
// See header for info.
//---------------------------------------------------------------------------

#pragma hdrstop

#include "AppTool.h"

#ifdef USE_ELOG
    #include "EasyLogger.h"
#endif

//---------------------------------------------------------------------------

namespace Subroutines
{

//---------------------- class statics --------------------------------------

/////////////////////////////////////////////////////////////////////////////
// TAppTool class
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TAppTool::TAppTool()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TAppTool::~TAppTool()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TAppTool::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TAppTool::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TAppTool::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here

    return true;
}
//---------------------------------------------------------------------------
bool TAppTool::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
// -Static
// -Get application directory (gets correct dir, even when run from ZIP)
std::string TAppTool::GetAppPathA()
{
    //see: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlongpathnamea
    const size_t maxFileNameLen = MaxLen_UnicodeFileName;
    int numCharsWritten;
    char* appNamePath = new char[maxFileNameLen + 1];
    DWORD lenCopied;
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    *appNamePath = '\0';
    lenCopied = ::GetModuleFileNameA(hInstance, appNamePath, static_cast<DWORD>(maxFileNameLen));

    if (maxFileNameLen == lenCopied || 0 == lenCopied)
    {
        delete [] appNamePath;
        return "";
    }

    char* longDir = new char[maxFileNameLen + 1]; //stores expanded directory path (no ~)

    *longDir = '\0';
    //make sure not DOS path
    lenCopied = ::GetLongPathNameA(appNamePath, longDir, static_cast<DWORD>(maxFileNameLen));

    //buffer to small - resize and try again (should never happen, given the original buffer size)
    if (lenCopied >= maxFileNameLen)
    {
        size_t newlongDirSize = lenCopied;
        delete [] longDir;
        longDir = new char[newlongDirSize];
        *longDir = '\0';
        lenCopied = ::GetLongPathNameA(appNamePath, longDir, static_cast<DWORD>(newlongDirSize));
    }

    std::string resultPath;

    if (0 == lenCopied) //error condition
        resultPath = "";
    else
        resultPath = longDir;

    delete [] appNamePath;
    delete [] longDir;

    return resultPath;
}
//---------------------------------------------------------------------------
// -Static
// -Get application directory (gets correct dir, even when run from ZIP)
std::wstring TAppTool::GetAppPathW()
{
    //see: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlongpathnamea
    const size_t maxFileNameLen = MaxLen_UnicodeFileName;
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
bool TAppTool::GetAppVersion(const char* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
    WORD* outRevision)
{
    if (nullptr == appOrDLLPath || nullptr == outMajorVer || nullptr == outMinorVer || nullptr == outBuild ||
        nullptr == outRevision)
    {
        return false;
    }

    //Note: code from MSDN, with a few modifications. Uses Winver.h.

    DWORD dwDummy=0, dwLen;
    UINT bufLen;
    LPVOID lpData;
    VS_FIXEDFILEINFO *pFileInfo;

    dwLen = ::GetFileVersionInfoSizeA(appOrDLLPath, &dwDummy);
    if (!dwLen)
        return false;

    lpData = (LPVOID)malloc(dwLen);
    if (!lpData)
        return false;

    bool result = ::GetFileVersionInfoA(appOrDLLPath, 0, dwLen, lpData);

    if(result && ::VerQueryValueA(lpData, "\\",(LPVOID *)&pFileInfo,(PUINT)&bufLen ))
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
bool TAppTool::GetAppVersion(const wchar_t* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
    WORD* outRevision)
{
    if (nullptr == appOrDLLPath || nullptr == outMajorVer || nullptr == outMinorVer || nullptr == outBuild ||
        nullptr == outRevision)
    {
        return false;
    }

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

//---------------------------------------------------------------------------

} // namespace Subroutines
