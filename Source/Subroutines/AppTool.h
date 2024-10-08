//---------------------------------------------------------------------------
// AppTool.h
// Author: Anthony West - ASW Software
//
// Contains routines for working with apps.
//
// Every attempt should be made to keep this module at least Windows portable.
//---------------------------------------------------------------------------

#ifndef AppToolH
#define AppToolH
//---------------------------------------------------------------------------
#include "Version.h"
//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TAppTool class
/////////////////////////////////////////////////////////////////////////////
class TAppTool
{
typedef TAppTool ThisType;

public:
    ////// only const static variables allowed in this public - see second public for regular stuff //////

    static const size_t MaxLen_UnicodeFileName = 32767;

protected:
    ////// static variables //////

    ////// variables //////

    ////// functions //////

private:
    ////// static variables //////

    ////// variables //////

    ////// functions //////
    void Destroy_Private();
    bool Reset_Private();

public:
    TAppTool();
    ~TAppTool();

    ////// variables //////

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for a constructor to not call a virtual function

    static std::string GetAppPathA();
    static std::wstring GetAppPathW();
    static bool GetAppVersion(const char* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
    static bool GetAppVersion(const wchar_t* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
};

} // namespace Subroutines

#endif // #ifndef AppToolH
