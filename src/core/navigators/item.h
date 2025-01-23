#pragma once
#include <limits>
#include "navigator.h"

// Describes the direction of items.
enum class Direction {Horizontal, Vertical};

// Defines some synonyms for the common types of the class.
using Limit = int64_t;
using Index = int64_t;

// Provides an interface for navigating between item through arrow keys.
class ItemNavigator : public AbstractNavigator
{
    Limit _min = 0;
    Index _pos = 0;
    Direction _direction = Direction::Vertical;
    Limit _max = std::numeric_limits<Limit>::max();

public:
    ItemNavigator() {}
    ItemNavigator(Direction const& direction);
    ItemNavigator(Limit const& min, Limit const& max);
    ItemNavigator(Limit const& min, Limit const& max, Direction const& direction);

public:
    Limit GetMinimum() const;
    void SetMinimum(Limit const& value);

    Limit GetMaximum() const;
    void SetMaximum(Limit const& value);

    Index GetPosition() const;
    void SetPosition(Index const& value);

    Direction GetDirection() const;
    void SetDirection(Direction const& value);

public:
    virtual void MoveTop() override;
    virtual void MoveLeft() override;
    virtual void MoveRight() override;
    virtual void MoveBottom() override;
};