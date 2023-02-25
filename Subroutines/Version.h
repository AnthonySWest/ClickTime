//---------------------------------------------------------------------------

#ifndef VersionH
#define VersionH

#include <windows.h>
#include <string>

//---------------------------------------------------------------------------

namespace Subroutines
{

//for version comparison checking
class TVersion
{
typedef TVersion ThisType;

public:
	TVersion();
	TVersion(const TVersion& version);
	TVersion(unsigned long long valueU64);
	TVersion(const std::string& verStr);
	TVersion(const std::wstring& verStr);
	~TVersion();

    //share memory between version elements for easy assignment working with windows version stuff
	union
	{
		unsigned long long ValueU64;
		struct
		{
			WORD Revision;
			WORD Build;
			WORD Minor;
			WORD Major;
		};
	};

	void Reset();
	void SetVersion(WORD major, WORD minor, WORD build, WORD revision);
	bool ExtractVersionNumbersFromVersionStr(const std::string& verStr);
    bool ExtractVersionNumbersFromVersionStr(const std::wstring& verStr);
	static bool ExtractVersionNumbersFromVersionStr(const std::string& verStr, int *major,
			int *minor, int *build, int *revision);
    static bool ExtractVersionNumbersFromVersionStr(const std::wstring& verStr, int *major,
			int *minor, int *build, int *revision);
	std::string ToStrVer() const;
    std::wstring ToStrVerW() const;

	//operator related functions
	bool CopyFrom(const ThisType& otherClass);
	bool SameAs(const ThisType& otherClass) const;
	ThisType& operator=(const ThisType& right);
	bool operator==(const ThisType& right) const;
	bool operator!=(const ThisType& right) const;
	bool operator<(const ThisType& right) const;
	bool operator<=(const ThisType& right) const;
	bool operator>(const ThisType& right) const;
	bool operator>=(const ThisType& right) const;
};


} //end namepsace
#endif
