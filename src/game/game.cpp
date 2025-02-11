#include <random>
#include <fstream>
#include <iostream>
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

void Reversi::Execute(Match &match)
{
    using namespace std;
    using namespace System;
    using namespace Graphics;

    const PointList legals = match.GetPanel().GetLegals(match.GetTurn());

    if(match.GetType() == Match::Type::SinglePlayer && match.GetTurn() == Piece::Opponent) {
        int index = GuessIndex(0, legals.size() - 1);
        match.PutPiece(legals.at(index));
        match.ToggleTurn();
        return;
    }

    while(1) {
        cout << match;
        DrawSeperator(64);

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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ':' << ' ' << "Enter the number of location to continue: ";
        cin >> index;
        if(cin.bad()) {
            throw BadInputException();
        }
        else if(index < 0 || index >= legals.size()) {
            throw InvalidPointException();
        }

        match.PutPiece(legals.at(index));
    }

    match.ToggleTurn();
}