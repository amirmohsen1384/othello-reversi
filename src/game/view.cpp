#include "core/graphics.h"
#include "game/match.h"
#include <sstream>
#include <iostream>
#include <algorithm>


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