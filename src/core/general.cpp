#include "include/core/general.h"

std::ostream& General::WriteString(std::ostream &stream, std::string const &string) {
    // Writes the size of the string.
    const size_t size = string.size();
    if(stream.write(reinterpret_cast<const char*>(&size), sizeof(size)).bad()) {
        return stream;
    }

    // Writes the data of the string.
    if(stream.write(reinterpret_cast<const char*>(string.data()), size).bad()) {
        return stream;
    }

    return stream;
}

std::istream& General::ReadString(std::istream &stream, std::string &string) {
    // Reads the size of the string.
    size_t size = 0;
    if(stream.read(reinterpret_cast<char*>(&size), sizeof(size)).bad()) {
        return stream;
    }

    // Clears the existing data from the string.
    string.clear();

    // Reserves enough space for the string.
    string.reserve(size);
    if(stream.read(reinterpret_cast<char*>(string.data()), size).bad()) {
        return stream;
    }

    return stream;
}