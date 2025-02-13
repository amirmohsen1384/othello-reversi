#include "include/core/exceptions.h"
#include "include/core/graphics.h"
#include "include/core/general.h"
#include "include/core/system.h"
#include <iostream>

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
    string.resize(size);
    if(stream.read(reinterpret_cast<char*>(string.data()), size).bad()) {
        return stream;
    }

    return stream;
}

// Displays an error message
void DisplayException(std::exception const &exception) {
    System::EraseConsole();

    Graphics::Draw(exception.what(), Graphics::Color::Red);
    std::cout << "Press any key to continue.";
    System::InstantKey();
}