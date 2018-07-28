#pragma once

#include <string>
#include <vector>

namespace SLL
{
    // Various console-output colors.
    enum class Color : size_t
    {
        // Basic Shades
        WHITE = 0,
        LIGHT_GREY,
        GREY,
        DARK_GREY,
        BLACK,

        // Basic Colors
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
        MAX,

        // MIN CAP
        BEGIN = 0
    };

    using ColorType = std::underlying_type_t<Color>;

    class ColorConverter
    {
        /// Static Class - No Ctors, Dtor, or Assignment Allowed
        ColorConverter( ) = delete;
        ColorConverter(const ColorConverter&) = delete;
        ColorConverter(ColorConverter&&) = delete;

        ~ColorConverter( ) = delete;

        ColorConverter& operator=(const ColorConverter&) = delete;
        ColorConverter& operator=(ColorConverter&&) = delete;

    private:
        /// Private Helper Methods \\\

        // Color String Getters
        template <class T, std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        const std::basic_string<T>& GetColorString(const size_t);

        // Color String Getter - char
        template <char>
        const std::basic_string<char>& GetColorString(const size_t i)
        {
            static const std::vector<std::basic_string<char>> colorStringsA
            {
                // Basic Shades
                "WHITE",
                "LIGHT_GREY",
                "GREY",
                "DARK_GREY",
                "BLACK",

                // Basic Colors
                "DARK_RED",
                "RED",
                "ORANGE",
                "YELLOW",
                "YELLOW_GREEN",
                "GREEN",
                "CYAN",
                "BLUE",
                "VIOLET",
                "PURPLE"
            };

            if ( i >= colorStringsA.size( ) )
            {
                throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(colorStringsA.size( ) - 1));
            }

            return colorStringsA[i];
        }

        // Color String Getter - wchar_t
        template <wchar_t>
        const std::basic_string<wchar_t>& GetColorString(const size_t i)
        {
            static const std::vector<std::basic_string<wchar_t>> colorStringsW
            {
                // Basic Shades
                L"WHITE",
                L"LIGHT_GREY",
                L"GREY",
                L"DARK_GREY",
                L"BLACK",

                // Basic Colors
                L"DARK_RED",
                L"RED",
                L"ORANGE",
                L"YELLOW",
                L"YELLOW_GREEN",
                L"GREEN",
                L"CYAN",
                L"BLUE",
                L"VIOLET",
                L"PURPLE"
            };

            if ( i >= colorStringsW.size( ) )
            {
                throw std::range_error(__FUNCTION__" - Index argument out of bounds.  Index = " + std::to_string(i) + ", Max = " + std::to_string(colorStringsW.size( ) - 1));
            }

            return colorStringsW[i];
        }

    public:

        /// Public Methods \\\

        ColorType ToScalar(Color c)
        {
            if ( c >= Color::MAX )
            {
                throw std::invalid_argument(__FUNCTION__" - Invalid color argument (" + std::to_string(static_cast<ColorType>(c)) + ").");
            }

            return static_cast<ColorType>(c);
        }

        template <class T, std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        const std::basic_string<T>& ToString(Color c)
        {
            return GetColorString<T>(ToScalar(c));
        }
    };
}
