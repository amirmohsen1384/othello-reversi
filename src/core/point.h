#pragma once

#include <vector>
#include <cstdint>
#include "interfaces.h"

using Unit = int64_t;

class Point : public ISerialize
{
private:
    Unit _x = 0;
    Unit _y = 0;

private:
    friend Point operator+(Point const &one, Point const &two); 
    friend Point operator-(Point const &one, Point const &two);

    friend bool operator==(Point const &one, Point const &two);
    friend bool operator!=(Point const &one, Point const &two);
    friend std::ostream& operator<<(std::ostream &output, Point const& point);

public:
    Point();
    Point(Point const& _object);
    Point(Unit const& _x, Unit const& _y);

    Point& operator=(Point const& _object);

    Unit GetX() const;
    Unit& ReferX();

    Unit GetY() const;
    Unit& ReferY();

    void Traspose();
    void SetX(Unit const& _x);
    void SetY(Unit const& _y);

    void MoveTop();
    void MoveTop(Unit const& _factor);

    void MoveLeft();
    void MoveLeft(Unit const& _factor);

    void MoveRight();
    void MoveRight(Unit const& _factor);

    void MoveBottom();
    void MoveBottom(Unit const& _factor);

    void MoveTopLeft();
    void MoveTopLeft(Unit const& _factor);

    void MoveTopRight();
    void MoveTopRight(Unit const& _factor);

    void MoveBottomLeft();
    void MoveBottomLeft(Unit const& _factor);

    void MoveBottomRight();
    void MoveBottomRight(Unit const& _factor);

    Point& operator+=(Point const& point);
    Point& operator-=(Point const& point);

public:
    static Point Transposed(Point const &point);
    virtual std::ostream& ToBinary(std::ostream &stream) override;
    virtual std::istream& FromBinary(std::istream &stream) override;
};

Point operator+(Point const &one, Point const &two); 
Point operator-(Point const &one, Point const &two);

bool operator==(Point const &one, Point const &two);
bool operator!=(Point const &one, Point const &two);
std::ostream& operator<<(std::ostream &output, Point const& point);

using PointList = std::vector<Point>;