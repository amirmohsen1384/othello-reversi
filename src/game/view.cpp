#include "include/core/graphics.h"
#include "include/game/match.h"
#include "include/game/game.h"
#include "include/menu/menu.h"
#include <algorithm>
#include <iostream>

void Match::PrintPanel() const {
    using namespace std;
    using namespace Graphics;

    // Queries for the legal points for the current player.
    auto list = _panel.GetLegals(this->_turn);

    // Queries for the dimensions of the panel.
    auto _size = this->GetPanelSize();

    // Prints the panel.
    for(auto _j = 0; _j < _size.GetHeight(); ++_j) {
        cout << ' ';

        for(auto _i = 0; _i < _size.GetWidth(); ++_i) {
            const auto location = Point(_i, _j);
            const auto piece = _panel.At(location);

            if(piece == Piece::Blank) {
                auto color = static_cast<Color>(Action::Blank);
                const auto match = std::find(list.begin(), list.end(), location);
                if(match != list.end()) {
                    const auto index = std::distance(list.begin(), match) + 1;
                    color = static_cast<Color>(Action::Legal);

                    SetForeground(color);
                    cout << '\b' << '(' << index << ')';
                    SetForeground(_default);

                }
                else {
                    Draw("O", color);

                }
            
            }

            auto string = std::string("");
            if(piece == Piece::User) {
                auto color = static_cast<Color>(Action::User);
                string.push_back(this->_user.GetName().at(0));
                Draw(string, color);
            
            } else {    
                auto color = static_cast<Color>(Action::Opponent);
                string.push_back(this->_user.GetName().at(0));
                Draw(string, color);

            }

            cout << '\t';
        }
        
        cout << '\n' << '\n';
    }
}

std::ostream &operator<<(std::ostream &stream, Board const &board)
{
    using namespace std;
    if(board.IsEmpty()) {
        throw BlankBoardException();
    }
    
    for(Dimension y = 0; y < board._size.GetHeight(); ++y) {
        for(Dimension x = 0; x < board._size.GetWidth(); ++x) {
            const Coordinate &value = board.At(x, y);
            switch(value) {
                case Piece::Opponent: {
                    Graphics::Draw("O", Graphics::Color::BrightRed);
                    break;
                }
                case Piece::User: {
                    Graphics::Draw("U", Graphics::Color::BrightBlue);
                    break;
                }
                case Piece::Blank: {
                    cout << 'O';
                }
            }
            cout << '\t';
        }
        cout << '\b' << '\n' << '\n';
    }
    return stream;
}

std::ostream& operator<<(std::ostream &stream, Match const &target)
{
    using namespace std;
    using namespace Graphics;

    SetForeground(static_cast<Color>(Action::User));
    stream << target._user.GetName() << '\n';
    stream << target._user.GetScore() << '\n';
    stream << endl;

    SetForeground(static_cast<Color>(Action::Opponent));
    stream << target._opponent.GetName() << '\n';
    stream << target._opponent.GetScore() << '\n';
    stream << endl;

    SetForeground(_default);
    for(auto i = 0; i < 64; ++i) {
        stream << '=';
    }
    stream << endl;

    target.PrintPanel();
    for(auto i = 0; i < 64; ++i) {
        stream << '=';
    }
    stream << endl;
    return stream;
}

size_t Player::Print(PlayerList const &players)
{
    using namespace Graphics;
    using namespace std;

    auto result = std::max_element(players.cbegin(), players.cend(), 
        [](Player const &one, Player const &two) { 
            return one._name > two._name;
        });

    if(result == players.end()) {
        return 0;
    }

    const size_t tab = 8;
    const size_t length = result->GetName().size();

    for(const Player &p : players) {
        Draw(p.GetName(), Color::BrightMagenta);

        const size_t gap = length - p.GetName().size();
        for(size_t i = 0; i < gap; ++i) {
            cout << ' ';
        }

        for(size_t i = 0; i < tab; ++i) {
            cout << ' ';
        }

        cout << p.GetScore() << '\n';
    }

    return length;
}

std::ostream& operator<<(std::ostream &output, Player const &target)
{
    output << "Name:"   << ' ' << target.GetName()  << std::endl;
    output << "Score:"  << ' ' << target.GetScore() << std::endl;
    return output;
}

void Reversi::Narrate(Match const &match)
{
    using namespace std;
    using namespace Graphics;
    
    Player::Print({match.GetUser(), match.GetOpponent()});
    for(auto i = 0; i < 32; ++i) {
        cout << '=';
    }
    cout << endl;

    switch(match.GetState()) {
        case Match::State::UserWon: {
            Draw(match.GetUser().GetName(), static_cast<Color>(Action::User));
            cout << ':' << ' ' << "You won the game!" << endl;
            break;
        }
        case Match::State::OpponentWon: {
            Draw(match.GetOpponent().GetName(), static_cast<Color>(Action::Opponent));
            cout << ':' << ' ' << "You won the game!" << endl;
            break;
        }
        case Match::State::GameDraw: {
            cout << "Game Draw! Nobody won in this match!" << endl;
            break;
        }
        case Match::State::Unspecified: {
            cout << "The match is unfinished." << endl;
            break;
        }
    }
}

std::string GetName(std::string const &message) {
    std::string name;
    using namespace std;
    using namespace Graphics;
    while(true) {
        try {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Draw(message, Color::Green);
            getline(cin, name);
            if(name.empty()) {
                System::EraseConsole();
                throw EmptyStringException();
            }
            break;
        }
        catch(EmptyStringException const &exception) {
            Draw("The player's name should entered.\n", Color::Red);
        }
    }
    return name;
}

void Reversi::Initialize(Match &match)
{
    using namespace std;
    using namespace Graphics;

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
            match.ResizePanel(Size(6, 6));
            break;
        }
        case 1: {
            match.ResizePanel(Size(8, 8));
            break;
        }
        case 2: {
            match.ResizePanel(Size(10, 10));
            break;
        }
    }

    Menu turnMenu;
    turnMenu.SetTitle("Who wants to go first in this match:");
    turnMenu.SetOrientation(Orientation::Vertical);
    turnMenu.push_back(match.GetUser().GetName() + " goes first.");
    turnMenu.push_back(match.GetOpponent().GetName() + " goes first.");
    if(turnMenu.Execute() == 1) {
        match.ToggleTurn();
    }
}