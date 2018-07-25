#pragma once

#include <string>
#include <vector>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class OptionFlag : size_t
    {
        // NO SETTINGS
        NONE                = 0,

        // MIN CAP
        BEGIN               = 1 << 0,

        LogToStdout         = 1 << 0,
        LogToFile           = 1 << 1,
        LogInColor          = 1 << 2,
        LogVerbosityLevel   = 1 << 3,
        LogTimestamp        = 1 << 4,
        LogThreadID         = 1 << 5,
        LogSynchronous      = 1 << 6,

        // MAX CAP
        MAX                 = 1 << 7
    };

    /// BITWISE OPERATOR OVERLOADS \\\

    using OptionFlagType = std::underlying_type_t<OptionFlag>;

    // Bitwise NOT
    inline OptionFlag operator~(const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(~static_cast<OptionFlagType>(rhs));
    }

    // Bitwise OR
    inline OptionFlag operator|(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) | static_cast<OptionFlagType>(rhs));
    }

    // Bitwise AND
    inline OptionFlag operator&(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) & static_cast<OptionFlagType>(rhs));
    }

    // Bitwise XOR
    inline OptionFlag operator^(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) ^ static_cast<OptionFlagType>(rhs));
    }


    // Bitwise OR - Assignment
    inline OptionFlag operator|=(OptionFlag& lhs, const OptionFlag rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    // Bitwise AND - Assignment
    inline OptionFlag operator&=(OptionFlag& lhs, const OptionFlag rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    // Bitwise XOR - Assignment
    inline OptionFlag operator^=(OptionFlag& lhs, const OptionFlag rhs)
    {
        lhs = lhs ^ rhs;
        return lhs;
    }


    // Left Shift
    inline OptionFlag operator<<(const OptionFlag lhs, const OptionFlagType s)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) << s);
    }

    // Right Shift
    inline OptionFlag operator>>(const OptionFlag lhs, const OptionFlagType s)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) >> s);
    }

    // Left Shift - Assignment
    inline OptionFlag operator<<=(OptionFlag& lhs, const OptionFlagType s)
    {
        lhs = lhs << s;
        return lhs;
    }

    // Right Shift - Assignment
    inline OptionFlag operator>>=(OptionFlag& lhs, const OptionFlagType s)
    {
        lhs = lhs >> s;
        return lhs;
    }


    template <class T, std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, wchar_t>::value>>
    const std::basic_string<T>& GetOptionFlagString(const size_t);

    template <char>
    const std::basic_string<char>& GetOptionFlagString(const size_t i)
    {
        static const std::vector<std::basic_string<char>> OptionFlagStringsA
        {
            "LOG_TO_STDOUT",
            "LOG_TO_FILE",
            "LOG_IN_COLOR",
            "LOG_VERBOSITY_LEVEL",
            "LOG_TIMESTAMP",
            "LOG_THREAD_ID",
            "LOG_SYNCHRONOUS"
        };
        
        if ( i >= OptionFlagStringsA.size( ) )
        {
            throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(OptionFlagStringsA.size( ) - 1));
        }

        return OptionFlagStringsA[i];
    }

    template <wchar_t>
    const std::basic_string<wchar_t>& GetOptionFlagString(const size_t i)
    {
        static const std::vector<std::basic_string<wchar_t>> OptionFlagStringsW
        {
            L"LOG_TO_STDOUT",
            L"LOG_TO_FILE",
            L"LOG_IN_COLOR",
            L"LOG_VERBOSITY_LEVEL",
            L"LOG_TIMESTAMP",
            L"LOG_THREAD_ID",
            L"LOG_SYNCHRONOUS"
        };

        if ( i >= OptionFlagStringsW.size( ) )
        {
            throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(OptionFlagStringsW.size( ) - 1));
        }

        return OptionFlagStringsW[i];
    }

    template <class T, std::enable_if_t<std::is_same<T, char>::value || std::is_same<T, wchar_t>::value>>
    const std::basic_string<T>& OptionFlagToString(OptionFlag flag)
    {
        std::basic_string<T> str;
        size_t i = 0;

        // No flag is set - return "NONE"
        if ( flag == OptionFlag::NONE )
        {
            str.assign("NONE");
            return str;
        }

        // Iterate thru OptionFlag mask.
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX && flag != OptionFlag::NONE; f <<= 1, i++ )
        {
            if ( f & flag )
            {
                str.append(GetOptionFlagString<T>(i) + ", ");
                flag &= ~f;
            }
        }

        // Flag should be cleared after the loop.
        if ( flag )
        {
            throw std::invalid_argument(__FUNCTION__" - flag is non-empty after iterating through values.  Flag = (" + std::to_string(static_cast<OptionFlagType>(flag)) + ").");
        }

        // The string should not be empty after the loop.
        if ( str.empty( ) )
        {
            throw std::runtime_error(__FUNCTION__" - flag is non-empty, but no matches were found.  Flag = (" + std::to_string(static_cast<OptionFlagType>(flag)) + ").");
        }

        // Trim the extra space and comma.
        str.push_back( );
        str.push_back( );

        return str;
    }
}