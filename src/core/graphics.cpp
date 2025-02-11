#include "include/core/graphics.h"
#include <windows.h>
#include <iostream>

// Converts an enumeration of colors into a windows-specific code.
WORD ToForeground(const Graphics::Color &color) {
    switch(color) {
        case Graphics::Color::Black: {
            return 0;

        }
        case Graphics::Color::Gray: {
            return FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Blue: {
            return FOREGROUND_BLUE;

        }
        case Graphics::Color::BrightBlue: {
            return FOREGROUND_BLUE | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Green: {
            return FOREGROUND_GREEN;

        }
        case Graphics::Color::BrightGreen: {
            return FOREGROUND_GREEN | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Red: {
            return FOREGROUND_RED;

        }
        case Graphics::Color::BrightRed: {
            return FOREGROUND_RED | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Cyan: {
            return FOREGROUND_BLUE | FOREGROUND_GREEN;

        }
        case Graphics::Color::BrightCyan: {
            return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Magenta: {
            return FOREGROUND_BLUE | FOREGROUND_RED;

        }
        case Graphics::Color::BrightMagenta: {
            return FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::Yellow: {
            return FOREGROUND_GREEN | FOREGROUND_RED;

        }
        case Graphics::Color::BrightYellow: {
            return FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

        }
        case Graphics::Color::White: {
            return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

        }
        default: {
            return 0;

        }
    }
}

void Graphics::SetForeground(Color const &color) {
    // Obtains a handle to the output.
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    // Converts the color into an integer.
    WORD value = ToForeground(color);

    // Sets the foreground color.
    SetConsoleTextAttribute(output, value);
}

void Graphics::Draw(std::string const &text, Color const &color)
{
    using namespace std;

    // Updates the color of the terminal.
    Graphics::SetForeground(color);

    // Prints the string.
    cout << text;

    // Resets the color.
    Graphics::SetForeground(_default);   
}

void Graphics::DrawSeperator(size_t count)
{
    using namespace std;
    for(int i = 0; i < count; ++i) {
        cout << '=';
    }
    cout << endl;
}
