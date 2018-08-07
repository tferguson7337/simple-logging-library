#include <OptionFlag.h>

#include <vector>

namespace SLL
{
    /// OptionFlag Enum Class Operator Overloads \\\

    // Bitwise NOT
    OptionFlag operator~(const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(~static_cast<OptionFlagType>(rhs) & (static_cast<OptionFlagType>(OptionFlag::MAX) - 1));
    }

    // Bitwise OR
    OptionFlag operator|(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) | static_cast<OptionFlagType>(rhs));
    }

    // Bitwise AND
    OptionFlag operator&(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) & static_cast<OptionFlagType>(rhs));
    }

    // Bitwise XOR
    OptionFlag operator^(const OptionFlag lhs, const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) ^ static_cast<OptionFlagType>(rhs));
    }


    // Bitwise OR - Assignment
    OptionFlag operator|=(OptionFlag& lhs, const OptionFlag rhs)
    {
        return lhs = lhs | rhs;
    }

    // Bitwise AND - Assignment
    OptionFlag operator&=(OptionFlag& lhs, const OptionFlag rhs)
    {
        return lhs = lhs & rhs;
    }

    // Bitwise XOR - Assignment
    OptionFlag operator^=(OptionFlag& lhs, const OptionFlag rhs)
    {
        return lhs = lhs ^ rhs;
    }


    // Left Shift
    OptionFlag operator<<(const OptionFlag lhs, const OptionFlagType s)
    {
        OptionFlagType val = static_cast<OptionFlagType>(lhs) << s;
        return (val > static_cast<OptionFlagType>(OptionFlag::MAX)) ? OptionFlag::MAX : static_cast<OptionFlag>(val);
    }

    // Right Shift
    OptionFlag operator>>(const OptionFlag lhs, const OptionFlagType s)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) >> s);
    }

    // Left Shift - Assignment
    OptionFlag operator<<=(OptionFlag& lhs, const OptionFlagType s)
    {
        return lhs = lhs << s;
    }

    // Right Shift - Assignment
    OptionFlag operator>>=(OptionFlag& lhs, const OptionFlagType s)
    {
        return lhs = lhs >> s;
    }

    /// OptionFlagConverter \\\

    /// Private Helper Methods \\\

    // OptionFlag String Getter - char
    template <>
    const std::basic_string<char>& OptionFlagConverter::GetOptionFlagString<char>(const size_t i)
    {
        static const std::vector<std::basic_string<char>> optionFlagStringsA
        {
            "LogToStdout",
            "LogToFile",
            "LogInColor",
            "LogSynchronous",
            "LogTimestamp",
            "LogThreadID",
            "LogVerbosityLevel"
        };

        if ( i >= optionFlagStringsA.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(optionFlagStringsA.size( ) - 1)
            );
        }

        return optionFlagStringsA[i];
    }

    // OptionFlag String Getter - wchar_t
    template <>
    const std::basic_string<wchar_t>& OptionFlagConverter::GetOptionFlagString<wchar_t>(const size_t i)
    {
        static const std::vector<std::basic_string<wchar_t>> optionFlagStringsW
        {
            L"LogToStdout",
            L"LogToFile",
            L"LogInColor",
            L"LogSynchronous",
            L"LogTimestamp",
            L"LogThreadID",
            L"LogVerbosityLevel"
        };

        if ( i >= optionFlagStringsW.size( ) )
        {
            throw std::range_error(
                __FUNCTION__" - Index argument out of bounds.  Index = " +
                std::to_string(i) +
                ", Max = " +
                std::to_string(optionFlagStringsW.size( ) - 1)
            );
        }

        return optionFlagStringsW[i];
    }

    // OptionFlag::NONE String Getter - char
    template <>
    const std::basic_string<char>& OptionFlagConverter::GetEmptyMaskString<char>( )
    {
        static const std::basic_string<char> emptyMaskA("NONE");
        return emptyMaskA;
    }

    // OptionFlag::NONE String Getter - wchar_t
    template <>
    const std::basic_string<wchar_t>& OptionFlagConverter::GetEmptyMaskString<wchar_t>( )
    {
        static const std::basic_string<wchar_t> emptyMaskW(L"NONE");
        return emptyMaskW;
    }

    // Separator Getter - char
    template <>
    const std::basic_string<char>& OptionFlagConverter::GetSeparator<char>( )
    {
        static const std::basic_string<char> separatorA(", ");
        return separatorA;
    }

    // Separator Getter - wchar_t
    template <>
    const std::basic_string<wchar_t>& OptionFlagConverter::GetSeparator<wchar_t>( )
    {
        static const std::basic_string<wchar_t> separatorW(L", ");
        return separatorW;
    }


    /// Public Methods \\\

    // Returns underlying scalar type value of OptionFlag mask argument.
    OptionFlagType OptionFlagConverter::ToScalar(OptionFlag f)
    {
        if ( f < OptionFlag::NONE || f > OptionFlag::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid flag argument (" +
                std::to_string(static_cast<OptionFlagType>(f)) +
                ")."
            );
        }

        return static_cast<OptionFlagType>(f);
    }

    // Returns string names of OptionFlag mask argument.
    template <class T, typename>
    std::basic_string<T> OptionFlagConverter::ToString(OptionFlag flag)
    {
        std::basic_string<T> str;
        size_t i = 0;

        // Ensure flag argument is valid.
        if ( flag >= OptionFlag::MAX )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Invalid flag argument (" +
                std::to_string(ToScalar(flag)) +
                ")."
            );
        }

        // No flag is set - return "NONE"
        if ( flag == OptionFlag::NONE )
        {
            return GetEmptyMaskString<T>( );
        }

        // Iterate thru OptionFlag mask.
        for ( OptionFlag f = OptionFlag::BEGIN; f < OptionFlag::MAX && flag != OptionFlag::NONE; f <<= 1, i++ )
        {
            if ( (f & flag) == f )
            {
                str.append(GetOptionFlagString<T>(i) + GetSeparator<T>( ));
                flag &= ~f;
            }
        }

        // The string should not be empty after the loop.
        if ( str.empty( ) )
        {
            throw std::runtime_error(
                __FUNCTION__" - No matches were found.  Flag = (" +
                std::to_string(ToScalar(flag)) +
                ")."
            );
        }

        // Flag mask should be cleared after the loop.
        if ( flag != OptionFlag::NONE )
        {
            throw std::invalid_argument(
                __FUNCTION__" - Not all flag values were matched. Remaining flags = (" +
                std::to_string(ToScalar(flag)) +
                ")."
            );
        }

        // Trim the extra space and comma.
        str.pop_back( );
        str.pop_back( );

        return str;
    }


    /// Explicit Template Instantiation \\\

    template std::basic_string<char> OptionFlagConverter::ToString<char>(OptionFlag);
    template std::basic_string<wchar_t> OptionFlagConverter::ToString<wchar_t>(OptionFlag);
}
