#pragma once

#include <string>
#include <vector>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class OptionFlag : size_t
    {
        // Logging Options
        LogToStdout         = 1 << 0,
        LogToFile           = 1 << 1,
        LogInColor          = 1 << 2,
        LogVerbosityLevel   = 1 << 3,
        LogTimestamp        = 1 << 4,
        LogThreadID         = 1 << 5,
        LogSynchronous      = 1 << 6,

        // NO SETTINGS
        NONE                = 0,

        // MIN CAP
        BEGIN               = 1 << 0,

        // MAX CAP
        MAX                 = 1 << 7
    };

    /// BITWISE OPERATOR OVERLOADS \\\

    using OptionFlagType = std::underlying_type_t<OptionFlag>;

    // Bitwise NOT
    inline OptionFlag operator~(const OptionFlag rhs)
    {
        return static_cast<OptionFlag>(~static_cast<OptionFlagType>(rhs) & (static_cast<OptionFlagType>(OptionFlag::MAX) - 1));
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
        return lhs = lhs | rhs;
    }

    // Bitwise AND - Assignment
    inline OptionFlag operator&=(OptionFlag& lhs, const OptionFlag rhs)
    {
        return lhs = lhs & rhs;
    }

    // Bitwise XOR - Assignment
    inline OptionFlag operator^=(OptionFlag& lhs, const OptionFlag rhs)
    {
        return lhs = lhs ^ rhs;
    }


    // Left Shift
    inline OptionFlag operator<<(const OptionFlag lhs, const OptionFlagType s)
    {
        OptionFlagType val = static_cast<OptionFlagType>(lhs) << s;
        return (val > static_cast<OptionFlagType>(OptionFlag::MAX)) ? OptionFlag::MAX : static_cast<OptionFlag>(val);
    }

    // Right Shift
    inline OptionFlag operator>>(const OptionFlag lhs, const OptionFlagType s)
    {
        return static_cast<OptionFlag>(static_cast<OptionFlagType>(lhs) >> s);
    }

    // Left Shift - Assignment
    inline OptionFlag operator<<=(OptionFlag& lhs, const OptionFlagType s)
    {
        return lhs = lhs << s;
    }

    // Right Shift - Assignment
    inline OptionFlag operator>>=(OptionFlag& lhs, const OptionFlagType s)
    {
        return lhs = lhs >> s;
    }


    class OptionFlagConverter
    {
        /// Static Class - No Ctors, Dtor, or Assignment Allowed
        OptionFlagConverter( ) = delete;
        OptionFlagConverter(const OptionFlagConverter&) = delete;
        OptionFlagConverter(OptionFlagConverter&&) = delete;
        
        ~OptionFlagConverter( ) = delete;

        OptionFlagConverter& operator=(const OptionFlagConverter&) = delete;
        OptionFlagConverter& operator=(OptionFlagConverter&&) = delete;

    private:
        /// Private Helper Methods \\\

        // OptionFlag String Getters
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetOptionFlagString(const size_t);

        // OptionFlag String Getter - char
        template <>
        static const std::basic_string<char>& GetOptionFlagString(const size_t i)
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
                throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(optionFlagStringsA.size( ) - 1));
            }

            return optionFlagStringsA[i];
        }

        // OptionFlag String Getter - wchar_t
        template <>
        static const std::basic_string<wchar_t>& GetOptionFlagString(const size_t i)
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
                throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(optionFlagStringsW.size( ) - 1));
            }

            return optionFlagStringsW[i];
        }


        // OptionFlag::NONE String Getters
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetEmptyMaskString( );

        // OptionFlag::NONE String Getter - char
        template <>
        static const std::basic_string<char>& GetEmptyMaskString( )
        {
            static const std::basic_string<char> emptyMaskA("NONE");
            return emptyMaskA;
        }

        // OptionFlag::NONE String Getter - wchar_t
        template <>
        static const std::basic_string<wchar_t>& GetEmptyMaskString( )
        {
            static const std::basic_string<wchar_t> emptyMaskW(L"NONE");
            return emptyMaskW;
        }


        // Separator Getters
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetSeparator( );
        
        // Separator Getter - char
        template <>
        static const std::basic_string<char>& GetSeparator( )
        {
            static const std::basic_string<char> separatorA(", ");
            return separatorA;
        }

        // Separator Getter - wchar_t
        template <>
        static const std::basic_string<wchar_t>& GetSeparator( )
        {
            static const std::basic_string<wchar_t> separatorW(L", ");
            return separatorW;
        }

    public:

        /// Public Methods \\\

        static OptionFlagType ToScalar(OptionFlag flag)
        {
            if ( flag < OptionFlag::NONE || flag > OptionFlag::MAX )
            {
                throw std::invalid_argument(__FUNCTION__" - Invalid flag argument (" + std::to_string(static_cast<OptionFlagType>(flag)) + ").");
            }

            return static_cast<OptionFlagType>(flag);
        }

        template <class T/*, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>*/>
        static std::basic_string<T> ToString(OptionFlag flag)
        {
            std::basic_string<T> str;
            size_t i = 0;

            // Ensure flag argument is valid.
            if ( flag >= OptionFlag::MAX )
            {
                throw std::invalid_argument(__FUNCTION__" - Invalid flag argument (" + std::to_string(ToScalar(flag)) + ").");
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
                throw std::invalid_argument(__FUNCTION__" - flag is non-empty after iterating through values.  Flag = (" + std::to_string(ToScalar(flag)) + ").");
            }

            // The string should not be empty after the loop.
            if ( str.empty( ) )
            {
                throw std::runtime_error(__FUNCTION__" - No matches were found.  Flag = (" + std::to_string(ToScalar(flag)) + ").");
            }

            // Trim the extra space and comma.
            str.pop_back( );
            str.pop_back( );

            return str;
        }
    };
}