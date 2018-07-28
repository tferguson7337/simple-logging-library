#pragma once

#include <string>
#include <vector>

namespace SLL
{
    // Various message-type categories.
    enum class VerbosityLevel : size_t
    {
        // Standard log message.
        INFO = 0,

        // Noteworthy, potentially erroneous behavior.
        WARN,

        // Erroneous behavior.
        ERROR,

        // Erroneous behavior leading to unrecoverable state. 
        FATAL,

        // MAX CAP
        MAX,

        // MIN CAP
        BEGIN = 0
    };

    using VerbosityLevelType = std::underlying_type_t<VerbosityLevel>;

    class VerbosityLevelConverter
    {
        /// Static Class - No Ctors, Dtor, or Assignment Allowed
        VerbosityLevelConverter( ) = delete;
        VerbosityLevelConverter(const VerbosityLevelConverter&) = delete;
        VerbosityLevelConverter(VerbosityLevelConverter&&) = delete;

        ~VerbosityLevelConverter( ) = delete;

        VerbosityLevelConverter& operator=(const VerbosityLevelConverter&) = delete;
        VerbosityLevelConverter& operator=(VerbosityLevelConverter&&) = delete;

    private:

        /// Private Helper Methods \\\

        // VerbosityLevel String Getters
        template <class T, std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        const std::basic_string<T>& GetVerbosityLevelString(const size_t);

        // VerbosityLevel String Getter - char
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

        // VerbosityLevel String Getter - wchar_t
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

    public:

        VerbosityLevelType ToScalar(VerbosityLevel lvl)
        {
            if ( lvl >= VerbosityLevel::MAX )
            {
                throw std::invalid_argument(__FUNCTION__" - Invalid verbosity level argument (" + std::to_string(static_cast<VerbosityLevelType>(lvl)) + ").");
            }

            return static_cast<VerbosityLevelType>(lvl);
        }

        template <class T, std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        const std::basic_string<T>& VerbosityLevelToString(VerbosityLevel lvl)
        {
            return GetVerbosityLevelString<T>(ToScalar(lvl));
        }
    };

    

    
}
