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

    Match sharedMatch;
    if(IO::Exists()) {
        Menu loadMessage;
        loadMessage.SetTitle("You have an unfinished match from your player.\nWould you like to continue?");
        loadMessage.push_back("Yes. I want to continue.");
        loadMessage.push_back("No. Start a new game.");
        switch(loadMessage.Execute()) {
            case 0: {
                IO::Load(sharedMatch);
                break;
            }
            case 1: {
                filesystem::remove(Reversi::IO::GetFilename());
                sharedMatch.Initialize();
                break;
            }
        }
    }
    else {
        sharedMatch.Initialize();
    }

    System::EraseConsole();

    while(sharedMatch.MatchContinues()) {
        try {
            sharedMatch.Execute();

        }
        catch(SavegameException const &) {            
            try {
                System::EraseConsole();
                std::string successMessage = "Your game has been saved successfully.\n";

                IO::Save(sharedMatch);
                Graphics::Draw(successMessage, Color::Green);
                Graphics::DrawSeperator(cout, successMessage.size());
                System::Delay(2000);

                Menu menu;
                menu.SetTitle("Would you like to continue the game?");
                menu.push_back("Yes! Resume the game.");
                menu.push_back("No! Quit the game.");
                if(menu.Execute() == 1) {
                    return;
                }

                System::EraseConsole();
            }
            catch(std::exception const &exception) {
                DisplayException(exception);
                return;
            }

        }
        catch(std::exception const &exception) {
            DisplayException(exception);
            return;
        }

        System::EraseConsole();
    }

    sharedMatch.Narrate();

    Player winner;
    switch(sharedMatch.GetState()) {
        case Match::State::UserWon: {
            winner = sharedMatch.GetUser();
            break;
        }
        case Match::State::OpponentWon: {
            winner = sharedMatch.GetOpponent();
            break;
        }
        default: {
            return;
        }
    }
    
    RankedPlayer ranked;
    ranked.SetName(winner.GetName());
    ranked.SetScore(winner.GetScore());
    ranked.SetSize(sharedMatch.GetPanel().GetDimensions());

    Scoreboard s;
    s.Insert(ranked);
}