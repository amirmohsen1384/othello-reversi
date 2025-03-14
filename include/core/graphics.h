#pragma once

#include <string>

namespace Graphics {
    
    // Describes a collection of different colors available.
    enum class Color {
        Magenta, BrightMagenta,

        Yellow, BrightYellow,

        Green, BrightGreen,

        Black, Gray, White,

        Blue, BrightBlue,

        Cyan, BrightCyan,

        Red, BrightRed
    };

    // Changes the foreground of the terminal.
    void SetForeground(Color const &color);

    // Prints a string with the specified color.
    void Draw(std::string const& text, Color const &color);

    // Describes some pre-defined colors for certain purposes.
    enum class Action {
        Blank =     static_cast<int>(Color::Gray),
        User =      static_cast<int>(Color::BrightBlue),
        Opponent =  static_cast<int>(Color::BrightRed),
        Legal =     static_cast<int>(Color::BrightMagenta),
        Error =     static_cast<int>(Color::Red)
    };
    
    // Draws a seperator on the screen.
    std::ostream& DrawSeperator(std::ostream &stream, size_t count);
}

static constexpr Graphics::Color _default = Graphics::Color::White;