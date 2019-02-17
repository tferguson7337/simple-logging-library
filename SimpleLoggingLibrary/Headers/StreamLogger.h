#pragma once

// Parent class
#include "LoggerBase.h"

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

    // Aliases for supported stream types.
    using StdOutStream = std::basic_ostream<utf16>;
    using FileStream = std::basic_ofstream<utf16>;

    // Aliases for logger types
    using StdOutLogger = StreamLogger<StdOutStream>;
    using FileLogger = StreamLogger<FileStream>;

    extern const std::vector<SupportedStringTuple> s_vColorSequences;

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

        std::basic_streambuf<utf16>* mpUTF16StreamBuffer;
        mutable StreamType mStream;

        /// Private Helper Methods \\\

        // Return whether or not stream is in good state.
        bool IsStreamGood( ) const noexcept;

        // Initialize Stream.
        void InitializeStream( ) const;

        // Restore Stream to Good State.
        bool RestoreStream( ) const;

        // Flush Buffer Contents To Stream.
        void Flush(_In_ const VerbosityLevel&) const;

        // Get Color String (VerbosityLevel).
        template <class T>
        const std::basic_string<T>& GetColorSequence(_In_ const VerbosityLevel&) const;

        // Get Color String (Color).
        template <class T>
        const std::basic_string<T>& GetColorSequence(_In_ const Color&) const;

        // Internal Logging Implementation.
        template <class T>
        bool LogInternal(_In_ const VerbosityLevel&, _In_ const std::thread::id&, _In_z_ _Printf_format_string_ const T*, _In_ va_list) const;

        // Log Prefixes to Stream.
        template <class T>
        void LogPrefixes(_In_ const VerbosityLevel&, _In_ const std::thread::id&) const;

        // Log User Message To Stream.
        template <class T>
        void LogMessage(_In_z_ _Printf_format_string_ const T*, _In_ va_list) const;

    protected:
        ConfigPackage& GetConfig( ) noexcept;

    public:
        /// Constructors \\\

        // ConfigPackage Constructor [C]
        explicit StreamLogger(_In_ const ConfigPackage&);

        // ConfigPackage Constructor [M]
        explicit StreamLogger(_In_ ConfigPackage&&);

        // Move Constructor
        StreamLogger(_In_ StreamLogger&&);

        /// Destructor \\\

        // Virtual Destructor
        virtual ~StreamLogger( ) noexcept;

        /// Assignment Overload \\\

        // Move Assignment
        StreamLogger& operator=(_In_ StreamLogger&&);

        /// Public Getter \\\

        const ConfigPackage& GetConfig( ) const noexcept;

        /// Public Methods \\\

        // Submit log message to stream(s) (variadic arguments).
        bool Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf8* pFormat, ...) const;
        bool Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf16* pFormat, ...) const;

        // Submit log message to stream(s) (va_list).
        bool Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf8* pFormat, _In_ va_list pArgs) const;
        bool Log(_In_ const VerbosityLevel& lvl, _In_z_ _Printf_format_string_ const utf16* pFormat, _In_ va_list pArgs) const;

        // Submit log message to stream(s) (variadic arguments, explicit thread ID).
        bool Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf8* pFormat, ...) const;
        bool Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf16* pFormat, ...) const;

        // Submit log message to stream(s) (va_list, explicit thread ID).
        bool Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf8* pFormat, _In_ va_list pArgs) const;
        bool Log(_In_ const VerbosityLevel& lvl, _In_ const std::thread::id& tid, _In_z_ _Printf_format_string_ const utf16* pFormat, _In_ va_list pArgs) const;
    };
}