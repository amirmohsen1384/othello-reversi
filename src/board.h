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

// Defines an exception to indicate unability to identify the legality of a blank piece.
class BlankPieceException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "Attempt to pass a blank piece to identify the legality.";
    }
};

// Defines the main board of the game.
class Board : public ISerialize {
protected:

    // Describes a row of the board.
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

private:

    // Describes different directions to be applied on the board.
    enum class Direction {
        TopLeft = 0,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left
    };

    /* Returns a list of surrounded pieces in 'point' on the board 
     * towrards 'direction' if 'piece' gets placed in that point. */
    PointList GetSurroundedPieces(Piece const &piece, Point const &point, Direction const &direction) const;

    // Returns true if it's legal to place 'piece' in 'point' towards 'direction'. 
    bool IsLegal(Piece const& piece, Point const& point, Direction const& direction) const;
    
    // Updates surrounded pieces in 'point' towards 'direction'.
    void UpdateSurroundedPieces(Point const &point, Direction const &direction);

public:
    Board() {}
    Board(Board const& another);
    Board(Dimension const& width, Dimension const& height);

    Board& operator=(Board const &another);
    ~Board();

    void Reset(Dimension const& width, Dimension const& height);

    bool IsLegal(Piece const& player, Point const& position) const;

    void UpdateSurroundedPieces(Point const &point);

    PointList GetLegals(Piece const& player) const;

    size_t Occurrences(Piece const& target) const;

    Coordinate& At(Dimension const& x, Dimension const& y);
    const Coordinate& At(Dimension const& x, Dimension const& y) const;

    bool IsEmpty() const;
    bool IsFull() const;
    bool IsSquared() const;
    bool Contains(Point const &point) const;

    void Inverse(Point const& point);
    void Inverse(Dimension const& x, Dimension const& y);

    inline BoardRow operator[](Dimension const& x) {
        return BoardRow(_data + x, _width);
    }
    inline const BoardRow operator[](Dimension const& x) const {
        return BoardRow(_data + x, _width);
    }
    inline Coordinate& At(Point const& _point) {
        return At(_point.GetX(), _point.GetY());
    }
    inline const Coordinate& At(Point const& _point) const {
        return At(_point.GetX(), _point.GetY());
    }

    virtual void Initialize();
    virtual std::ostream& ToBinary(std::ostream &stream);
    virtual std::istream& FromBinary(std::istream &stream);

    friend bool operator==(Board const& one, Board const& two);
    friend bool operator!=(Board const& one, Board const& two);

private:
    Coordinate  *_data = nullptr;
    Dimension   _height = 0;
    Dimension   _width = 0;
};

bool operator==(Board const& one, Board const& two);
bool operator!=(Board const& one, Board const& two);