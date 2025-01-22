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
}

static constexpr Graphics::Color _default = Graphics::Color::White;