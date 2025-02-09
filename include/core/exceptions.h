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