#pragma once
#include <cstdint>

namespace System {
    using KeyNumber = int32_t;

    // Erases everything from the screen.
    void EraseConsole();

    // Makes a pause in the execution flow.
    void Delay(int _duration);

    // Gets an instant key from the user.
    System::KeyNumber InstantKey();
}