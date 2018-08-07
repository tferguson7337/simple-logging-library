#pragma once

#include <Macros.h>

#include <string>

namespace SLL
{
    // Various console-output colors.
    enum class Color : size_t
    {
        // Basic Colors
        WHITE = 0,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        BLACK,

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
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& GetColorString(const size_t);

    public:

        /// Public Methods \\\

        // Returns underlying scalar type value of Color argument.
        static ColorType ToScalar(Color);

        // Return string name of Color argument.
        template <class T, STRING_TEMPLATE_ENABLE_IF_SUPPORTED_TYPE(T)>
        static const std::basic_string<T>& ToString(Color);
    };
}
