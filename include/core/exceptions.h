#pragma once

#include <exception>

// Defines an invalid position exception.
class InvalidPointException : public std::exception {
    public:
        virtual char const* what() const noexcept override {
            return "The point is not available.";
        }
};

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

// Defines an exception to indicate that a specific file was not found.
class FileNotFoundException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "The file was not available, or failed to create the file.";
    }
};

// Defines an exception to indicate a failure to write to a file.
class FileWritingException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "Failed to write to the file.";
    }
};

// Defines an exception to indicate a failure to read from a file.
class FileReadingException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "Failed to read from the file.";
    }
};

// Defines an exception to indicate an invaild file.
class InvalidFileException : public std::exception {
public:
    virtual char const* what() const noexcept override {
        return "The file is in a supported format.";
    }
};

// Defines an exception to indicate an illegal point.
class IllegalPointException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The selected point is not legal!";
    }
};    

// Defines an exception to indicate an empty string.
class EmptyStringException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The entered string is empty.";
    }
};

// Defines an exception to indicate a bad input.
class BadInputException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The entered input is not valid.";
    }
};

// Defines an exception to indicate a save game.
class SavegameException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The game needs to be saved.";
    }
};

// Displays the message of an exception.
void DisplayException(std::exception const &exception);