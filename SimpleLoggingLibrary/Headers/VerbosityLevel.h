#pragma once

#include <string>
#include <vector>

namespace SLL
{
    // Various message-type categories.
    enum class VerbosityLevel : size_t
    {
        // MIN CAP
        BEGIN = 0,

        // Standard log message.
        INFO = 0,

        // Noteworthy, yet expected behavior.
        WARN,

        // Erroneous behavior.
        ERROR,

        // Erroneous behavior leading to unrecoverable state. 
        FATAL,

        // MAX CAP
        MAX
    };

    using VerbosityLevelType = std::underlying_type_t<VerbosityLevel>;

    template <class T, std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, wchar_t>::value>>
    const std::basic_string<T>& GetVerbosityLevelString(const size_t);

    template <char>
    const std::basic_string<char>& GetVerbosityLevelString(const size_t i)
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
            throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(VerbosityLevelStringsA.size( ) - 1));
        }

        return VerbosityLevelStringsA[i];
    }

    template <wchar_t>
    const std::basic_string<wchar_t>& GetVerbosityLevelString(const size_t i)
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
            throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(VerbosityLevelStringsW.size( ) - 1));
        }

        return VerbosityLevelStringsW[i];
    }

    template <class T, std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, wchar_t>::value>>
    const std::basic_string<T>& VerbosityLevelToString(VerbosityLevel lvl)
    {
        return GetVerbosityLevelString<T>(static_cast<VerbosityLevelType>(lvl));
    }
}
