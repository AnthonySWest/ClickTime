//---------------------------------------------------------------------------
// Version.h
// Author: Anthony West - ASW Software
//
// Contains routines for working with module versions.
//
// Every attempt should be made to keep this module at least Windows portable.
//---------------------------------------------------------------------------

#ifndef VersionH
#define VersionH
//---------------------------------------------------------------------------
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TVersion
//
// For version comparison checking.
/////////////////////////////////////////////////////////////////////////////
class TVersion
{
private:
    typedef TVersion ThisType;

public:
    TVersion();
    TVersion(TVersion const& version);
    TVersion(unsigned long long valueU64);
    TVersion(std::string const& verStr);
    TVersion(std::wstring const& verStr);
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
    bool ExtractVersionNumbersFromVersionStr(std::string const& verStr);
    bool ExtractVersionNumbersFromVersionStr(std::wstring const& verStr);
    static bool ExtractVersionNumbersFromVersionStr(std::string const& verStr, int* major,
        int* minor, int* build, int* revision);
    static bool ExtractVersionNumbersFromVersionStr(std::wstring const& verStr, int* major,
        int* minor, int* build, int* revision);
    std::string ToStrVer() const;
    std::wstring ToStrVerW() const;

    // operator related functions
    bool CopyFrom(ThisType const& otherClass);
    bool SameAs(ThisType const& otherClass) const;
    ThisType& operator=(ThisType const& right);
    bool operator==(ThisType const& right) const;
    bool operator!=(ThisType const& right) const;
    bool operator<(ThisType const& right) const;
    bool operator<=(ThisType const& right) const;
    bool operator>(ThisType const& right) const;
    bool operator>=(ThisType const& right) const;
};

} // namespace Subroutines

//---------------------------------------------------------------------------
#endif // #ifndef VersionH
