#pragma once
#include "core/system.h"

// Describes different states of a single navigator
enum class State {
    Unknown = 0,
    Accepted = Unknown + 3,
    Rejected = Unknown + 1,
    UpdateNavigator = Unknown + 2,
    InvalidKeyPressed = Unknown - 1
};

// Provides an interface for handling arrow keys.
class AbstractNavigator {
private:
    State _currentState = State::Unknown;
    System::KeyNumber _currentKey = 0;

protected:
    virtual System::KeyNumber GetKeyAccept() const;
    virtual System::KeyNumber GetKeyReject() const;
    virtual System::KeyNumber GetKeyBottom() const;
    virtual System::KeyNumber GetKeyRight() const;
    virtual System::KeyNumber GetKeyLeft() const;
    virtual System::KeyNumber GetKeyTop() const;

public:
    virtual void MoveBottom() = 0;
    virtual void MoveRight() = 0;
    virtual void MoveLeft() = 0;
    virtual void MoveTop() = 0;
    virtual void Accept();
    virtual void Reject();

public:
    void ResetCurrentState();
    State GetCurrentState() const;
    void SetCurrentState(State const& state);

    virtual State Execute();
    System::KeyNumber GetCurrentKey() const;
};