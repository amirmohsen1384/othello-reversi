#pragma once

#include "interfaces.h"
#include <cstdint>

// Defines the dimensions of the board.
using Dimension = uint8_t;

class Size : public ISerialize {
public:
    Size();
    Size(const Size &another);
    Size(Dimension const &width, Dimension const &height);

    Size& operator=(Size const &another);

    void ResetWidth();
    Dimension GetWidth() const;
    void SetWidth(Dimension const &value);

    void ResetHeight();
    Dimension GetHeight() const;
    void SetHeight(Dimension const &value);

    void Reset();

    virtual std::ostream& ToBinary(std::ostream &stream) const override;
    virtual std::istream& FromBinary(std::istream &stream) override;

    friend std::ostream& operator<<(std::ostream &output, Size const &size);
    friend bool operator==(Size const& one, Size const& two);
    friend bool operator!=(Size const& one, Size const& two);

private:
    Dimension _width = 0;
    Dimension _height = 0;
};

std::ostream& operator<<(std::ostream &output, Size const &size);
bool operator==(Size const& one, Size const& two);
bool operator!=(Size const& one, Size const& two);