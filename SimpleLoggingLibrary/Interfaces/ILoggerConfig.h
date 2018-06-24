#pragma once

#include <string>

namespace SLL
{
    /// Forward Declarations
    enum class LoggerOption : size_t;
    enum class VerbosityLevel : size_t;
    enum class LogColor : size_t;
    template <class T> class LoggerConfig;

    /// LoggerConfig Interface \\\

    template <class T>
    class ILoggerConfig
    {
    public:

        ILoggerConfig( ) = default;
        virtual ~ILoggerConfig( ) = default;

        /// Assignment Overloads \\\

        virtual LoggerConfig<T>& operator=(const LoggerConfig<T>&) = 0;
        virtual LoggerConfig<T>& operator=(LoggerConfig<T>&&) = 0;

        /// Getters \\\

        virtual LogColor GetLogColor(const VerbosityLevel) const = 0;
        virtual VerbosityLevel GetVerbosityThreshold( ) const noexcept = 0;
        virtual const std::basic_string<T>& GetFile( ) const noexcept = 0;

        /// Setters \\\

        virtual void SetLogColor(const LogColor, const VerbosityLevel) = 0;
        virtual void SetVerbosityThreshold(const VerbosityLevel) = 0;
        virtual void SetFile(const std::basic_string<T>&) = 0;
        virtual void SetFile(std::basic_string<T>&&) noexcept = 0;

        /// Public Methods \\\

        virtual void Enable(const LoggerOption) = 0;
        virtual void Disable(const LoggerOption) = 0;

        virtual bool OptionEnabled(const LoggerOption) const = 0;
    };
}

