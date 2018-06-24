#pragma once

#include <ILoggerConfig.h>

#include <vector>

namespace SLL
{
    template <class T>
    class LoggerConfig : public virtual ILoggerConfig<T>
    {
        /// Class-local typedef
        typedef unsigned long long LogOptMask;

    private:
        std::basic_string<T> mLogFile;
        std::vector<LogColor> mVerbosityColors;
        LogOptMask mOptionMask;
        VerbosityLevel mVerbosityThreshold;

        void ValidateVerbosityLevel(const VerbosityLevel, const std::string&) const;
        void ValidateLogColor(const LogColor, const std::string&) const;
        void ValidateLoggerOption(const LoggerOption, const std::string&) const;

    public:
        /// Constructors \\\

        LoggerConfig( );
        LoggerConfig(const LoggerConfig&);
        LoggerConfig(LoggerConfig&&) noexcept;

        /// Destructor \\\

        virtual ~LoggerConfig( ) = default;

        /// Assignment Overloads \\\

        LoggerConfig& operator=(const LoggerConfig&);
        LoggerConfig& operator=(LoggerConfig&&);

        /// Getters \\\

        LogColor GetLogColor(const VerbosityLevel) const;
        VerbosityLevel GetVerbosityThreshold( ) const noexcept;
        const std::basic_string<T>& GetFile( ) const noexcept;

        /// Setters \\\

        void SetLogColor(const LogColor, const VerbosityLevel);
        void SetVerbosityThreshold(const VerbosityLevel);
        void SetFile(const std::basic_string<T>&);
        void SetFile(std::basic_string<T>&&) noexcept;

        /// Public Methods \\\

        void Enable(const LoggerOption);
        void Disable(const LoggerOption);

        bool OptionEnabled(const LoggerOption) const;
    };
}