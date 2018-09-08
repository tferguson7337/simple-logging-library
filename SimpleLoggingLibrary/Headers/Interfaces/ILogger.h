#pragma once

// SLL Enum Classes
#include <ConfigPackage.h>

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
        virtual bool Log(const VerbosityLevel&, const char*, ...) = 0;
        virtual bool Log(const VerbosityLevel&, const wchar_t*, ...) = 0;

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const char*, ...) = 0;
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const wchar_t*, ...) = 0;

        // Submit log message to stream(s) (va_list).
        virtual bool Log(const VerbosityLevel&, const char*, va_list) = 0;
        virtual bool Log(const VerbosityLevel&, const wchar_t*, va_list) = 0;

        // Submit log message to stream(s) (va_list, explicit thread ID).
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const char*, va_list) = 0;
        virtual bool Log(const VerbosityLevel&, const std::thread::id&, const wchar_t*, va_list) = 0;
    };
}