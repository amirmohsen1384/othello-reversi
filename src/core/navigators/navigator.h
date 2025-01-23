#pragma once
#include "../system.h"

// Describes different states of a single navigator
enum class State {
    Successful,
    Pending,
    Failed
};

// Provides an interface for handling arrow keys.
class AbstractNavigator {
private:
    State _currentState = State::Pending;
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
    State GetCurrentState() const;
    void SetCurrentState(State const& state);

    virtual State Execute();
    System::KeyNumber GetCurrentKey() const;
};