#pragma once

// Parent class
#include <LoggerBase.h>

// C++ STL File Stream
#include <fstream>

namespace SLL
{
    class FileLogger : protected LoggerBase
    {
        /// Construction requires ConfigPackage
        FileLogger( ) = delete;

        /// No copy.
        FileLogger(const FileLogger&) = delete;
        FileLogger& operator=(const FileLogger&) = delete;

    private:

        /// Data Members \\\

        std::basic_ofstream<wchar_t> mFileStream;

        /// Private Helper Methods \\\

        void InitializeFileStream( );

    public:
        /// Constructors \\\
        
        // ConfigPackage Constructor [C]
        FileLogger(const ConfigPackage&);

        // ConfigPackage Constructor [M]
        FileLogger(ConfigPackage&&);

        // Move Constructor
        FileLogger(FileLogger&&) noexcept;

        /// Destructor \\\

        // Virtual Destructor
        virtual ~FileLogger( );

        /// Assignment Overload \\\

        // Move Assignment
        FileLogger& operator=(FileLogger&&);

        /// Public Method \\\

        // Flush buffer contents to file.
        void Flush( );
        
        // Log formatted message to file.
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> | std::is_same_v<T, wchar_t>>>
        bool Log(const VerbosityLevel lvl, const T* pFormat, ...);
    };
}