#pragma once

#include <string>

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
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& GetColorString(const size_t);

    public:

        /// Public Methods \\\

        // Returns underlying scalar type value of Color argument.
        static ColorType ToScalar(Color);

        // Return string name of Color argument.
        template <class T, typename = std::enable_if_t<std::is_same_v<T, char> || std::is_same_v<T, wchar_t>>>
        static const std::basic_string<T>& ToString(Color);
    };
}
