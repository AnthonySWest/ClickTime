//---------------------------------------------------------------------------


#include "Version.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

namespace Subroutines
{

// /////////////////// class statics ////////////////////////////////////////


//---------------------------------------------------------------------------
TVersion::TVersion()
{
	Reset();
}
//---------------------------------------------------------------------------
TVersion::TVersion(const TVersion& version)
{
	Reset();
	ValueU64 = version.ValueU64;
}
//---------------------------------------------------------------------------
TVersion::TVersion(unsigned long long valueU64)
{
	Reset();
	ValueU64 = valueU64;
}
//---------------------------------------------------------------------------
TVersion::TVersion(const std::string& verStr)
{
	Reset();
	ExtractVersionNumbersFromVersionStr(verStr);
}
//---------------------------------------------------------------------------
TVersion::TVersion(const std::wstring& verStr)
{
	Reset();
	ExtractVersionNumbersFromVersionStr(verStr);
}
//---------------------------------------------------------------------------
TVersion::~TVersion()
{

}
//---------------------------------------------------------------------------
void TVersion::Reset()
{
	ValueU64 = 0;
}
//---------------------------------------------------------------------------
void TVersion::SetVersion(WORD major, WORD minor, WORD build, WORD revision)
{
	Major = major;
	Minor = minor;
	Build = build;
	Revision = revision;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(const std::string& verStr)
{
	int major, minor, build, revision;

	ValueU64 = 0;

	if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
	{
		Major = (WORD)major;
		Minor = (WORD)minor;
		Build = (WORD)build;
		Revision = (WORD)revision;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(const std::wstring& verStr)
{
	int major, minor, build, revision;

	ValueU64 = 0;

	if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
	{
		Major = (WORD)major;
		Minor = (WORD)minor;
		Build = (WORD)build;
		Revision = (WORD)revision;
		return true;
	}
    return false;
}
//---------------------------------------------------------------------------
//  -Static
bool TVersion::ExtractVersionNumbersFromVersionStr(const std::string& verStr, int *major,
			int *minor, int *build, int *revision)
{
	*major = *minor = *build = *revision = 0;

	if (verStr.length() == 0)
		return false;

	const char *walker = verStr.c_str();

	//get major version number
	*major = atoi(walker);

	//get minor version number
	while (*walker && *walker != '.')
		walker++;

	if (*walker)
		*minor = atoi(++walker);

	//get build number
	while (*walker && *walker != '.')
		walker++;

	if (*walker)
		*build = atoi(++walker);

	//get revision number
	while (*walker && *walker != '.')
		walker++;

	if (*walker)
		*revision = atoi(++walker);

	return true;
}
//---------------------------------------------------------------------------
//  -Static
bool TVersion::ExtractVersionNumbersFromVersionStr(const std::wstring& verStr, int *major,
			int *minor, int *build, int *revision)
{
	*major = *minor = *build = *revision = 0;

	if (verStr.length() == 0)
		return false;

	const wchar_t *walker = verStr.c_str();

	//get major version number
	*major = _wtoi(walker);

	//get minor version number
	while (*walker && *walker != L'.')
		walker++;

	if (*walker)
		*minor = _wtoi(++walker);

	//get build number
	while (*walker && *walker != L'.')
		walker++;

	if (*walker)
		*build = _wtoi(++walker);

	//get revision number
	while (*walker && *walker != L'.')
		walker++;

	if (*walker)
		*revision = _wtoi(++walker);

	return true;
}
//---------------------------------------------------------------------------
std::string TVersion::ToStrVer() const
{
	return std::to_string(Major) + "." + std::to_string(Minor) + "." +
		std::to_string(Build) + "." + std::to_string(Revision);
}
//---------------------------------------------------------------------------
std::wstring TVersion::ToStrVerW() const
{
	return std::to_wstring(Major) + L"." + std::to_wstring(Minor) + L"." +
		std::to_wstring(Build) + L"." + std::to_wstring(Revision);
}

//---------------------------------------------------------------------------
// -Returns true for success
bool TVersion::CopyFrom(const ThisType& otherClass)
{
	if (this == &otherClass)
		return true; //done due to self-assignment

	Reset();

	//try
	//{
		ValueU64 = otherClass.ValueU64;
	//}
	//catch (...)
	//{
	//	ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: Exception.\n", __FUNCTIONW__);
	//	return false;
	//}

	return true;
}
//---------------------------------------------------------------------------
bool TVersion::SameAs(const ThisType& otherClass) const
{
	if (this == &otherClass)
		return true; //self-compare

	if (ValueU64 != otherClass.ValueU64)
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
TVersion& TVersion::operator=(const ThisType& right)
{
	//avoid a self assignment
	if (this != &right)
		this->CopyFrom(right);
	return *this;
}
//---------------------------------------------------------------------------
bool TVersion::operator==(const ThisType& right) const
{
	if (this == &right)
		return true; //self-compare
	return this->SameAs(right);
}
//---------------------------------------------------------------------------
bool TVersion::operator!=(const ThisType& right) const
{
	return !(*this == right);
}
//---------------------------------------------------------------------------
bool TVersion::operator<(const ThisType& right) const
{
	if (this == &right)
		return false; //self-compare
	return ValueU64 < right.ValueU64;
}
//---------------------------------------------------------------------------
bool TVersion::operator<=(const ThisType& right) const
{
	if (this == &right)
		return true; //self-compare
	return ValueU64 <= right.ValueU64;
}
//---------------------------------------------------------------------------
bool TVersion::operator>(const ThisType& right) const
{
	return !(*this <= right);
}
//---------------------------------------------------------------------------
bool TVersion::operator>=(const ThisType& right) const
{
	return !(*this < right);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

} //end namespace
