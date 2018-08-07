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

        /// Getter Methods \\\
       
        virtual const ConfigPackage& GetConfig( ) const noexcept = 0;

        /// Public Methods \\\

        // Flush any buffers to log stream(s).
        virtual void Flush( ) = 0;

        // Submit log message to stream(s).
        // Unfortunately, C++ doesn't allow virtual template methods.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        bool Log(const VerbosityLevel, const T*, ...);
    };
}