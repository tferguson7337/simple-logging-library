#include <VerbosityLevel.h>

#include <vector>

/// Private Helper Methods \\\

// VerbosityLevel String Getter - char
template <>
const std::basic_string<char>& SLL::VerbosityLevelConverter::GetVerbosityLevelString<char>(const size_t i)
{
    static const std::vector<std::basic_string<char>> VerbosityLevelStringsA
    {
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };

    if ( i >= VerbosityLevelStringsA.size( ) )
    {
        throw std::range_error(
            __FUNCTION__" - Index argument out of bounds.  Index = " + 
            std::to_string(i) + 
            ", Max = " + 
            std::to_string(VerbosityLevelStringsA.size( ) - 1)
        );
    }

    return VerbosityLevelStringsA[i];
}

// VerbosityLevel String Getter - wchar_t
template <>
const std::basic_string<wchar_t>& SLL::VerbosityLevelConverter::GetVerbosityLevelString<wchar_t>(const size_t i)
{
    static const std::vector<std::basic_string<wchar_t>> VerbosityLevelStringsW
    {
        L"INFO",
        L"WARN",
        L"ERROR",
        L"FATAL"
    };

    if ( i >= VerbosityLevelStringsW.size( ) )
    {
        throw std::range_error(
            __FUNCTION__" - Index argument out of bounds.  Index = " + 
            std::to_string(i) + 
            ", Max = " + 
            std::to_string(VerbosityLevelStringsW.size( ) - 1)
        );
    }

    return VerbosityLevelStringsW[i];
}


/// Public Methods \\\

// Returns underlying scalar type value of VerbosityLevel argument.
SLL::VerbosityLevelType SLL::VerbosityLevelConverter::ToScalar(VerbosityLevel lvl)
{
    if ( lvl < VerbosityLevel::BEGIN || lvl > VerbosityLevel::MAX )
    {
        throw std::invalid_argument(
            __FUNCTION__" - Invalid verbosity level argument (" + 
            std::to_string(static_cast<VerbosityLevelType>(lvl)) + 
            ")."
        );
    }

    return static_cast<VerbosityLevelType>(lvl);
}

// Returns string name of VerbosityLevel argument.
template <class T, typename>
const std::basic_string<T>& SLL::VerbosityLevelConverter::ToString(VerbosityLevel lvl)
{
    if ( lvl >= VerbosityLevel::MAX )
    {
        throw std::invalid_argument(
            __FUNCTION__" - Invalid verbosity level argument (" + 
            std::to_string(static_cast<VerbosityLevelType>(lvl)) + 
            ")."
        );
    }

    return GetVerbosityLevelString<T>(ToScalar(lvl));
}


/// Explicit Template Instantiation \\\

template const std::basic_string<char>& SLL::VerbosityLevelConverter::ToString<char>(VerbosityLevel);
template const std::basic_string<wchar_t>& SLL::VerbosityLevelConverter::ToString<wchar_t>(VerbosityLevel);