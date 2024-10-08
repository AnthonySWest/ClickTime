//Author: Anthony West
//Contains routines for file/folder path manipulation
// -Every attempt should be made to keep this module at least Windows portable.

//---------------------------------------------------------------------------

#ifndef PathToolH
#define PathToolH

#include <windows.h>
#include <string>
//---------------------------------------------------------------------------

namespace Subroutines
{

class TPathTool
{
public:

	////// only const static variables allowed in this public - see second public for regular stuff //////

    static const size_t MaxLen_UnicodeFileName = 32767;

    static const std::string AlphaCharsA;
    static const std::wstring AlphaCharsW;


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
	TPathTool();
	~TPathTool();

	////// variables //////

	////// functions //////
	virtual void Destroy(); //calls Destroy_Private()
	virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

    static bool IsDots(std::string str);
    static bool IsDots(std::wstring str);

    static std::string File_ExtractDir(const std::string &fileName, bool keepTrailSlash=true);
    static std::wstring File_ExtractDir(const std::wstring &fileName, bool keepTrailSlash=true);

    static bool Path_IsRelative(const std::string &path);
	static bool Path_IsRelative(const std::wstring &path);
    static bool Path_IsEnvironment(const std::string &path);
	static bool Path_IsEnvironment(const std::wstring &path);
    static bool Path_IsNetwork(const std::string &path);
	static bool Path_IsNetwork(const std::wstring &path);
    static std::string Path_Combine(const std::string& path1, const std::string& path2);
    static std::wstring Path_Combine(const std::wstring& path1, const std::wstring& path2);
    static bool Path_ExpandEnvironmentVars(const std::string& path, std::string &dest);
	static bool Path_ExpandEnvironmentVars(const std::wstring& path, std::wstring &dest);
    static std::string Path_ExtractFileName(const std::string& path);
	static std::wstring Path_ExtractFileName(const std::wstring& path);
	static std::string Path_ExtractFileName(const std::string& path, bool removeExtension);
	static std::wstring Path_ExtractFileName(const std::wstring& path, bool removeExtension);
	static std::string Path_RemoveExtension(const std::string& path);
	static std::wstring Path_RemoveExtension(const std::wstring& path);
	static std::string Path_GetExtension(const std::string& path);
	static std::wstring Path_GetExtension(const std::wstring& path);
    static std::string Path_GenerateRandomNameA(size_t len);
    static std::string Path_GenerateRandomName(size_t len, const std::string& charList);
    static std::wstring Path_GenerateRandomNameW(size_t len);
    static std::wstring Path_GenerateRandomName(size_t len, const std::wstring& charList);

};

} //end namespace

//---------------------------------------------------------------------------
#endif
