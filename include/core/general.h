#pragma once

#include <ostream>
#include <istream>
#include <cstdint>

class ISerialize {
public:
    virtual std::ostream& ToBinary(std::ostream &stream) const = 0;
    virtual std::istream& FromBinary(std::istream &stream) = 0;
};

#define START_LOOKING_OVER_BOARD(Position, Board) \
    { \
        Size _size = (Board).GetDimensions(); \
        for(Dimension _j = 0; _j < _size.GetHeight(); ++_j) { \
            for(Dimension _i = 0; _i < _size.GetWidth(); ++_i) { \
                Position = Point(_i, _j);


#define END_LOOKING_OVER_BOARD \
            } \
        } \
    }

// Describes some general functions to be used broadly.
namespace General {
    std::ostream& WriteString(std::ostream &stream, std::string const &string);
    std::istream& ReadString(std::istream &stram, std::string &string);
}

// Defines some synonyms for the common types of the class.
using Limit = int64_t;
using Index = int64_t;

#include "exceptions.h"