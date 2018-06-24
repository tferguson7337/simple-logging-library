#pragma once

#include <type_traits>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class LoggerOption : size_t
    {
        LogToStdout = 1 << 0,
        LogToFile = 1 << 1,
        LogInColor = 1 << 2,
        LogVerbosityLevel = 1 << 3,
        LogTimestamp = 1 << 4,
        LogThreadID = 1 << 5,
        LogSynchronous = 1 << 6,

        // MAX CAP
        MAX = 1 << 7
    };

    /// BITWISE OPERATOR OVERLOADS \\\

    using LoggerOptionType = std::underlying_type_t<LoggerOption>;

    // Bitwise NOT
    LoggerOption operator~(LoggerOption rhs)
    {
        return static_cast<LoggerOption>(~static_cast<LoggerOptionType>(rhs));
    }

    // Bitwise OR
    LoggerOption operator|(LoggerOption lhs, LoggerOption rhs)
    {
        return static_cast<LoggerOption>(static_cast<LoggerOptionType>(lhs) | static_cast<LoggerOptionType>(rhs));
    }

    // Bitwise AND
    LoggerOption operator&(LoggerOption lhs, LoggerOption rhs)
    {
        return static_cast<LoggerOption>(static_cast<LoggerOptionType>(lhs) & static_cast<LoggerOptionType>(rhs));
    }

    // Bitwise XOR
    LoggerOption operator^(LoggerOption lhs, LoggerOption rhs)
    {
        return static_cast<LoggerOption>(static_cast<LoggerOptionType>(lhs) ^ static_cast<LoggerOptionType>(rhs));
    }


    // Bitwise OR - Assignment
    LoggerOption operator|=(LoggerOption& lhs, LoggerOption rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    // Bitwise AND - Assignment
    LoggerOption operator&=(LoggerOption& lhs, LoggerOption rhs)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    // Bitwise XOR - Assignment
    LoggerOption operator^=(LoggerOption& lhs, LoggerOption rhs)
    {
        lhs = lhs ^ rhs;
        return lhs;
    }


    // Left Shift
    LoggerOption operator<<(LoggerOption lhs, LoggerOptionType s)
    {
        return static_cast<LoggerOption>(static_cast<LoggerOptionType>(lhs) << s);
    }

    // Right Shift
    LoggerOption operator>>(LoggerOption lhs, LoggerOptionType s)
    {
        return static_cast<LoggerOption>(static_cast<LoggerOptionType>(lhs) >> s);
    }

    // Left Shift - Assignment
    LoggerOption operator<<=(LoggerOption& lhs, LoggerOptionType s)
    {
        lhs = lhs << s;
        return lhs;
    }

    // Right Shift - Assignment
    LoggerOption operator>>=(LoggerOption& lhs, LoggerOptionType s)
    {
        lhs = lhs << s;
        return lhs;
    }
}