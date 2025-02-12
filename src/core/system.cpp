#include "include/core/system.h"
#include <windows.h>
#include <cstdlib>

void System::EraseConsole() {
    // Clears the console.
    system("cls");

    // Resets the attributes of the console.
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode |= ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
    SetConsoleMode(hStdin, mode);
}

void System::Delay(int _duration) {
    Sleep(static_cast<DWORD>(_duration));
}

System::KeyNumber System::InstantKey()
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    INPUT_RECORD irInBuf;
    DWORD dwMode, cNumRead;
    if (!GetConsoleMode(input, &dwMode))
    {
        return -1;
    }
    
    dwMode &= ~ENABLE_ECHO_INPUT;
    dwMode &= ~ENABLE_LINE_INPUT;
    if (!SetConsoleMode(input, dwMode))
    {
        return -1;
    }

    while (true) 
    {
        if (!ReadConsoleInput(input, &irInBuf, 1, &cNumRead))
        {
            return -1;
        }
        else if (irInBuf.EventType == KEY_EVENT && irInBuf.Event.KeyEvent.bKeyDown)
        {
            return irInBuf.Event.KeyEvent.wVirtualKeyCode;
        }
    }
}
