#pragma once

#include <Macros.h>

#include <string>

namespace SLL
{
    // Various console-output colors.
    enum class Color : size_t
    {
        // Basic Colors
        BLACK   = 0,
        RED     = 1 << 0,
        GREEN   = 1 << 1,
        YELLOW  = RED | GREEN,
        BLUE    = 1 << 2,
        MAGENTA = RED | BLUE,
        CYAN    = GREEN | BLUE,
        WHITE   = RED | GREEN | BLUE,

        // Bright Flag
        BRIGHT  = 1 << 3,

        // Default
        DEFAULT = 1 << 4,

        // MAX CAP
        MAX,

        // Color Range
        BASIC_BEGIN = BLACK,
        BASIC_END = DEFAULT,
    };

    using ColorType = std::underlying_type_t<Color>;

    // Bitwise NOT
    Color operator~(const Color rhs);

    // Bitwise OR
    Color operator|(const Color lhs, const Color rhs);

    // Bitwise AND
    Color operator&(const Color lhs, const Color rhs);

    // Bitwise XOR
    Color operator^(const Color lhs, const Color rhs);


    // Bitwise OR - Assignment
    Color operator|=(Color& lhs, const Color rhs);

    // Bitwise AND - Assignment
    Color operator&=(Color& lhs, const Color rhs);

    // Bitwise XOR - Assignment
    Color operator^=(Color& lhs, const Color rhs);


    // Left Shift
    Color operator<<(const Color lhs, const ColorType s);

    // Right Shift
    Color operator>>(const Color lhs, const ColorType s);


    // Left Shift - Assignment
    Color operator<<=(Color& lhs, const ColorType s);

    // Right Shift - Assignment
    Color operator>>=(Color& lhs, const ColorType s);


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
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        static const std::basic_string<T>& GetColorString(const size_t);

    public:

        /// Public Methods \\\

        // Returns underlying scalar type value of Color argument.
        static ColorType ToScalar(Color);

        // Return string name of Color argument.
        template <class T, ENABLE_IF_SUPPORTED_CHARACTER_TYPE(T)>
        static const std::basic_string<T>& ToString(Color);
    };
}
