//Author: Anthony West
//See header for info.
//---------------------------------------------------------------------------

#pragma hdrstop

#include <codecvt>
#include <wctype.h>
//#include <cctype>
//#include <iomanip>
//#include <sstream>

#include "StringTool.h"

#ifndef strcmpI
	#ifdef __BORLANDC__
        #define strcmpI stricmp
	#else
		#define strcmpI _stricmp
	#endif
#endif // strcmpI

#ifdef _MSC_VER
	//This will use the VS safe string functions to avoid the "may be unsafe" warnings.
	//Note that _CRT_SECURE_NO_WARNINGS can also be defined in order to ignore the warnings.
	#define	USE_SAFESTR_FUNCS
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

namespace Subroutines
{

//---------------------- class statics --------------------------------------

const char TStrTool::HEX_UPPER[NUM_HEX_CHARS] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const char TStrTool::HEX_LOWER[NUM_HEX_CHARS] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

/////////////////////////////////////////////////////////////////////////////
//					         TStrTool class
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TStrTool::TStrTool()
{
	this->Reset_Private();
}
//---------------------------------------------------------------------------
TStrTool::~TStrTool()
{
	this->Destroy_Private();
}
//---------------------------------------------------------------------------
void TStrTool::Destroy_Private()
{
	//code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TStrTool::Destroy() //virtual
{
	this->Destroy_Private();
}
//---------------------------------------------------------------------------
bool TStrTool::Reset_Private()
{
	//code specific to this level of inheritance goes here - no virtual functions can be called here
	this->Destroy_Private();

	//reset class vars here


	return true;
}
//---------------------------------------------------------------------------
bool TStrTool::Reset() //virtual
{
	return this->Reset_Private();
}
//---------------------------------------------------------------------------
// - Static
std::string TStrTool::UnicodeStrToUtf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::Utf8ToUnicodeStr(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}
//---------------------------------------------------------------------------
// -Static
// -Parameter 'winLastErr' must be the value of API call GetLastError()
std::string TStrTool::GetWindowsLastErrorCodeAsStringA(const DWORD winLastErr)
{
	LPSTR error = NULL;
	std::string errorStr;

	//translate the windows error code to string
	if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, winLastErr, 0, (LPSTR)&error, 0, NULL) == 0)
	{
		//if failed to get the error string, show the error hex number instead
		const size_t error2Size = 96;
		char error2[error2Size];
		sprintf_s(error2, error2Size, "Unknown error: %u (0x%08lx)", winLastErr, winLastErr);
		errorStr = error2;
	}
	else if (error != NULL)
	{
		errorStr = error;
		::LocalFree(error);
	}

    //A trailing newline is sometimes present. Remove.
	TrimRight(errorStr);
	return errorStr;
}
//---------------------------------------------------------------------------
// -Static
// -Parameter 'winLastErr' must be the value of API call GetLastError()
std::wstring TStrTool::GetWindowsLastErrorCodeAsStringW(const DWORD winLastErr)
{
	LPWSTR error = NULL;
	std::wstring errorStr;

	//translate the windows error code to string
	if (::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, winLastErr, 0, (LPWSTR)&error, 0, NULL) == 0)
	{
		//if failed to get the error string, show the error hex number instead
		const size_t error2Size = 96;
		wchar_t error2[error2Size];
		swprintf_s(error2, error2Size, L"Unknown error: %u (0x%08lx)", winLastErr, winLastErr);
		errorStr = error2;
	}
	else if (error != NULL)
	{
		errorStr = error;
		::LocalFree(error);
	}

	//A trailing newline is sometimes present. Remove.
	TrimRight(errorStr);
	return errorStr;
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//	destSize_words, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_words is zero.
// -Ignores locale.
bool TStrTool::StrCpyW(wchar_t* dest, size_t destSize_words, const wchar_t* src)
{
	if (nullptr == dest || destSize_words == 0)
		return false;

	wchar_t* destWalker = dest;
	const wchar_t* srcWalker = src;
	size_t count = 0, maxCopy = destSize_words - sizeof('\0');

	while (count < maxCopy && *srcWalker)
	{
		*destWalker++ = *srcWalker++;
		count++;
	}

	*destWalker = L'\0';
	return true;
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//	destSize_bytes, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_bytes is zero.
// -Ignores locale.
bool TStrTool::StrCpyA(char* dest, size_t destSize_bytes, const char* src)
{
	if (nullptr == dest || destSize_bytes == 0)
		return false;

	char* destWalker = dest;
	const char* srcWalker = src;
	size_t count = 0, maxCopy = destSize_bytes - sizeof('\0');

	while (count < maxCopy && *srcWalker)
	{
		*destWalker++ = *srcWalker++;
		count++;
	}

	*destWalker = '\0';
	return true;
}
//---------------------------------------------------------------------------
// -Static
// -Performs a copy by calling either StrCpyW() or StrCpyA(), depending on
//	the project TCHAR type (unicode or not).
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//	destArrayLen, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destArrayLen is zero.
// -Ignores locale.
bool TStrTool::StrCpyT(TCHAR* dest, size_t destArrayLen, const TCHAR* src)
{
#ifdef _UNICODE
	return StrCpyW(dest, destArrayLen, src);
#else
	return StrCpyA(dest, destArrayLen, src);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//	comes first, not exceeding destSize_words, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_words is zero.
bool TStrTool::StrNCpy_safeW(wchar_t* dest, size_t destSize_words, const wchar_t* src, size_t maxCount)
{
	if (nullptr == dest || destSize_words == 0)
		return false;

	//Ensure that maxCount does not exceed the destination buffer size with null terminator.
	if (maxCount >= destSize_words)
		maxCount = destSize_words - sizeof('\0');

	if (0 == maxCount || nullptr == src)
	{
		*dest = L'\0';
		return true; //nothing to copy
	}

#ifdef USE_SAFESTR_FUNCS
	//wcsncpy_s(dest, bufferLen, src, _TRUNCATE);
	return (0 == wcsncpy_s(dest, destSize_words, src, maxCount));
#else
	wcsncpy(dest, src, maxCount);
	dest[maxCount] = L'\0';
	return true;
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//	comes first, not exceeding destSize_bytes, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_bytes is zero.
bool TStrTool::StrNCpy_safeA(char* dest, size_t destSize_bytes, const char* src, size_t maxCount)
{
	if (nullptr == dest || destSize_bytes == 0)
		return false;

	//Ensure that maxCount does not exceed the destination buffer size with null terminator.
	if (maxCount >= destSize_bytes)
		maxCount = destSize_bytes - sizeof('\0');

	if (0 == maxCount || nullptr == src)
	{
		*dest = '\0';
		return true; //nothing to copy
	}

#ifdef USE_SAFESTR_FUNCS
	//strncpy_s(dest, bufferLen, src, _TRUNCATE);
	return (0 == strncpy_s(dest, destSize_bytes, src, maxCount));
#else
	strncpy(dest, src, maxCount);
	dest[maxCount] = '\0';
	return true;
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Performs a copy by calling either StrNCpy_safeW() or StrNCpy_safeA(), depending on
//	the project TCHAR type (unicode or not).
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//	comes first, not exceeding destArrayLen, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destArrayLen is zero.
bool TStrTool::StrNCpy_safeT(TCHAR* dest, size_t destArrayLen, const TCHAR* src, size_t maxCount)
{
#ifdef _UNICODE
	return StrNCpy_safeW(dest, destArrayLen, src, maxCount);
#else
	return StrNCpy_safeA(dest, destArrayLen, src, maxCount);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimLeft(std::string& s)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(), [](unsigned char ch)
			{
				return !std::isspace(ch);
			})
		);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::TrimLeft(std::string& s, const unsigned char trimChar, bool trim_isspace)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(), [trimChar, trim_isspace](unsigned char ch)
			{
				return trim_isspace ? (!std::isspace(ch) && trimChar != ch) : trimChar != ch;
			})
		);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimLeft(std::wstring& s)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(), [](wchar_t ch)
			{
				return !std::iswspace(ch);
			})
	);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::TrimLeft(std::wstring& s, const wchar_t trimChar, bool trim_iswspace)
{
	s.erase(s.begin(),
		std::find_if(s.begin(), s.end(), [trimChar, trim_iswspace](wchar_t ch)
			{
				return trim_iswspace ? (!std::iswspace(ch) && trimChar != ch) : trimChar != ch;
			})
	);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimRight(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}).base(), s.end());
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::TrimRight(std::string& s, const unsigned char trimChar, bool trim_isspace)
{
    //Note: Can use [trimChar, trim_isspace] in the lambda, or just [=] to get access to
    //trimChar and trim_isspace.
	s.erase(std::find_if(s.rbegin(), s.rend(), [=](unsigned char ch)
		{
			return trim_isspace ? (!std::isspace(ch) && trimChar != ch) : trimChar != ch;
		}).base(), s.end());
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimRight(std::wstring& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch)
		{
			return !std::iswspace(ch);
		}).base(), s.end());
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::TrimRight(std::wstring& s, const wchar_t trimChar, bool trim_iswspace)
{
    //Note: Can use [trimChar, trim_iswspace] in the lambda, or just [=] to get access to
    //trimChar and trim_iswspace.
	s.erase(std::find_if(s.rbegin(), s.rend(), [=](wchar_t ch)
		{
			return trim_iswspace ? (!std::iswspace(ch) && trimChar != ch) : trimChar != ch;
		}).base(), s.end());
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
void TStrTool::Trim(std::string& s)
{
	TrimLeft(s);
	TrimRight(s);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::Trim(std::string& s, const unsigned char trimChar, bool trim_isspace)
{
	TrimLeft(s, trimChar, trim_isspace);
	TrimRight(s, trimChar, trim_isspace);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
void TStrTool::Trim(std::wstring& s)
{
	TrimLeft(s);
	TrimRight(s);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::Trim(std::wstring& s, const wchar_t trimChar, bool trim_iswspace)
{
	TrimLeft(s, trimChar, trim_iswspace);
	TrimRight(s, trimChar, trim_iswspace);
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::TrimLeft_Copy(std::string s)
{
	TrimLeft(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::TrimLeft_Copy(std::string s, const unsigned char trimChar, bool trim_isspace)
{
	TrimLeft(s, trimChar, trim_isspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::TrimLeft_Copy(std::wstring s)
{
	TrimLeft(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::TrimLeft_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace)
{
	TrimLeft(s, trimChar, trim_iswspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::TrimRight_Copy(std::string s)
{
	TrimRight(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::TrimRight_Copy(std::string s, const unsigned char trimChar, bool trim_isspace)
{
	TrimRight(s, trimChar, trim_isspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::TrimRight_Copy(std::wstring s)
{
	TrimRight(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::TrimRight_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace)
{
	TrimRight(s, trimChar, trim_iswspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::Trim_Copy(std::string s)
{
	Trim(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::Trim_Copy(std::string s, const unsigned char trimChar, bool trim_isspace)
{
	Trim(s, trimChar, trim_isspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::Trim_Copy(std::wstring s)
{
	Trim(s);
	return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::Trim_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace)
{
	Trim(s, trimChar, trim_iswspace);
	return s;
}
//---------------------------------------------------------------------------
// -Static
bool TStrTool::IsEmptyOrWhiteSpace(const std::string &s)
{
	if (s.length() == 0)
		return true;

	if (TStrTool::Trim_Copy(s).length() == 0)
		return true;

	return false;
}
//---------------------------------------------------------------------------
// -Static
bool TStrTool::IsEmptyOrWhiteSpace(const std::wstring &s)
{
	if (s.length() == 0)
		return true;

	if (TStrTool::Trim_Copy(s).length() == 0)
		return true;

	return false;
}
//---------------------------------------------------------------------------
// -'path' will be wrapped in double quotes, unless already starts with a double quote.
// -Caller is responsible for ensuring that 'args' is a proper command argument list, or blank.
std::string TStrTool::CombinePathAndArgs(const std::string& path, const std::string& args)
{
	std::string pathAndArgs;

	if (path[0] == '\"')
		pathAndArgs = path;
	else
		pathAndArgs = "\"" + path + "\"";

	if (args.length() > 0)
		pathAndArgs += " " + args;

	return pathAndArgs;
}
//---------------------------------------------------------------------------
// -'path' will be wrapped in double quotes, unless already starts with a double quote.
// -Caller is responsible for ensuring that 'args' is a proper command argument list, or blank.
std::wstring TStrTool::CombinePathAndArgs(const std::wstring& path, const std::wstring& args)
{
	std::wstring pathAndArgs;

	if (path[0] == L'\"')
		pathAndArgs = path;
	else
		pathAndArgs = L"\"" + path + L"\"";

	if (args.length() > 0)
		pathAndArgs += L" " + args;

	return pathAndArgs;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::string> TStrTool::Split(const std::string& str, const char sep)
{
	std::string::size_type pos = 0, prevPos = 0;
	std::vector<std::string> list;

	while((pos = str.find(sep, pos)) != std::string::npos)
	{
		list.push_back(str.substr(prevPos, pos - prevPos));
		prevPos = ++pos;
	}

    //get last item
	list.push_back(str.substr(prevPos, pos - prevPos));
    return list;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::wstring> TStrTool::Split(const std::wstring& str, const wchar_t sep)
{
	std::wstring::size_type pos = 0, prevPos = 0;
	std::vector<std::wstring> list;

	while((pos = str.find(sep, pos)) != std::wstring::npos)
	{
		list.push_back(str.substr(prevPos, pos - prevPos));
		prevPos = ++pos;
	}

	//get last item
	list.push_back(str.substr(prevPos, pos - prevPos));
	return list;
}
//---------------------------------------------------------------------------
// -Static
// -Inserts a delim character wherever there is a delim character found.
std::string TStrTool::DelimStr_Escape(const std::string& strIn, const char delim)
{
	if (strIn.length() == 0)
		return "";

	size_t outBuffSize = (strIn.length() * 2) + sizeof('\0');
	char* outBuff = new char[outBuffSize];
	const char* walkerIn = strIn.c_str();
	char* walkerOut = outBuff;

	//copy in string to out string while inserting a delim wherever a delim is found
	while (*walkerIn)
	{
		if (delim == *walkerIn)
			*walkerOut++ = delim;
		*walkerOut++ = *walkerIn++;
	}

	*walkerOut = '\0';
	std::string strOut = outBuff;
	delete[] outBuff;
	return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::DelimStr_Escape(const std::wstring& strIn, wchar_t delim)
{
	if (strIn.length() == 0)
		return L"";

	size_t outBuffSize = (strIn.length() * 2) + sizeof('\0');
	wchar_t* outBuff = new wchar_t[outBuffSize];
	const wchar_t* walkerIn = strIn.c_str();
	wchar_t* walkerOut = outBuff;

	//copy in string to out string while inserting a delim wherever a delim is found
	while (*walkerIn)
	{
		if (delim == *walkerIn)
			*walkerOut++ = delim;
		*walkerOut++ = *walkerIn++;
	}

	*walkerOut = L'\0';
	std::wstring strOut = outBuff;
	delete[] outBuff;
	return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::string TStrTool::DelimStr_UnEscape(const std::string& strIn, char delim)
{
	if (strIn.length() == 0)
		return "";

	size_t outBuffSize = strIn.length() + sizeof('\0');
	char* outBuff = new char[outBuffSize];
	const char* walkerIn = strIn.c_str();
	char* walkerOut = outBuff;

	//copy in string to out string while inserting a delim wherever a delim is found
	while (*walkerIn)
	{
		if (delim == *walkerIn && delim == *(walkerIn + 1))
			walkerIn++; //skip a delim
		*walkerOut++ = *walkerIn++;
	}

	*walkerOut = '\0';
	std::string strOut = outBuff;
	delete[] outBuff;
	return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::DelimStr_UnEscape(const std::wstring& strIn, wchar_t delim)
{
	if (strIn.length() == 0)
		return L"";

	size_t outBuffSize = strIn.length() + sizeof('\0');
	wchar_t* outBuff = new wchar_t[outBuffSize];
	const wchar_t* walkerIn = strIn.c_str();
	wchar_t* walkerOut = outBuff;

	//copy in string to out string while inserting a delim wherever a delim is found
	while (*walkerIn)
	{
		if (delim == *walkerIn && delim == *(walkerIn + 1))
			walkerIn++; //skip a delim
		*walkerOut++ = *walkerIn++;
	}

	*walkerOut = L'\0';
	std::wstring strOut = outBuff;
	delete[] outBuff;
	return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::string> TStrTool::CombineLists_Unique(const std::vector<std::string>& list1,
	const std::vector<std::string>& list2, bool ignoreBlank, bool ignoreCase)
{
	std::vector<std::string> dest;

	for (int i = 0; i < list1.size() || i < list2.size(); i++)
	{
		const char* val1P = i < list1.size() ? list1[i].c_str() : "";
		const char* val2P = i < list2.size() ? list2[i].c_str() : "";
		bool addVal1 = (!ignoreBlank || *val1P != '\0') && i < list1.size();
		bool addVal2 = (!ignoreBlank || *val2P != '\0') && i < list2.size();

		if (!addVal1 && !addVal2)
			continue;

		//if val1 and val2 are still eligible, and if val1 and val2 are the same, don't bother with val2
		if (addVal1 && addVal2)
		{
			if ((ignoreCase && strcmpI(val1P, val2P) == 0) || (!ignoreCase && strcmp(val1P, val2P) == 0))
				addVal2 = false;
		}

		//check new list for existing items
		for (int j = 0; j < dest.size(); j++)
		{
			const char* destP = dest[j].c_str();

			if (addVal1)
			{
				if ((ignoreCase && strcmpI(destP, val1P) == 0) || (!ignoreCase && strcmp(destP, val1P) == 0))
					addVal1 = false;
			}

			if (addVal2)
			{
				if ((ignoreCase && strcmpI(destP, val2P) == 0) || (!ignoreCase && strcmp(destP, val2P) == 0))
					addVal2 = false;
			}

            if (!addVal1 && !addVal2)
				break; //stop searching if it has been determined to not add either one
		}

		if (addVal1)
			dest.push_back(val1P);

		if (addVal2)
			dest.push_back(val2P);
	}

    return dest;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::wstring> TStrTool::CombineLists_Unique(const std::vector<std::wstring>& list1,
	const std::vector<std::wstring>& list2, bool ignoreBlank, bool ignoreCase)
{
	std::vector<std::wstring> dest;

	for (int i = 0; i < list1.size() || i < list2.size(); i++)
	{
		const wchar_t* val1P = i < list1.size() ? list1[i].c_str() : L"";
		const wchar_t* val2P = i < list2.size() ? list2[i].c_str() : L"";
		bool addVal1 = (!ignoreBlank || *val1P != L'\0') && i < list1.size();
		bool addVal2 = (!ignoreBlank || *val2P != L'\0') && i < list2.size();

		if (!addVal1 && !addVal2)
			continue;

		//if val1 and val2 are still eligible, and if val1 and val2 are the same, don't bother with val2
		if (addVal1 && addVal2)
		{
			if ((ignoreCase && _wcsicmp(val1P, val2P) == 0) || (!ignoreCase && wcscmp(val1P, val2P) == 0))
				addVal2 = false;
		}

		//check new list for existing items
		for (int j = 0; j < dest.size(); j++)
		{
			const wchar_t* destP = dest[j].c_str();

			if (addVal1)
			{
				if ((ignoreCase && _wcsicmp(destP, val1P) == 0) || (!ignoreCase && wcscmp(destP, val1P) == 0))
					addVal1 = false;
			}

			if (addVal2)
			{
				if ((ignoreCase && _wcsicmp(destP, val2P) == 0) || (!ignoreCase && wcscmp(destP, val2P) == 0))
					addVal2 = false;
			}

            if (!addVal1 && !addVal2)
				break; //stop searching if it has been determined to not add either one (i.e. they already exist in dest)
		}

		if (addVal1)
			dest.push_back(val1P);

		if (addVal2)
			dest.push_back(val2P);
	}

    return dest;
}
//---------------------------------------------------------------------------
// -Static
// -Splits the host and path portion from a URL. If URL has not path (just a host) then
//  pathUtf8 will be set to "/".
bool TStrTool::URL_Split(const std::string& url, std::string* hostUtf8, std::string* pathUtf8)
{
	const std::string httpPrefix = "http://";
	const std::string httpsPrefix = "https://";

	if (nullptr != hostUtf8)
		*hostUtf8 = "";

	if (nullptr != pathUtf8)
		*pathUtf8 = "";

	if (url.length() == 0)
		return false;

	const char* urlStart = url.c_str();
	const char* walker = urlStart;

	//skip http/s prefix, if there is one
#ifdef _MSC_VER
	if (_strnicmp(walker, httpPrefix.c_str(), httpPrefix.length()) == 0)
		walker += httpPrefix.length();
	else if (_strnicmp(walker, httpsPrefix.c_str(), httpsPrefix.length()) == 0)
		walker += httpsPrefix.length();
#else
	if (strncmpi(walker, httpPrefix.c_str(), httpPrefix.length()) == 0)
		walker += httpPrefix.length();
	else if (strncmpi(walker, httpsPrefix.c_str(), httpsPrefix.length()) == 0)
		walker += httpsPrefix.length();
#endif

	//find "/", which marks the end of the host part
	while (*walker && *walker != '/' && *walker != '\\')
		walker++;

	if (!*walker) //no slash was found - at end of string
	{
		//there is only a host portion, no path, so set path to just a slash
		if (nullptr != hostUtf8)
			*hostUtf8 = url;

		if (nullptr != pathUtf8)
			*pathUtf8 = "/";
		return true;
	}

	size_t slashIdx = walker - urlStart;

	//slash was found - set host and path
	if (nullptr != hostUtf8)
		*hostUtf8 = url.substr(0, slashIdx);

	if (nullptr != pathUtf8)
		*pathUtf8 = url.substr(slashIdx, std::string::npos);
	return true;
}
//---------------------------------------------------------------------------
// -Static
// -Converts a an uppercase or lowercase hex character to a byte.
// -Caller must join the low and high of a hex pair with a bit shift. ( "b = (high << 4) | low;" )
bool TStrTool::HexSingleToByte(const char hexSingle, BYTE* out)
{
	if (nullptr == out)
		return false;

	if (hexSingle >= '0' && hexSingle <= '9')
		*out = hexSingle - '0';
	else if (hexSingle >= 'A' && hexSingle <= 'F')
		*out = hexSingle - 'A' + 10;
	else if (hexSingle >= 'a' && hexSingle <= 'f')
		*out = hexSingle - 'a' + 10;
	else
		return false; //invalid hex character

	return true;
}
//---------------------------------------------------------------------------
// -Static
// -Converts utf8 string to url encoded escaped "%" values
std::string TStrTool::URL_EncodeUtf8(const std::string& valueUtf8, bool useUpperCaseHex)
{
	if (valueUtf8.length() == 0)
		return "";

	const char* hexP = useUpperCaseHex ? HEX_UPPER : HEX_LOWER;

	const size_t maxEncodedCharWidth = 3; //e.g. "%FF"
	const size_t buffSize = (valueUtf8.length() * maxEncodedCharWidth) + sizeof('\0');
	char* buff = new char[buffSize];
	char* buffP = buff;
	const char* walker = valueUtf8.c_str();

	while (*walker)
	{
		if (isalnum(*walker) ||
			*walker == '.'  || *walker == '-' || *walker == '_' || *walker == '~')
		{
			*buffP++ = *walker++;
		}
		else
		{
			*buffP++ = URL_HEX_ESCAPE;
			//convert char to byte so that utf8 works correctly, then convert to hex
			BYTE byteVal = *walker++;
			*buffP++ = hexP[((byteVal >> 4) & 0xF)];//left
			*buffP++ = hexP[(byteVal) & 0x0F];//right
		}
	}

	//terminate, copy, cleanup and return
	*buffP = '\0';
	std::string encStr = buff;
	delete[] buff;
	return encStr;
}
//---------------------------------------------------------------------------
// -Static
// -Decodes url encoded/escaped string back to a utf8 string.
std::string TStrTool::URL_DecodeUtf8(const std::string& encodedStr, bool* invalidHexEncountered)
{
	if (encodedStr.length() == 0)
		return "";

	if (nullptr != invalidHexEncountered)
		*invalidHexEncountered = false;

	const size_t buffSize = encodedStr.length() + sizeof('\0');
	char* buff = new char[buffSize];
	char* buffP = buff;
	const char* walker = encodedStr.c_str();

	while (*walker)
	{
		if (*walker == URL_HEX_ESCAPE)
		{
			walker++;

			BYTE high, low;

			//convert hex pair to high and low parts
			if (!HexSingleToByte(*walker, &high) || !HexSingleToByte(*(walker + 1), &low))
			{
				//be tolerant of invalid encoded URL strings - preserve and move on
				*buffP++ = URL_HEX_ESCAPE;
				if (nullptr != invalidHexEncountered)
					*invalidHexEncountered = true;
				continue;
			}

			//hex singles are valid - combine into a byte
			*buffP++ = (high << 4) | low;
			walker += 2;
		}
		else if (*walker == '+')
		{
			*buffP++ = ' ';
			walker++;
		}
		else
		{
			*buffP++ = *walker++;
		}
	}

	//terminate, copy, cleanup and return
	*buffP = '\0';
	std::string valUtf8Str = buff;
	delete[] buff;
	return valUtf8Str;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

} //end namespace

