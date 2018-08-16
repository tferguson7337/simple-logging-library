#pragma once

// Parent class
#include <LoggerBase.h>

// C++ STL File Stream
#include <fstream>

// Forward declaration of unit-testing helper-class.
namespace FileLoggerTests
{
    class Tester;
}

namespace SLL
{
    class FileLogger : public LoggerBase
    {
        // Friend class, intended to expose non-public methods for testing.
        friend ::FileLoggerTests::Tester;

        /// Construction requires ConfigPackage
        FileLogger( ) = delete;

    private:
        /// Private Data Members \\\

        std::basic_ofstream<wchar_t> mFileStream;

        /// Private Helper Methods \\\

        // Initialize File Stream.
        void InitializeFileStream( );

        // Restore File Stream.
        bool RestoreFileStream( );

        // Flush buffer contents to file.
        void Flush(const VerbosityLevel&);

        // Log Prefixes to File.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        void LogPrefixes(const VerbosityLevel&, const std::thread::id&);

        // Log User Message to File.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        void LogMessage(const T*, va_list);

    public:
        /// Constructors \\\
        
        // ConfigPackage Constructor [C]
        explicit FileLogger(const ConfigPackage&);

        // ConfigPackage Constructor [M]
        explicit FileLogger(ConfigPackage&&);

        // Move Constructor
        FileLogger(FileLogger&&) noexcept;

        /// Destructor \\\

        // Virtual Destructor
        virtual ~FileLogger( );

        /// Assignment Overload \\\

        // Move Assignment
        FileLogger& operator=(FileLogger&&);

        /// Public Methods \\\

        // Log formatted message to file (va_list).
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, va_list pArgs);
        
        // Log formatted message to file (variadic args).
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        bool Log(const VerbosityLevel& lvl, const T* pFormat, ...);
    };
}