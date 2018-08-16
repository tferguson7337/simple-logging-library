#pragma once

// Parent class
#include <LoggerBase.h>

// C++ STL File Stream
#include <iostream>

namespace StreamLoggerTests
{
    class Tester;
}

namespace SLL
{
    ///
    //
    //  Class   - StreamLogger
    //
    //  Purpose - Output message contents to stream (i.e., stdout, stderr, user-provided stream).   
    //
    ///
    class StreamLogger : public LoggerBase
    {
        // Friend class, intended to expose non-public methods for testing.
        friend ::StreamLoggerTests::Tester;

        /// Construction requires ConfigPackage
        StreamLogger( ) = delete;

    private:
        /// Private Data Members \\\

        std::basic_ostream<wchar_t> mStream;

        /// Private Helper Methods \\\

        // Initialize Stream.
        void InitializeStream( );

        // Restore Stream to Good State.
        bool RestoreStream( );

        // Flush Buffer Contents To Stream.
        void Flush(const VerbosityLevel&);

        // Log Prefixes to Stream.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        void LogPrefixes(const VerbosityLevel&, const std::thread::id&);

        // Log User Message To File.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        void LogMessage(const T*, va_list);


    public:
        /// Constructors \\\

        // ConfigPackage Constructor [C]
        explicit StreamLogger(const ConfigPackage&);

        // ConfigPackage Constructor [M]
        explicit StreamLogger(ConfigPackage&&) noexcept;

        // Move Constructor
        StreamLogger(StreamLogger&&) noexcept;

        /// Destructor \\\

        // Virtual Destructor
        virtual ~StreamLogger( );

        /// Assignment Overload \\\

        // Move Assignment
        StreamLogger& operator=(StreamLogger&&);

        /// Public Methods \\\

        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs);

        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, ...);
    };
}