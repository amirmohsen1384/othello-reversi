#pragma once

#include "include/core/general.h"
#include "navigator.h"
#include <vector>
#include <string>
#include <limits>

// Describes the direction of items.
enum class Orientation {Horizontal, Vertical};

// Provides an interface for navigating between item through arrow keys.
class MenuNavigator : public AbstractNavigator
{
    Limit _min = 0;
    Index _pos = 0;
    bool _modal = true;
    Limit _max = std::numeric_limits<Limit>::max();
    Orientation _orientation = Orientation::Vertical;

protected:
    virtual System::KeyNumber GetKeyReject() const;

public:
    MenuNavigator();
    MenuNavigator(Limit const& min, Limit const& max);
    MenuNavigator(Limit const& min, Limit const& max, Orientation const& orientation);

public:
    Limit GetMinimum() const;
    void SetMinimum(Limit const& value);

    Limit GetMaximum() const;
    void SetMaximum(Limit const& value);

    Index GetPosition() const;
    bool SetPosition(Index const& value);

    Orientation GetOrientation() const;
    void SetOrientation(Orientation const& value);

    bool IsModal() const;
    void SetModal(bool value);

public:
    virtual void MoveTop() override;
    virtual void MoveLeft() override;
    virtual void MoveRight() override;
    virtual void MoveBottom() override;
};

#define EXECUTION_FAILED -1

// Defines a list of strings.
using StringList = std::vector<std::string>;

// Describes a menu of items
class Menu : public StringList {
private:
    Orientation     _orientation = Orientation::Vertical;
    Index           _position = 0;
    std::string     _title;
    
protected:
    virtual void PrintMenu() const;
    friend size_t SeperatorLength(Menu const &menu);
    
public:
    // Obtains the current orientation of items.
    Orientation GetOrientation() const;

    // Changes the current orientation of items.
    void SetOrientation(Orientation const& value);

    // Obtains the title of the menu.
    std::string GetTitle() const;

    // Changes the title of the menu.
    void SetTitle(std::string const& value);

    // Obtains the position of the last selected item.
    Index GetPosition() const;

    // Changes the current item in the menu.
    bool SetPosition(Index const& index);

    // Shows up a menu containing some items.
    int Execute();
};