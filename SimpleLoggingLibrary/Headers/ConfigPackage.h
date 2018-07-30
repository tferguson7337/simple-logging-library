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
    class ConfigPackage
    {
    private:
        std::wstring mLogFile;
        std::vector<Color> mVerbosityColors;
        OptionFlag mOptionMask;
        VerbosityLevel mVerbosityThreshold;

        void ValidateVerbosityLevel(const VerbosityLevel, const std::string&) const;
        void ValidateColor(const Color, const std::string&) const;
        void ValidateOptionFlag(const OptionFlag, const std::string&) const;

    public:
        /// Constructors \\\

        ConfigPackage( );
        ConfigPackage(const ConfigPackage&);
        ConfigPackage(ConfigPackage&&) noexcept;

        /// Destructor \\\

        ~ConfigPackage( ) = default;

        /// Assignment Overloads \\\

        ConfigPackage& operator=(const ConfigPackage&);
        ConfigPackage& operator=(ConfigPackage&&);

        /// Getters \\\

        Color GetColor(const VerbosityLevel) const;
        VerbosityLevel GetVerbosityThreshold( ) const noexcept;
        const std::wstring& GetFile( ) const noexcept;

        /// Setters \\\

        void SetColor(const VerbosityLevel, const Color);
        void SetVerbosityThreshold(const VerbosityLevel);

        template <class T>
        void SetFile(const std::basic_string<T>&);

        template <class T>
        void SetFile(std::basic_string<T>&&);

        /// Public Methods \\\

        void Enable(const OptionFlag);
        void Disable(const OptionFlag);
        bool OptionEnabled(const OptionFlag) const;
    };

    /// TEMPLATE METHOD DEFINITIONS \\\

    // Setter ([C] Wide Conversion) - Set Target Log File
    template <>
    inline void ConfigPackage::SetFile(const std::basic_string<char>& file)
    {
        mLogFile = StringUtil::template ConvertAndCopy<wchar_t>(file);
    }

    // Setter ([M] Wide Conversion) - Set Target Log File
    template <>
    inline void ConfigPackage::SetFile(std::basic_string<char>&& file)
    {
        mLogFile = StringUtil::template ConvertAndCopy<wchar_t>(std::move(file));
    }


    // Setter (Copy) - Set Target Log File
    template <>
    inline void ConfigPackage::SetFile(const std::basic_string<wchar_t>& file)
    {
        mLogFile = file;
    }

    // Setter (Move) - Set Target Log File
    template <>
    inline void ConfigPackage::SetFile(std::basic_string<wchar_t>&& file)
    {
        mLogFile = std::move(file);
    }
}