#pragma once

#include <ILoggerConfig.h>

#include <vector>

namespace SLL
{
    // Logging options that can be enabled/disabled.
    enum class LoggerOption
    {
        LogToStdout         = 1 << 0,
        LogToFile           = 1 << 1,
        LogColor            = 1 << 2, 
        LogVerbosityLevel   = 1 << 3,
        LogTimestamp        = 1 << 4,
        LogThreadID         = 1 << 5,
        LogSynchronize      = 1 << 6,

        // MAX CAP
        MAX
    };

    // Various message-type categories.
    enum class VerbosityLevel
    {
        // Standard log message.
        INFO = 0,

        // Noteworthy, yet expected behavior.
        WARN,

        // Erroneous behavior.
        ERROR,

        // Behavior leading to unrecoverable state. 
        FATAL,

        // MAX CAP
        MAX
    };

    // Various console-output colors.
    enum class LogColor
    {
        // Basic shades
        WHITE = 0,
        LIGHT_GREY,
        GREY,
        DARK_GREY,
        BLACK,

        // Basic Spectrum
        DARK_RED,
        RED,
        ORANGE,
        YELLOW,
        YELLOW_GREEN,
        GREEN,
        CYAN,
        BLUE,
        VIOLET,
        PURPLE,

        // MAX CAP
        MAX
    };


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