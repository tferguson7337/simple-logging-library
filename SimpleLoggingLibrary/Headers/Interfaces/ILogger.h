#pragma once

// SLL Enum Classes
#include <ConfigPackage.h>

#include <Uncopyable.h>

namespace SLL
{
    class ILogger : Uncopyable
    {
        ILogger(const ILogger&) = delete;
        ILogger& operator=(const ILogger&) = delete;

    public:
        ILogger( ) = default;
        virtual ~ILogger( ) = default;

        /// Public Methods \\\

        // Submit log message to stream(s).
        // - Unfortunately, C++ doesn't allow virtual template methods.
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel, const T*, ...);
    };
}