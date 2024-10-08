//Author: Anthony West
//Contains routines for string manipulation
// -Every attempt should be made to keep this module at least Windows portable.

//---------------------------------------------------------------------------

#ifndef StringToolH
#define StringToolH

#include <windows.h>
#include <string>
#include <vector>

//---------------------------------------------------------------------------

namespace Subroutines
{

class TStrTool
{
public:

	////// only const static variables allowed in this public - see second public for regular stuff //////

	static const size_t NUM_HEX_CHARS = 16;
	static const char HEX_UPPER[NUM_HEX_CHARS];
	static const char HEX_LOWER[NUM_HEX_CHARS];
	static const char URL_HEX_ESCAPE = '%';

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
	TStrTool();
	~TStrTool();

	////// variables //////

	////// functions //////
	virtual void Destroy(); //calls Destroy_Private()
	virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

	static std::string UnicodeStrToUtf8(const std::wstring& str);
	static std::wstring Utf8ToUnicodeStr(const std::string& str);
	static std::string GetWindowsLastErrorCodeAsStringA(const DWORD winLastErr);
	static std::wstring GetWindowsLastErrorCodeAsStringW(const DWORD winLastErr);

	static bool StrCpyW(wchar_t* dest, size_t destSize_words, const wchar_t* src);
	static bool StrCpyA(char* dest, size_t destSize_bytes, const char* src);
	static bool StrCpyT(TCHAR* dest, size_t destArrayLen, const TCHAR* src);

	static bool StrNCpy_safeW(wchar_t* dest, size_t destSize_words, const wchar_t* src, size_t maxCount);
	static bool StrNCpy_safeA(char* dest, size_t destSize_bytes, const char* src, size_t maxCount);
	static bool StrNCpy_safeT(TCHAR* dest, size_t destArrayLen, const TCHAR* src, size_t maxCount);

    //For trim functions, see: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	static void TrimLeft(std::string& s);
    static void TrimLeft(std::string& s, const unsigned char trimChar, bool trim_isspace);
	static void TrimLeft(std::wstring& s);
    static void TrimLeft(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
	static void TrimRight(std::string& s);
    static void TrimRight(std::string& s, const unsigned char trimChar, bool trim_isspace);
	static void TrimRight(std::wstring& s);
    static void TrimRight(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
	static void Trim(std::string& s);
    static void Trim(std::string& s, const unsigned char trimChar, bool trim_isspace);
	static void Trim(std::wstring& s);
    static void Trim(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
	static std::string TrimLeft_Copy(std::string s);
    static std::string TrimLeft_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
	static std::wstring TrimLeft_Copy(std::wstring s);
    static std::wstring TrimLeft_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);
	static std::string TrimRight_Copy(std::string s);
    static std::string TrimRight_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
	static std::wstring TrimRight_Copy(std::wstring s);
    static std::wstring TrimRight_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);
	static std::string Trim_Copy(std::string s);
    static std::string Trim_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
	static std::wstring Trim_Copy(std::wstring s);
    static std::wstring Trim_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);

	static bool IsEmptyOrWhiteSpace(const std::string &s);
	static bool IsEmptyOrWhiteSpace(const std::wstring &s);

	static std::string CombinePathAndArgs(const std::string& path, const std::string& args);
	static std::wstring CombinePathAndArgs(const std::wstring& path, const std::wstring& args);

	static std::vector<std::string> Split(const std::string& str, const char sep);
	static std::vector<std::wstring> Split(const std::wstring& str, const wchar_t sep);

	static std::string DelimStr_Escape(const std::string& strIn, const char delim);
	static std::wstring DelimStr_Escape(const std::wstring& strIn, const wchar_t delim);
	static std::string DelimStr_UnEscape(const std::string& strIn, const char delim);
	static std::wstring DelimStr_UnEscape(const std::wstring& strIn, const wchar_t delim);

	static std::vector<std::string> CombineLists_Unique(const std::vector<std::string>& list1,
		const std::vector<std::string>& list2, bool ignoreBlank, bool ignoreCase);
	static std::vector<std::wstring> CombineLists_Unique(const std::vector<std::wstring>& list1,
		const std::vector<std::wstring>& list2, bool ignoreBlank, bool ignoreCase);

	static bool HexSingleToByte(const char hexSingle, BYTE* out);

	static bool URL_Split(const std::string& url, std::string* hostUtf8, std::string* pathUtf8);
	static std::string URL_EncodeUtf8(const std::string& valueUtf8, bool useUpperCaseHex = true);
	static std::string URL_DecodeUtf8(const std::string& encodedStr, bool* invalidHexEncountered);
	
};

} //end namespace

//---------------------------------------------------------------------------
#endif
