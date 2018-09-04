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

        // Log Prefixes to Stream.
        template <class T>
        void LogPrefixes(const VerbosityLevel&, const std::thread::id&);

        // Log User Message To File.
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

        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs);

        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, ...);
    };
}