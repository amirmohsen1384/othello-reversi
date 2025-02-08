#include "game/board.h"

void Board::Initialize()
{
    if(this->IsEmpty()) {
        throw BlankBoardException();
    } 

    START_LOOKING_OVER_BOARD(Point p, *this)
        this->At(p) = Piece::Blank;
    END_LOOKING_OVER_BOARD
    
    Point point = Point((_size.GetWidth() - 1) / 2, (_size.GetHeight() - 1) / 2);
    
    At(point) = Piece::User;
    point.MoveRight();

    At(point) = Piece::Opponent;
    point.MoveBottom();

    At(point) = Piece::User;
    point.MoveLeft();

    At(point) = Piece::Opponent;
    point.MoveTop();
}

void Board::UpdateSurroundedPieces(Point const &point, Direction const &direction)
{
    PointList container = GetSurroundedPieces(At(point), point, direction);
    for(Point const& p : container) {
        Inverse(p);
    }
}

PointList Board::GetSurroundedPieces(Piece const &piece, Point const &point, Direction const &direction) const
{
    if(IsEmpty()) {
        throw BlankBoardException();

    }
    else if(piece == Piece::Blank) {
        throw BlankPieceException();

    }

    const Piece inversed = (piece == Piece::User ? Piece::Opponent : Piece::User);
    Coordinate pointer = At(point);
    PointList container;
    Point temp = point;
    
    do {
        switch(direction) {
            case Direction::Top: {
                temp.MoveTop();
                break;
            }

            case Direction::Bottom: {
                temp.MoveBottom();
                break;
            }

            case Direction::Left: {
                temp.MoveLeft();
                break;
            }

            case Direction::Right: {
                temp.MoveRight();
                break;
            }

            case Direction::TopLeft: {
                temp.MoveTopLeft();
                break;
            }

            case Direction::TopRight: {
                temp.MoveTopRight();
                break;
            }

            case Direction::BottomLeft: {
                temp.MoveBottomLeft();
                break;
            }
            
            case Direction::BottomRight: {
                temp.MoveBottomRight();
                break;
            }
        }

        try {
            pointer = At(temp);
        }
        catch(InvalidPointException const &) {
            return PointList();
        }

        if(pointer == Piece::Blank) {
            return PointList();
        }
        else if(pointer == inversed) {
            container.push_back(temp);
        }

    }
    while(pointer != piece);

    return container;
}

bool Board::IsLegal(Piece const &piece, Point const &point, Direction const &direction) const
{
    if(At(point) != Piece::Blank) {
        return false;
    }
    return (GetSurroundedPieces(piece, point, direction).size() > 0);
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
        _data = new Coordinate[another._size.GetWidth() * another._size.GetHeight()];
    }
    catch(std::bad_alloc const&) {
        throw CreationException();
    }

    // Creates a copy of all pieces in the board.
    START_LOOKING_OVER_BOARD(Point p, *this)
        this->At(p) = another.At(p);
    END_LOOKING_OVER_BOARD

    return *this;
}

Board::~Board()
{
    delete this->_data;
    this->_size.Reset();
    this->_data = nullptr;
}

void Board::Reset(Dimension const &width, Dimension const &height)
{
    // deletes the existing board.
    delete _data;
    _data = nullptr;

    // Creates a new one.
    try {
        _data = new Coordinate[width * height];
        this->_size.SetHeight(height);
        this->_size.SetWidth(width);

        // Initializes the new board.
        this->Initialize();
    }
    catch(std::bad_alloc const&) {
        throw CreationException();
    }
}

bool Board::IsLegal(Piece const &player, Point const &position) const
{
    for(int i = 0; i < 8; ++i) {
        if(IsLegal(player, position, static_cast<Direction>(i))) {
            return true;
        }
    }
    return false;
}

void Board::UpdateSurroundedPieces(Point const &point)
{
    for(int i = 0; i < 8; ++i) {
        UpdateSurroundedPieces(point, static_cast<Direction>(i));
    }
}

Size Board::GetDimensions() const
{
    return _size;
}

PointList Board::GetLegals(Piece const &player) const
{
    PointList result;
    START_LOOKING_OVER_BOARD(Point p, *this)
        if(IsLegal(player, p)) {
            result.push_back(p);
        }
    END_LOOKING_OVER_BOARD
    return result;
}

Coordinate &Board::At(Dimension const &x, Dimension const &y)
{
    if(!Contains(Point(x, y))) {
        throw InvalidPointException();
    }
    size_t pos = (y * _size.GetWidth()) + x;
    return *(_data + pos);
}
const Coordinate &Board::At(Dimension const &x, Dimension const &y) const
{
    if(!Contains(Point(x, y))) {
        throw InvalidPointException();
    }
    size_t pos = (y * _size.GetWidth()) + x;
    return *(_data + pos);
}

size_t Board::Occurrences(Piece const &target) const
{
    if(IsEmpty()) {
        throw BlankBoardException();
    }

    size_t result = 0;
    START_LOOKING_OVER_BOARD(Point p, *this)
        if(target == At(p)) {
            result++;
        }
    END_LOOKING_OVER_BOARD

    return result;
}

bool Board::Contains(Point const &point) const {
    Dimension x = point.GetX();
    Dimension y = point.GetY();
    if(x >= _size.GetWidth()) {
        return false;
    }
    else if(y >= _size.GetHeight()) {
        return false;
    }
    return true;
}

void Board::Inverse(Point const& point) {
    if(!Contains(point)) {
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
    if(y >= _length) {
        throw InvalidPointException();
    }
    return *(_data + y * _length);
}
const Coordinate& Board::BoardRow::At(Dimension const& y) const {
    if(y >= _length) {
        throw InvalidPointException();
    }
    return *(_data + y * _length);
}

std::ostream& Board::ToBinary(std::ostream &stream) const
{
    if(_size.ToBinary(stream).bad()) {
        return stream;
    }

    START_LOOKING_OVER_BOARD(Point p, *this)
        int8_t value = static_cast<int8_t>(At(p));
        if(stream.write(reinterpret_cast<const char*>(&value), sizeof(value)).bad()) {
            return stream;
        }
    END_LOOKING_OVER_BOARD
    
    return stream;
}
std::istream &Board::FromBinary(std::istream &stream)
{
    if(_size.FromBinary(stream).bad()) {
        return stream;
    }
    
    try {
        delete _data;
        _data = nullptr;
        _data = new Coordinate[_size.GetWidth() * _size.GetHeight()];
    }
    catch(std::bad_alloc const &) {
        throw CreationException();
    }

    START_LOOKING_OVER_BOARD(Point p, *this)
        int8_t value = 0;
        if(stream.read(reinterpret_cast<char*>(&value), sizeof(value)).bad()) {
            return stream;
        }
        At(p) = static_cast<Coordinate>(value);
    END_LOOKING_OVER_BOARD

    return stream;
}

bool operator==(Board const& one, Board const& two)
{
    if(one._size.GetWidth() != two._size.GetWidth()) {
        return false;
    }
    else if(one._size.GetHeight() != two._size.GetHeight()) {
        return false;
    }
    
    START_LOOKING_OVER_BOARD(Point p, one)
        if(one.At(p) != two.At(p)) {
            return false;
        }
    END_LOOKING_OVER_BOARD

    return true;
}
bool operator!=(Board const& one, Board const& two)
{
    return !(one == two);
}

bool Board::IsEmpty() const {
    return _data == nullptr;
}