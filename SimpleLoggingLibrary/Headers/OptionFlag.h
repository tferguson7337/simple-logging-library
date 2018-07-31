#pragma once

#include <string>

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
    OptionFlag operator~(const OptionFlag rhs);

    // Bitwise OR
    OptionFlag operator|(const OptionFlag lhs, const OptionFlag rhs);

    // Bitwise AND
    OptionFlag operator&(const OptionFlag lhs, const OptionFlag rhs);

    // Bitwise XOR
    OptionFlag operator^(const OptionFlag lhs, const OptionFlag rhs);


    // Bitwise OR - Assignment
    OptionFlag operator|=(OptionFlag& lhs, const OptionFlag rhs);

    // Bitwise AND - Assignment
    OptionFlag operator&=(OptionFlag& lhs, const OptionFlag rhs);

    // Bitwise XOR - Assignment
    OptionFlag operator^=(OptionFlag& lhs, const OptionFlag rhs);


    // Left Shift
    OptionFlag operator<<(const OptionFlag lhs, const OptionFlagType s);

    // Right Shift
    OptionFlag operator>>(const OptionFlag lhs, const OptionFlagType s);


    // Left Shift - Assignment
    OptionFlag operator<<=(OptionFlag& lhs, const OptionFlagType s);

    // Right Shift - Assignment
    OptionFlag operator>>=(OptionFlag& lhs, const OptionFlagType s);


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

        // OptionFlag::NONE String Getters
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetEmptyMaskString( );

        // Separator Getters
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetSeparator( );

    public:

        /// Public Methods \\\

        // Returns underlying scalar type value of OptionFlag mask argument.
        static OptionFlagType ToScalar(OptionFlag);

        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static std::basic_string<T> ToString(OptionFlag);
    };
}