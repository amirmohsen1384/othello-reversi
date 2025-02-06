#pragma once
#include "point.h"

// Defines different states of a piece in the board.
enum class Piece {User, Blank, Opponent};

// Defines each cell on the board as a coordinate.
using Coordinate = Piece;

// Defines the dimensions of the board.
using Dimension = uint8_t;

// Defines an exception to indicate an allocation failure.
class CreationException : public std::bad_alloc {
public:
    virtual char const* what() const noexcept override {
        return "Failed to create the board.";
    }
};

// Defines an exception to indicate an invalid operation on a blank board.
class BlankBoardException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "Attempt to operate on a blank board.";
    }
};

// Defines the main board of the game.
class Board : public ISerialize {
protected:
    class BoardRow {
        Dimension   _length = 0;
        Coordinate  *_data = nullptr;

    public:
        explicit BoardRow(Coordinate *init, Dimension const& length) : 
            _data(init), 
            _length(length) 
        {}
        BoardRow(BoardRow const& another) { 
            *this = another; 
        }  
        BoardRow& operator=(BoardRow const& another);

        const Coordinate& At(Dimension const& y) const;
        Coordinate& At(Dimension const& y);

        inline Coordinate& operator[](Dimension const& y) {
            return At(y);
        }
        inline const Coordinate& operator[](Dimension const& y) const {
            return At(y);
        }
    };

    virtual void Initialize();

public:
    Board() {}
    Board(Board const& another);
    Board(Dimension const& width, Dimension const& height);

    Board& operator=(Board const &another);
    ~Board();

    void Reset(Dimension const& width, Dimension const& height);

    Coordinate& At(Dimension const& x, Dimension const& y);
    const Coordinate& At(Dimension const& x, Dimension const& y) const;
    
    inline Coordinate& At(Point const& _point) {
        return At(_point.GetX(), _point.GetY());
    }
    inline const Coordinate& At(Point const& _point) const {
        return At(_point.GetX(), _point.GetY());
    }

    size_t Occurrences(Coordinate const& target) const;
    inline bool IsEmpty() const {
        return _data == nullptr;
    }
    inline bool IsFull() const {
        return Occurrences(Piece::Blank) == 0;
    }

    inline BoardRow operator[](Dimension const& x) {
        return BoardRow(_data + x, _width);
    }
    inline const BoardRow operator[](Dimension const& x) const {
        return BoardRow(_data + x, _width);
    }

    virtual std::ostream& ToBinary(std::ostream &stream);
    virtual std::istream& FromBinary(std::istream &stream);

private:
    Coordinate  *_data = nullptr;
    Dimension   _height = 0;
    Dimension   _width = 0;
};