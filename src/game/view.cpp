#include "core/graphics.h"
#include "game/match.h"
#include <iostream>

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