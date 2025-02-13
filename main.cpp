#include "include/scoreboard/scoreboard.h"
#include "include/core/system.h"
#include "include/game/game.h"
#include "include/menu/menu.h"
#include <iostream>

int main() {
    Menu mainMenu;
    mainMenu.SetTitle("Othelo & Reversi - By Amir Mohsen Ghasemi");
    mainMenu.push_back("Start Game");
    mainMenu.push_back("Show High Scores");
    mainMenu.push_back("Quit The Game");

    while(true) {
        int index = mainMenu.Execute();
        System::EraseConsole();

        switch(index) {
            case 0: {
                Reversi::Play();
                break;
            }
            case 1: {
                Scoreboard score;
                std::cout << score;
                std::cout << "Press any key to go back to the main menu.";
                System::InstantKey();
                break;
            }
            case 2: {
                std::exit(EXIT_SUCCESS);
                break;
            }
        }

        System::EraseConsole();
    }
    return 0;
}