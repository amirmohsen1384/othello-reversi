#include "include/core/size.h"

Size::Size() {}
Size::Size(const Size &another)
{
    *this = another;
}
Size::Size(Dimension const &width, Dimension const &height) : Size()
{
    _width = width;
    _height = height;
}
Size &Size::operator=(Size const &another)
{
    _width = another._width;
    _height = another._height;
    return *this;
}

void Size::ResetWidth()
{
    this->_width = 0;
}

Dimension Size::GetWidth() const
{
    return _width;
}

void Size::SetWidth(Dimension const &value)
{
    _width = value;
}

void Size::ResetHeight()
{
    this->_height = 0;
}

Dimension Size::GetHeight() const
{
    return _height;
}

void Size::SetHeight(Dimension const &value)
{
    _height = value;
}

void Size::Reset()
{
    ResetWidth();
    ResetHeight();
}

std::ostream &Size::ToBinary(std::ostream &stream) const
{
    if(stream.write(reinterpret_cast<const char*>(&_width), sizeof(_width)).bad()) {
        return stream;
    }
    if(stream.write(reinterpret_cast<const char*>(&_height), sizeof(_height)).bad()) {
        return stream;
    }
    return stream;
}

std::istream &Size::FromBinary(std::istream &stream)
{
    if(stream.read(reinterpret_cast<char*>(&_width), sizeof(_width)).bad()) {
        return stream;
    }
    if(stream.read(reinterpret_cast<char*>(&_height), sizeof(_height)).bad()) {
        return stream;
    }
    return stream;
}

bool operator==(Size const& one, Size const& two)
{
    return (one._width == two._width) && (one._height == two._height);
}
bool operator!=(Size const& one, Size const& two)
{
    return !(one == two);
}