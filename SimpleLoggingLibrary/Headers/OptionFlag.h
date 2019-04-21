#pragma once

#include "../CommonCode/Headers/CCMacros.h"

#include <string>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class OptionFlag : size_t
    {
        // NO SETTINGS
        NONE = 0,

        // Logging Options - Global Behavior
        LogToStdout         = 1 << 0,
        LogToFile           = 1 << 1,
        LogInColor          = 1 << 2,
        LogAsynchronous     = 1 << 3,

        // Logging Options - Prefix Behavior
        LogTimestamp        = 1 << 4,
        LogThreadID         = 1 << 5,
        LogVerbosityLevel   = 1 << 6,

        // Enum Begin/Max
        BEGIN               = 1 << 0,
        MAX                 = 1 << 7,

        // Behavior Begin
        GLOBAL_BEGIN        = LogToStdout,
        PREFIX_BEGIN        = LogTimestamp,

        // Behavior End
        GLOBAL_END          = PREFIX_BEGIN,
        PREFIX_END          = MAX,

        // Behavior Masks
        ALL_MASK            = MAX - 1,
        GLOBAL_MASK         = PREFIX_BEGIN - 1,
        PREFIX_MASK         = ALL_MASK & ~GLOBAL_MASK,
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
        template <class T>
        static const std::basic_string<T>& GetOptionFlagString(const size_t);

        // OptionFlag::NONE String Getters
        template <class T>
        static const std::basic_string<T>& GetEmptyMaskString( );

        // Separator Getters
        template <class T>
        static const std::basic_string<T>& GetSeparator( );

    public:

        /// Public Methods \\\

        // Returns underlying scalar type value of OptionFlag mask argument.
        static OptionFlagType ToScalar(OptionFlag);

        template <class T>
        static std::basic_string<T> ToString(OptionFlag);
    };
}