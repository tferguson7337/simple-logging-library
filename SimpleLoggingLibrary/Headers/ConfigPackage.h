#pragma once

// Enum Class Definitions
#include <Color.h>
#include <OptionFlag.h>
#include <VerbosityLevel.h>

// String Conversion
#include <StringUtil.hpp>

// STL
#include <vector>

namespace SLL
{
    ///
    //
    //  Class   - ConfigPackage
    //
    //  Purpose - Used in conjunction with LoggerFactory to construct a Logger object.
    //
    ///
    class ConfigPackage
    {
    private:
        // Wide string for target log filename.
        std::wstring mLogFile;

        // Color vector, mapped to verbosity levels.
        std::vector<Color> mVerbosityColors;

        // Option flag mask to specify desired logger functionality.
        OptionFlag mOptionMask;

        // Verbosity level to specifiy desired logging threshold.
        VerbosityLevel mVerbosityThreshold;

        /// Private Helper Methods \\\

        // Sanity checker for verbosity level arguments.
        void ValidateVerbosityLevel(const VerbosityLevel, const std::string&) const;

        // Sanity checker for color arguments.
        void ValidateColor(const Color, const std::string&) const;

        // Sanity check for option flag arguments.
        void ValidateOptionFlag(const OptionFlag, const std::string&) const;

    public:
        /// Constructors \\\

        // Constructor (zero-initialize)
        ConfigPackage( );

        // Copy Constructor
        ConfigPackage(const ConfigPackage&);

        // Move Constructor
        ConfigPackage(ConfigPackage&&) noexcept;

        /// Destructor \\\

        // Destructor (default)
        ~ConfigPackage( ) = default;

        /// Assignment Overloads \\\

        // Copy Assignment
        ConfigPackage& operator=(const ConfigPackage&);

        // Move Assignment
        ConfigPackage& operator=(ConfigPackage&&);

        /// Getters \\\

        // Returns configured color output for specified verbosity threshold.
        Color GetColor(const VerbosityLevel) const;

        // Returns configured verbosity threshold.
        VerbosityLevel GetVerbosityThreshold( ) const noexcept;

        // Returns configured target file.
        const std::wstring& GetFile( ) const noexcept;

        /// Setters \\\

        // Sets color output for specified verbosity level.
        void SetColor(const VerbosityLevel, const Color);

        // Sets minimum verbosity level required to trigger logger output.
        void SetVerbosityThreshold(const VerbosityLevel);

        // Specifies file to log to [C].
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        void SetFile(const std::basic_string<T>&);

        // Specifies file to log to [M].
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        void SetFile(std::basic_string<T>&&);

        /// Public Methods \\\

        // Enables specified logger functionality.
        void Enable(const OptionFlag);

        // Disables specified logger functionality.
        void Disable(const OptionFlag);

        // Returns true if specified logger functionality is enabled.
        // If argument specifies more than one option, this 
        // will only return true if all options are enabled.
        bool OptionEnabled(const OptionFlag) const;
    };
}