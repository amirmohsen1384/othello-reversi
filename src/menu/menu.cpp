#include "core/graphics.h"
#include "core/system.h"
#include "menu/menu.h"
#include <sstream>
#include <iostream>
#include <algorithm>

MenuNavigator::MenuNavigator()
{
    _min = 0;
    _pos = 0;
    _orientation = Orientation::Vertical;
    _max = std::numeric_limits<Limit>::max();
}
MenuNavigator::MenuNavigator(Limit const& min, Limit const& max) : MenuNavigator()
{
    SetMinimum(min);
    SetMaximum(max);
}
MenuNavigator::MenuNavigator(Limit const& min, Limit const& max, Orientation const& orientation) : MenuNavigator(min, max)
{
    SetOrientation(orientation);
}

Limit MenuNavigator::GetMinimum() const
{
    return _min;
}
void MenuNavigator::SetMinimum(Limit const &value)
{
    _min = value;
    if(this->_pos < _min) {
        SetPosition(_min);
    }
}

Limit MenuNavigator::GetMaximum() const
{
    return _max;
}
void MenuNavigator::SetMaximum(Limit const &value)
{
    _max = value;
    if(this->_pos > _max) {
        SetPosition(_max);
    }
}

Index MenuNavigator::GetPosition() const
{
    return _pos;
}
bool MenuNavigator::SetPosition(Index const &value)
{
    if(value >= _min && value <= _max) {
        _pos = value;
        return true;
    } else {
        return false;
    }
}

Orientation MenuNavigator::GetOrientation() const
{
    return _orientation;
}
void MenuNavigator::SetOrientation(Orientation const &value)
{
    _orientation = value;
}

void MenuNavigator::MoveTop()
{
    if(_orientation == Orientation::Vertical) {
        Index pos = GetPosition();
        SetCurrentState(SetPosition(--pos) ? State::UpdateNavigator : State::InvalidKeyPressed);

    } else {
        SetCurrentState(State::InvalidKeyPressed);
    }
}

void MenuNavigator::MoveLeft()
{
    if(_orientation == Orientation::Horizontal) {
        Index pos = GetPosition();
        SetCurrentState(SetPosition(--pos) ? State::UpdateNavigator : State::InvalidKeyPressed);

    } else {
        SetCurrentState(State::InvalidKeyPressed);
    }
}

void MenuNavigator::MoveRight()
{
    if(_orientation == Orientation::Horizontal) {
        Index pos = GetPosition();
        SetCurrentState(SetPosition(++pos) ? State::UpdateNavigator : State::InvalidKeyPressed);
    
    } else {
        SetCurrentState(State::InvalidKeyPressed);
    }

}

void MenuNavigator::MoveBottom()
{
    if(_orientation == Orientation::Vertical) {
        Index pos = GetPosition();
        SetCurrentState(SetPosition(++pos) ? State::UpdateNavigator : State::InvalidKeyPressed);

    } else {
        SetCurrentState(State::InvalidKeyPressed);
    }
}

Orientation Menu::GetOrientation() const
{
    return _orientation;
}
void Menu::SetOrientation(Orientation const &value)
{
    _orientation = value;
}

std::string Menu::GetTitle() const
{
    return _title;
}
void Menu::SetTitle(std::string const &value)
{
    _title = value;
}

Index Menu::GetPosition() const
{
    return _position;
}
bool Menu::SetPosition(Index const &index)
{
    int64_t size = this->size();
    if(index >= 0 && index < size) {
        _position = index;
        return true;
    }
    return false;
}

size_t SeperatorLength(Menu const &menu) {
    using namespace std;
    if(menu.empty()) {
        return 0;
    }
    size_t i = 0, itemLength = 0;
    const Orientation orientation = menu.GetOrientation();
    const size_t titleLength = menu.GetTitle().size();
    switch(orientation) {
        case Orientation::Horizontal: {
            stringstream stream;
            for(auto s : menu) {
                stream << ++i << ')';
                stream << ' ';
                stream << s;
                stream << '\t';
            }
            const string &result = stream.str();
            itemLength = result.size();
            break;
        }
        case Orientation::Vertical: {
            for(auto s : menu) {
                stringstream stream;
                stream << ++i << ')';
                stream << ' ';
                stream << s;
                const string &target = stream.str();
                if(itemLength < target.size()) {
                    itemLength = target.size();
                }
            }
            break;
        }
    }
    return std::max(itemLength, titleLength);
} 

void DrawSeperator(size_t count) {
    using namespace std;
    const char symbol = '=';
    for(size_t i = 0; i < count; ++i) {
        cout << symbol;
    }
    cout << endl;
}

// Prints a menu on the screen.
void Menu::PrintMenu() const
{
    using namespace Graphics;
    using namespace std;

    // Measures the count of seperators.
    const size_t seperatorCount = SeperatorLength(*this);

    // Draws the description.
    Draw(this->_title, Color::Magenta);
    cout << endl;
    
    // Draws a seperator.
    DrawSeperator(seperatorCount);

    // Starts printing the items.
    int64_t i = 0;
    for(auto s : *this) {
        if(i == _position) {
            SetForeground(Color::Green);
        }
        cout << ++i << ')';
        cout << ' ';
        cout << s;
        SetForeground(_default);
        if(_orientation == Orientation::Horizontal) {
            cout << '\t';
        } else {
            cout << '\n';
        }
    }
    if(_orientation == Orientation::Horizontal) {
        cout << endl;
    }
    // Draws a seperator.
    DrawSeperator(seperatorCount);
}

int Menu::Execute()
{
    if(this->empty()) {
        return EXECUTION_FAILED;
    }

    MenuNavigator navigator;
    navigator.SetOrientation(this->_orientation);
    navigator.SetMaximum(this->size() - 1);
    navigator.SetMinimum(0);

    State state = navigator.GetCurrentState();
    System::EraseConsole();
    PrintMenu();

    while(state != State::Accepted && state != State::Rejected) {
        state = navigator.Execute();
        if(state == State::UpdateNavigator) {
            Index pos = navigator.GetPosition();
            this->SetPosition(pos);
            System::EraseConsole();
            PrintMenu();
        }
    }

    return state == State::Accepted ? this->GetPosition() : EXECUTION_FAILED;
}