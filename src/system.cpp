#include "system.h"
#include <cstdlib>
#include <windows.h>

void System::EraseConsole() {
    system("cls");
}

void System::Delay(int _duration) {
    Sleep(static_cast<DWORD>(_duration));
}
