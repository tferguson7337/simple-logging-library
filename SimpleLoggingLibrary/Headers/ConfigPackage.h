#pragma once

// Enum Class Definitions
#include <Color.h>
#include <OptionFlag.h>
#include <VerbosityLevel.h>

// String Conversion and Macros
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
        // Color vector, mapped to verbosity levels.
        std::vector<Color> mVerbosityColors;

        // Option flag mask to specify desired logger functionality.
        OptionFlag mOptionMask;

        // Verbosity level to specifiy desired logging threshold.
        VerbosityLevel mVerbosityThreshold;

        // Wide string for target log filename.
        std::wstring mLogFile;

        /// Private Helper Methods \\\

        // Sanity checker for verbosity level arguments.
        static void ValidateVerbosityLevel(const VerbosityLevel, const std::string&);

        // Sanity checker for color arguments.
        static void ValidateColor(const Color, const std::string&);

        // Sanity check for option flag arguments.
        static void ValidateOptionFlag(const OptionFlag, const std::string&);

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

        // Returns currently option flag mask representing currently enabled options.
        OptionFlag GetOptionFlags( ) const noexcept;

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
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        void SetFile(const std::basic_string<T>&);

        // Specifies file to log to [M].
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        void SetFile(std::basic_string<T>&&);

        /// Public Methods \\\

        // Enables specified logger functionality.
        void Enable(const OptionFlag);

        // Disables specified logger functionality.
        void Disable(const OptionFlag);

        // Wrapper For OptionsEnabledAll
        bool OptionEnabled(const OptionFlag) const;

        // Returns true if specified logger functionality is enabled.
        // If argument specifies more than one option, this
        // will only return true if one or more options are enabled.
        bool OptionsEnabledAny(const OptionFlag) const;

        // Returns true if specified logger functionality is enabled.
        // If argument specifies more than one option, this 
        // will only return true if all options are enabled.
        bool OptionsEnabledAll(const OptionFlag) const;
    };
}