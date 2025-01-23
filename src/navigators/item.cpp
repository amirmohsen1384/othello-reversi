#include "item.h"

ItemNavigator::ItemNavigator() {}

ItemNavigator::ItemNavigator(Direction const& direction)
{
    SetDirection(direction);
}

ItemNavigator::ItemNavigator(Limit const& min, Limit const& max)
{
    SetMinimum(min);
    SetMaximum(max);
}

ItemNavigator::ItemNavigator(Limit const& min, Limit const& max, Direction const& direction)
{
    SetMinimum(min);
    SetMaximum(max);
    SetDirection(direction);
}

Limit ItemNavigator::GetMinimum() const
{
    return _min;
}

void ItemNavigator::SetMinimum(Limit const &value)
{
    _min = value;
}

Limit ItemNavigator::GetMaximum() const
{
    return _max;
}

void ItemNavigator::SetMaximum(Limit const &value)
{
    _max = value;
}

Index ItemNavigator::GetPosition() const
{
    return _pos;
}

void ItemNavigator::SetPosition(Index const &value)
{
    if(value >= _min && value <= _max) {
        _pos = value;
    }
}

Direction ItemNavigator::GetDirection() const
{
    return _direction;
}

void ItemNavigator::SetDirection(Direction const &value)
{
    _direction = value;
}

void ItemNavigator::MoveTop()
{
    if(_direction == Direction::Vertical) {
        Index pos = GetPosition();
        SetPosition(--pos);
    }
}

void ItemNavigator::MoveLeft()
{
    if(_direction == Direction::Horizontal) {
        Index pos = GetPosition();
        SetPosition(--pos);
    }
}

void ItemNavigator::MoveRight()
{
    if(_direction == Direction::Horizontal) {
        Index pos = GetPosition();
        SetPosition(++pos);
    }
}

void ItemNavigator::MoveBottom()
{
    if(_direction == Direction::Vertical) {
        Index pos = GetPosition();
        SetPosition(++pos);
    }
}