//Author: Anthony West
//Contains routines for working with apps.
// -Every attempt should be made to keep this module at least Windows portable.

//---------------------------------------------------------------------------

#ifndef AppToolH
#define AppToolH

#include "Version.h"
//---------------------------------------------------------------------------

namespace Subroutines
{

//for version comparison checking
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
	virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

    static std::string GetAppPathA();
    static std::wstring GetAppPathW();
    static bool GetAppVersion(const char* appOrDLLPath, WORD* outMajorVer,
        WORD* outMinorVer, WORD* outBuild, WORD* outRevision);
    static bool GetAppVersion(const wchar_t* appOrDLLPath, WORD* outMajorVer,
        WORD* outMinorVer, WORD* outBuild, WORD* outRevision);
};


} //end namepsace
#endif
