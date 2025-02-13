#include <random>
#include <fstream>
#include <iostream>
#include "include/menu/menu.h"
#include "include/game/game.h"
#include "include/core/system.h"
#include "include/core/graphics.h"

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

int GuessIndex(size_t min_index, size_t max_index) {
    if(min_index > max_index) {
        return -1;
    }
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distro(min_index, max_index);
    return distro(generator);
}

bool Reversi::Execute(Match &match)
{
    using namespace std;
    using namespace System;
    using namespace Graphics;

    const PointList legals = match.GetPanel().GetLegals(match.GetTurn());

    if(match.GetType() == Match::Type::SinglePlayer && match.GetTurn() == Piece::Opponent) {
        int index = GuessIndex(0, legals.size() - 1);
        match.PutPiece(legals.at(index));
        match.ToggleTurn();
        return true;
    }

    while(1) {
        cout << match;
        switch(match.GetTurn()) {
            case Piece::User: {
                Draw(match.GetUser().GetName(), static_cast<Color>(Action::User));
                break;
            }
            case Piece::Opponent: {
                Draw(match.GetOpponent().GetName(), static_cast<Color>(Action::Opponent));
                break;
            }
            default: {
                throw BlankPieceException();
            }
        }

        int index = 0;
        cout << ':' << ' ' << "Enter the number of location to continue: ";
        cin >> index;
        if(cin.bad()) {
            throw BadInputException();
        }
        else if(index < 1 || index > static_cast<int>(legals.size())) {
            if(index == -1) {
                try {
                    IO::Save(match);
                    return false;
                }
                catch(std::exception const &exception) {
                    Draw(exception.what(), Color::Red);
                    cout << '\n' << "Press any key to come back to the game..." << '\n';
                    System::InstantKey();
                    System::EraseConsole();
                    continue;
                }
            } else {
                throw InvalidPointException();    
            }
        }

        match.PutPiece(legals.at(index - 1));
        break;
    }

    match.ToggleTurn();
    return true;
}

std::string GetName(std::string const &message) {
    std::string name;
    using namespace std;
    using namespace Graphics;
    while(true) {
        try {
            Draw(message, Color::Green);
            std::getline(std::cin, name);
            if (name.empty()) {
                System::EraseConsole();
                throw EmptyStringException();
            }
            break;
        }
        catch(EmptyStringException const &exception) {
            Draw("The player's name should be entered.\n", Color::Red);
        }
    }
    return name;
}


Match Reversi::Initialize()
{
    using namespace std;
    using namespace Graphics;

    Menu typeMessage;
    typeMessage.SetTitle("How is the match going to be played?");
    typeMessage.push_back("Single Player");
    typeMessage.push_back("Multi Player");

    Match match(static_cast<Match::Type>(typeMessage.Execute()));
    System::EraseConsole();

    switch(match.GetType()) {
        case Match::Type::SinglePlayer: {
            std::string name = GetName("Enter your name: ");
            System::EraseConsole();
            match.SetUserName(name);
            match.SetOpponentName("Computer");
            break;
        }
        case Match::Type::DoublePlayer: {
            std::string user = GetName("Enter the name of player 1: ");
            System::EraseConsole();

            std::string opponent = GetName("Enter the name of player 2: ");
            System::EraseConsole();

            match.SetOpponentName(opponent);
            match.SetUserName(user);
            break;
        }
    }

    Menu sizeMenu;
    sizeMenu.SetTitle("Select one of the following dimensions to start:");
    sizeMenu.SetOrientation(Orientation::Horizontal);
    sizeMenu.push_back("6 x 6");
    sizeMenu.push_back("8 x 8");
    sizeMenu.push_back("10 x 10");
    auto index = sizeMenu.Execute();
    switch(index) {
        case 0: {
            match.GetPanel().Reset(6, 6);
            break;
        }
        case 1: {
            match.GetPanel().Reset(8, 8);
            break;
        }
        case 2: {
            match.GetPanel().Reset(10, 10);
            break;
        }
    }
    System::EraseConsole();

    Menu turnMenu;
    turnMenu.SetTitle("Who wants to go first in this match:");
    turnMenu.SetOrientation(Orientation::Vertical);
    turnMenu.push_back(match.GetUser().GetName() + " goes first.");
    turnMenu.push_back(match.GetOpponent().GetName() + " goes first.");
    if(turnMenu.Execute() == 1) {
        match.ToggleTurn();
    }
    System::EraseConsole();

    return match;
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
                sharedMatch = Initialize();
                break;
            }
        }
    }
    else {
        sharedMatch = Initialize();
    }

    System::EraseConsole();
    

    while(sharedMatch.MatchContinues()) {
        try {
            if(Execute(sharedMatch) == false) {
                System::EraseConsole();
                Graphics::Draw("Saved Successfully!", Color::Green);
                cout << endl << "Press any key to to exit...";
                System::InstantKey();
                return;

            }
        }
        catch(std::exception const &execption) {
            Draw(execption.what(), Color::Red);
            cout << endl;
            DrawSeperator(cout, 64);
        }
        System::EraseConsole();
    }
   
    Narrate(sharedMatch);
}