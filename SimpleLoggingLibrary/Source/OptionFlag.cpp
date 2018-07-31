#include <OptionFlag.h>

#include <vector>

/// OptionFlag Enum Class Operator Overloads \\\

// Bitwise NOT
SLL::OptionFlag SLL::operator~(const OptionFlag rhs)
{
    return static_cast<OptionFlag>(~static_cast<OptionFlagType>(rhs) & (static_cast<OptionFlagType>(OptionFlag::MAX) - 1));
}

// Bitwise OR
SLL::OptionFlag SLL::operator|(const OptionFlag lhs, const OptionFlag rhs)
{
    return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) | static_cast<OptionFlagType>(rhs));
}

// Bitwise AND
SLL::OptionFlag SLL::operator&(const OptionFlag lhs, const OptionFlag rhs)
{
    return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) & static_cast<OptionFlagType>(rhs));
}

// Bitwise XOR
SLL::OptionFlag SLL::operator^(const OptionFlag lhs, const OptionFlag rhs)
{
    return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) ^ static_cast<OptionFlagType>(rhs));
}


// Bitwise OR - Assignment
SLL::OptionFlag SLL::operator|=(OptionFlag& lhs, const OptionFlag rhs)
{
    return lhs = lhs | rhs;
}

// Bitwise AND - Assignment
SLL::OptionFlag SLL::operator&=(OptionFlag& lhs, const OptionFlag rhs)
{
    return lhs = lhs & rhs;
}

// Bitwise XOR - Assignment
SLL::OptionFlag SLL::operator^=(OptionFlag& lhs, const OptionFlag rhs)
{
    return lhs = lhs ^ rhs;
}


// Left Shift
SLL::OptionFlag SLL::operator<<(const OptionFlag lhs, const OptionFlagType s)
{
    OptionFlagType val = static_cast<OptionFlagType>(lhs) << s;
    return (val > static_cast<OptionFlagType>(OptionFlag::MAX)) ? OptionFlag::MAX : static_cast<OptionFlag>(val);
}

// Right Shift
SLL::OptionFlag SLL::operator>>(const OptionFlag lhs, const OptionFlagType s)
{
    return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) >> s);
}

// Left Shift - Assignment
SLL::OptionFlag SLL::operator<<=(OptionFlag& lhs, const OptionFlagType s)
{
    return lhs = lhs << s;
}

// Right Shift - Assignment
SLL::OptionFlag SLL::operator>>=(OptionFlag& lhs, const OptionFlagType s)
{
    return lhs = lhs >> s;
}

/// OptionFlagConverter \\\

/// Private Helper Methods \\\

// OptionFlag String Getter - char
template <>
const std::basic_string<char>& SLL::OptionFlagConverter::GetOptionFlagString<char>(const size_t i)
{
    static const std::vector<std::basic_string<char>> optionFlagStringsA
    {
        "LOG_TO_STDOUT",
        "LOG_TO_FILE",
        "LOG_IN_COLOR",
        "LOG_VERBOSITY_LEVEL",
        "LOG_TIMESTAMP",
        "LOG_THREAD_ID",
        "LOG_SYNCHRONOUS"
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
const std::basic_string<wchar_t>& SLL::OptionFlagConverter::GetOptionFlagString<wchar_t>(const size_t i)
{
    static const std::vector<std::basic_string<wchar_t>> optionFlagStringsW
    {
        L"LOG_TO_STDOUT",
        L"LOG_TO_FILE",
        L"LOG_IN_COLOR",
        L"LOG_VERBOSITY_LEVEL",
        L"LOG_TIMESTAMP",
        L"LOG_THREAD_ID",
        L"LOG_SYNCHRONOUS"
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
const std::basic_string<char>& SLL::OptionFlagConverter::GetEmptyMaskString<char>( )
{
    static const std::basic_string<char> emptyMaskA("NONE");
    return emptyMaskA;
}

// OptionFlag::NONE String Getter - wchar_t
template <>
const std::basic_string<wchar_t>& SLL::OptionFlagConverter::GetEmptyMaskString<wchar_t>( )
{
    static const std::basic_string<wchar_t> emptyMaskW(L"NONE");
    return emptyMaskW;
}

// Separator Getter - char
template <>
const std::basic_string<char>& SLL::OptionFlagConverter::GetSeparator<char>( )
{
    static const std::basic_string<char> separatorA(", ");
    return separatorA;
}

// Separator Getter - wchar_t
template <>
const std::basic_string<wchar_t>& SLL::OptionFlagConverter::GetSeparator<wchar_t>( )
{
    static const std::basic_string<wchar_t> separatorW(L", ");
    return separatorW;
}


/// Public Methods \\\

// Returns underlying scalar type value of OptionFlag mask argument.
SLL::OptionFlagType SLL::OptionFlagConverter::ToScalar(OptionFlag f)
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
std::basic_string<T> SLL::OptionFlagConverter::ToString(OptionFlag flag)
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

    // Flag mask should be cleared after the loop.
    if ( flag != OptionFlag::NONE )
    {
        throw std::invalid_argument(
            __FUNCTION__" - flag is non-empty after iterating through values.  Flag = (" + 
            std::to_string(ToScalar(flag)) + 
            ")."
        );
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

    // Trim the extra space and comma.
    str.pop_back( );
    str.pop_back( );

    return str;
}


/// Explicit Template Instantiation \\\

template std::basic_string<char> SLL::OptionFlagConverter::ToString<char>(OptionFlag);
template std::basic_string<wchar_t> SLL::OptionFlagConverter::ToString<wchar_t>(OptionFlag);
