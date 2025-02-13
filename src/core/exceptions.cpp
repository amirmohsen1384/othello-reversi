#include "include/core/exceptions.h"
#include "include/core/graphics.h"
#include "include/core/system.h"
#include <iostream>

// Displays an error message
void DisplayException(std::exception const &exception) {
    System::EraseConsole();

    Graphics::Draw(exception.what(), Graphics::Color::Red);
    std::cout << "Press any key to continue.";
    System::InstantKey();
}