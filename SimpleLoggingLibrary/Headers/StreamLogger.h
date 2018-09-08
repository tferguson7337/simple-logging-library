#pragma once

// Parent class
#include <LoggerBase.h>

// C++ STL Streams
#include <fstream>
#include <iostream>

namespace StreamLoggerTests
{
    template <class LoggerType, class StreamType>
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
    class StreamLogger : public virtual LoggerBase
    {
        // Friend class, intended to expose non-public methods for testing.
        friend ::StreamLoggerTests::Tester<FileLogger, StreamType>;
        friend ::StreamLoggerTests::Tester<StdOutLogger, StreamType>;

        /// Construction requires ConfigPackage
        StreamLogger( ) = delete;

    private:
        /// Private Data Members \\\

        StreamType mStream;
        std::wstreambuf* mpWideStreamBuffer;

        static const std::vector<std::basic_string<char>> mColorSequencesA;
        static const std::vector<std::basic_string<wchar_t>> mColorSequencesW;

        /// Private Helper Methods \\\

        // Return whether or not stream is in good state.
        bool IsStreamGood( ) const noexcept;

        // Initialize Stream.
        void InitializeStream( );

        // Restore Stream to Good State.
        bool RestoreStream( );

        // Flush Buffer Contents To Stream.
        void Flush(const VerbosityLevel&);

        // Get Color String (VerbosityLevel).
        template <class T>
        const std::basic_string<T>& GetColorSequence(const VerbosityLevel&) const;

        // Get Color String (Color).
        template <class T>
        const std::basic_string<T>& GetColorSequence(const Color&) const;

        // Internal Logging Implementation.
        template <class T>
        bool LogInternal(const VerbosityLevel&, const std::thread::id&, const T*, va_list);

        // Log Prefixes to Stream.
        template <class T>
        void LogPrefixes(const VerbosityLevel&, const std::thread::id&);

        // Log User Message To Stream.
        template <class T>
        void LogMessage(const T*, va_list);

    protected:
        ConfigPackage& GetConfig( ) noexcept;

    public:
        /// Constructors \\\

        // ConfigPackage Constructor [C]
        explicit StreamLogger(const ConfigPackage&);

        // ConfigPackage Constructor [M]
        explicit StreamLogger(ConfigPackage&&);

        // Move Constructor
        StreamLogger(StreamLogger&&);

        /// Destructor \\\

        // Virtual Destructor
        virtual ~StreamLogger( ) noexcept;

        /// Assignment Overload \\\

        // Move Assignment
        StreamLogger& operator=(StreamLogger&&);

        /// Public Getter \\\

        const ConfigPackage& GetConfig( ) const noexcept;

        /// Public Methods \\\

        // Submit log message to stream(s) (variadic arguments).
        bool Log(const VerbosityLevel& lvl, const char* pFormat, ...);
        bool Log(const VerbosityLevel& lvl, const wchar_t* pFormat, ...);

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, ...);
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, ...);

        // Submit log message to stream(s) (va_list).
        bool Log(const VerbosityLevel& lvl, const char* pFormat, va_list pArgs);
        bool Log(const VerbosityLevel& lvl, const wchar_t* pFormat, va_list pArgs);

        // Submit log message to stream(s) (va_list, explicit thread ID).
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const char* pFormat, va_list pArgs);
        bool Log(const VerbosityLevel& lvl, const std::thread::id& tid, const wchar_t* pFormat, va_list pArgs);
    };
}