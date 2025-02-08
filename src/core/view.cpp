#include "core/graphics.h"
#include "core/point.h"
#include "core/size.h"

std::ostream& operator<<(std::ostream &output, Point const& point)
{
    output << '(' << point._x << ", " << point._y << ')';
    return output;
}

std::ostream& operator<<(std::ostream &output, Size const &size)
{
    output << '(' << size._width << ", " << size._height << ')';
    return output;
}