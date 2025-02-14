#include <cstring>
#include <fstream>
#include <iostream>
#include "include/menu/menu.h"
#include "include/game/game.h"
#include "include/core/graphics.h"
#include "include/scoreboard/scoreboard.h"

using Magic = uint64_t;

Magic MagicNumber() {
    constexpr Magic magic = 0xF0C0D04;
    return magic;
}

std::filesystem::path Reversi::IO::GetFilename()
{
    using namespace std::filesystem;

    // Forms the main file path.
    const path simple = "saves/game.revf";
    
    // Makes the file path if not available.
    create_directories(simple.parent_path());

    return simple;
}

void Reversi::IO::Save(const Match &match)
{
    using namespace std;
    const filesystem::path filename = GetFilename();
    
    ofstream stream(filename.string(), ios::binary | ios::out | ios::trunc);
    if(!stream.is_open()) {
        throw FileNotFoundException();
    } 

    const Magic magic = MagicNumber();
    if(stream.write(reinterpret_cast<const char*>(&magic), sizeof(magic)).bad()) {
        throw FileWritingException();
    }

    if(match.ToBinary(stream).bad()) {
        throw FileWritingException();
    }
}

void Reversi::IO::Load(Match &match)
{
    using namespace std;
    const filesystem::path filename = GetFilename();
    
    ifstream stream(filename.string(), ios::binary | ios::in);
    if(!stream.is_open()) {
        throw FileNotFoundException();
    } 

    Magic magic = 0;
    if(stream.read(reinterpret_cast<char*>(&magic), sizeof(magic)).bad()) {
        throw FileReadingException();
    
    } else if(magic != MagicNumber()) {
        throw InvalidFileException();

    }

    if(match.FromBinary(stream).bad()) {
        throw FileReadingException();
    }
}

bool Reversi::IO::Exists()
{
    using namespace std::filesystem;
    return exists(GetFilename());
}

void Reversi::Play()
{
    using namespace std;
    using namespace Graphics;

    Match mainMatch;
    if(IO::Exists()) {
        bool result = Menu::Confirm("You have an unfinished match from your player.\nWould you like to continue?");
        if(result) {
            IO::Load(mainMatch);
        } else {
            filesystem::remove(Reversi::IO::GetFilename());
            mainMatch.Initialize();
        }

    } else {
        mainMatch.Initialize();
    }

    System::EraseConsole();

    while(mainMatch.MatchContinues()) {
        try {
            mainMatch.Execute();
            System::EraseConsole();
        }
        catch(SavegameException const &) {            
            try {
                IO::Save(mainMatch);
                bool result = Menu::Confirm("Saved successfully. Would you like to continue the game?");
                if(!result) {
                    return;
                }
                System::EraseConsole();
            }
            catch(exception const &exception) {
                System::EraseConsole();
                DisplayException(exception);
                return;
            }

        }
        catch(BadInputException const &exception) {
            cin.clear();
            System::EraseConsole();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Draw(exception.what(), static_cast<Color>(Action::Error));
            cout << std::endl << std::string(std::strlen(exception.what()), '=') << std::endl;
        }
        catch(MenuReturnException const &) {
            System::EraseConsole();
            return;
        }
        catch(IllegalPointException const &exception) {
            System::EraseConsole();
            Draw(exception.what(), static_cast<Color>(Action::Error));
            cout << std::endl << std::string(std::strlen(exception.what()), '=') << std::endl;
        }
    }

    mainMatch.Narrate();

    Player winner;
    switch(mainMatch.GetState()) {
        case Match::State::UserWon: {
            winner = mainMatch.GetUser();
            break;
        }
        case Match::State::OpponentWon: {
            winner = mainMatch.GetOpponent();
            break;
        }
        default: {
            return;
        }
    }
    
    RankedPlayer ranked;
    ranked.SetName(winner.GetName());
    ranked.SetScore(winner.GetScore());
    ranked.SetSize(mainMatch.GetPanel().GetDimensions());

    Scoreboard s;
    s.Insert(ranked);
}