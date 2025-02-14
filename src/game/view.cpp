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
    auto _size = this->GetPanel().GetDimensions();

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

            } else {
                auto string = std::string("");
                if(piece == Piece::User) {
                    auto color = static_cast<Color>(Action::User);
                    string.push_back(this->_user.GetName().at(0));
                    Draw(string, color);
                
                } else {    
                    auto color = static_cast<Color>(Action::Opponent);
                    string.push_back(this->_opponent.GetName().at(0));
                    Draw(string, color);

                }
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
    cout << '\b';
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

    cout << std::string(64, '=') << std::endl;
    target.PrintPanel();
    cout << std::string(64, '=') << std::endl;
    
    return stream;
}

size_t Player::Print(PlayerList const &players)
{
    using namespace Graphics;
    if (players.empty()) {
        return 0;
    }
    
    auto result = std::max_element(players.cbegin(), players.cend(), 
        [](Player const &one, Player const &two) { 
            return one.GetName().size() < two.GetName().size();
        }
    );

    const size_t tab = 8;
    const size_t length = result->GetName().size();

    for (const Player &p : players) {
        Draw(p.GetName(), Color::BrightMagenta);
        const size_t gap = (length > p.GetName().size()) ? (length - p.GetName().size()) : 0;
        std::cout << std::string(gap, ' ') << std::string(tab, ' ') << p.GetScore() << '\n';
    }

    return length;
}


std::ostream& operator<<(std::ostream &output, Player const &target)
{
    output << "Name:"   << ' ' << target.GetName()  << std::endl;
    output << "Score:"  << ' ' << target.GetScore() << std::endl;
    return output;
}

void Match::Narrate()
{
    using namespace std;
    using namespace Graphics;

    Player::Print({_user, _opponent});
    for(auto i = 0; i < 32; ++i) {
        cout << '=';
    }
    cout << endl;

    switch(_state) {
        case State::UserWon: {
            Draw(_user.GetName(), static_cast<Color>(Action::User));
            cout << ':' << ' ' << "You won the game!" << endl;
            break;
        }
        case Match::State::OpponentWon: {
            Draw(_opponent.GetName(), static_cast<Color>(Action::Opponent));
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