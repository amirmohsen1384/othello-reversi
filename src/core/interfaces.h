#pragma once

#include <ostream>
#include <istream>

class ISerialize {
public:
    virtual std::ostream& ToBinary(std::ostream &stream) = 0;
    virtual std::istream& FromBinary(std::istream &stream) = 0;
};