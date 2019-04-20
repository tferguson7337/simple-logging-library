#pragma once

// CC Types
#include <CCTypes.h>

// SLL Enum Classes
#include "../ConfigPackage.h"

// STL - Thread ID
#include <thread>

namespace SLL
{
    class ILogger
    {
        ILogger(const ILogger&) = delete;
        ILogger& operator=(const ILogger&) = delete;

    public:
        ILogger( ) = default;
        virtual ~ILogger( ) = default;

        /// Public Methods \\\

        // Submit log message to stream(s) (variadic arguments).
        virtual bool Log(const VerbosityLevel&, const utf8*, ...) const = 0;
        virtual bool Log(const VerbosityLevel&, const utf16*, ...) const = 0;

        // Submit log message to stream(s) (va_list).
        virtual bool Log(const VerbosityLevel&, const utf8*, va_list) const = 0;
        virtual bool Log(const VerbosityLevel&, const utf16*, va_list) const = 0;

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const utf8*, ...) const = 0;
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const utf16*, ...) const = 0;

        // Submit log message to stream(s) (va_list, explicit thread ID).
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const utf8*, va_list) const = 0;
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const utf16*, va_list) const = 0;
    };
}