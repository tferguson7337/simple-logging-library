#pragma once

#include <type_traits>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class OptionFlag : size_t
    {
        NONE                = 0,

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
}