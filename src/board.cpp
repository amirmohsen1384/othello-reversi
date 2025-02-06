#include "board.h"

void Board::Initialize()
{
    if(this->IsEmpty()) {
        throw BlankBoardException();
    } 

    for(Dimension j = 0; j < _height; ++j) {
        for(Dimension i = 0; i < _width; ++i) {
            this->At(i, j) = Piece::Blank;
        }
    }
    
    Point point = Point((_width - 1) / 2, (_height - 1) / 2);
    
    At(point) = Piece::User;
    point.MoveRight();

    At(point) = Piece::Opponent;
    point.MoveBottom();

    At(point) = Piece::User;
    point.MoveLeft();

    At(point) = Piece::Opponent;
    point.MoveTop();
}

Board::Board(Board const &another) : Board()
{
    *this = another;
}

Board::Board(Dimension const &width, Dimension const &height) : Board()
{
    Reset(width, height);
}

Board &Board::operator=(Board const &another)
{
    // deletes the existing board.
    delete _data;

    // Creates a new one.
    try {
        _data = new Coordinate[another._width * another._height];
    }
    catch(std::bad_alloc const&) {
        throw CreationException();
    }

    // Creates a copy of all pieces in the board.
    Board &current = *this;
    for(Dimension y = 0; y < _height; ++y) {
        for(Dimension x = 0; x < _width; ++x) {
            size_t pos = x + y * _width;
            current._data[pos] = another._data[pos];
        }
    } 

    return *this;
}

Board::~Board()
{
    delete this->_data;
    this->_data = nullptr;
    this->_height = 0;
    this->_width = 0;
}

void Board::Reset(Dimension const &width, Dimension const &height)
{
    // deletes the existing board.
    delete _data;
    _data = nullptr;

    // Creates a new one.
    try {
        _data = new Coordinate[width * height];
        this->_height = height;
        this->_width = width;

        // Initializes the new board.
        this->Initialize();
    }
    catch(std::bad_alloc const&) {
        throw CreationException();
    }
}

Coordinate &Board::At(Dimension const &x, Dimension const &y)
{
    if(!IsValid(Point(x, y))) {
        throw InvalidPointException();
    }
    size_t pos = (y * _width) + x;
    return *(_data + pos);
}

const Coordinate &Board::At(Dimension const &x, Dimension const &y) const
{
    if(!IsValid(Point(x, y))) {
        throw InvalidPointException();
    }
    size_t pos = (y * _width) + x;
    return *(_data + pos);
}

size_t Board::Occurrences(Coordinate const &target) const
{
    if(IsEmpty()) {
        throw BlankBoardException();
    }

    size_t result = 0;
    for(Dimension y = 0; y < _height; ++y) {
        for(Dimension x = 0; x < _width; ++x) {
            if(target == At(x, y)) {
                result++;
            }
        }
    }

    return result;
}

bool Board::IsValid(Point const &point) const {
    Dimension x = point.GetX();
    Dimension y = point.GetY();
    if(x < 0 || x >= _width) {
        return false;
    }
    else if(y < 0 || y >= _height) {
        return false;
    }
    return true;
}

void Board::Inverse(Point const& point) {
    if(!IsValid(point)) {
        throw InvalidPointException();
    }
    Coordinate &value = At(point);
    if(value == Piece::Blank) {
        return;
    }
    value = (value == Piece::User) ? Piece::Opponent : Piece::User; 
}

void Board::Inverse(Dimension const& x, Dimension const& y) {
    this->Inverse(Point(x, y));
}

Board::BoardRow &Board::BoardRow::operator=(BoardRow const &another)
{
    this->_data = another._data;
    this->_length = another._length;
    return *this;
}

Coordinate &Board::BoardRow::At(Dimension const& y)
{
    if(y < 0 || y >= _length) {
        throw InvalidPointException();
    }
    return *(_data + y * _length);
}
const Coordinate& Board::BoardRow::At(Dimension const& y) const {
    if(y < 0 || y >= _length) {
        throw InvalidPointException();
    }
    return *(_data + y * _length);
}

std::ostream& Board::ToBinary(std::ostream &stream)
{
    if(stream.write(reinterpret_cast<const char*>(&_width), sizeof(_width)).bad()) {
        return stream;
    }    
    if(stream.write(reinterpret_cast<const char*>(&_height), sizeof(_height)).bad()) {
        return stream;
    }

    for(Dimension y = 0; y < _height; ++y) {
        for(Dimension x = 0; x < _width; ++x) {
            int8_t value = static_cast<int8_t>(At(x, y));
            if(stream.write(reinterpret_cast<const char*>(&value), sizeof(value)).bad()) {
                return stream;
            }
        }
    }

    return stream;
}

std::istream &Board::FromBinary(std::istream &stream)
{
    if(stream.read(reinterpret_cast<char*>(&_width), sizeof(_width)).bad()) {
        return stream;
    }
    if(stream.read(reinterpret_cast<char*>(&_height), sizeof(_height)).bad()) {
        return stream;
    }

    try {
        delete _data;
        _data = nullptr;
        _data = new Coordinate[_width * _height];
    }
    catch(std::bad_alloc const &) {
        throw CreationException();
    }

    for(Dimension y = 0; y < _height; ++y) {
        for(Dimension x = 0; x < _width; ++x) {
            int8_t value = 0;
            if(stream.read(reinterpret_cast<char*>(&value), sizeof(value)).bad()) {
                return stream;
            }
            At(x, y) = static_cast<Coordinate>(value);
        }
    }
    
    return stream;
}

bool operator==(Board const& one, Board const& two)
{
    if(one._width != two._width) {
        return false;
    }
    else if(one._height != two._height) {
        return false;
    }
    
    const Dimension width = one._width;
    const Dimension height = one._height;

    for(Dimension y = 0; y < height; ++y) {
        for(Dimension x = 0; x < width; ++x) {
            if(one.At(x, y) != two.At(x, y)) {
                return false;
            }
        }
    }

    return true;
}
bool operator!=(Board const& one, Board const& two)
{
    return !(one == two);
}