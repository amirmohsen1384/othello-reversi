#pragma once

#include <ostream>
#include <istream>

class ISerialize {
public:
    virtual std::ostream& ToBinary(std::ostream &stream) const = 0;
    virtual std::istream& FromBinary(std::istream &stream) = 0;

    static std::ostream& WriteString(std::ostream &stream, std::string const &string);
    static std::istream& ReadString(std::istream &stram, std::string &string);
};