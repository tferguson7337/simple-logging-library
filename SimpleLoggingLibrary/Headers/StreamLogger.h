#pragma once

// Parent class
#include <LoggerBase.h>

// C++ STL Streams
#include <fstream>
#include <iostream>

namespace FileLoggerTests
{
    class Tester;
}

namespace StreamLoggerTests
{
    class Tester;
}

namespace SLL
{
    template <class StreamType>
    class StreamLogger;

    // typedefs for supported stream types.
    typedef std::basic_ostream<wchar_t> StdOutStream;
    typedef std::basic_ofstream<wchar_t> FileStream;

    // typedefs for logger types
    typedef StreamLogger<StdOutStream> StdOutLogger;
    typedef StreamLogger<FileStream> FileLogger;

    ///
    //
    //  Class   - StreamLogger
    //
    //  Purpose - Output message contents to stream (e.g., stdout, file).   
    //
    ///
    template <class StreamType>
    class StreamLogger : public LoggerBase
    {
        // Friend class, intended to expose non-public methods for testing.
        friend ::FileLoggerTests::Tester;
        friend ::StreamLoggerTests::Tester;

        /// Construction requires ConfigPackage
        StreamLogger( ) = delete;

    private:
        /// Private Data Members \\\

        StreamType mStream;

        /// Private Helper Methods \\\

        // Return whether or not stream is in good state.
        bool IsStreamGood( ) const noexcept;

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