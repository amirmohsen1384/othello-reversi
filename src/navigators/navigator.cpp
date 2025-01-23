#include "navigator.h"
#include <windows.h>

System::KeyNumber AbstractNavigator::GetKeyBottom() const {
    return VK_DOWN;
}

System::KeyNumber AbstractNavigator::GetKeyTop() const {
    return VK_UP;
}

System::KeyNumber AbstractNavigator::GetKeyLeft() const {
    return VK_LEFT;
}

System::KeyNumber AbstractNavigator::GetKeyRight() const {
    return VK_RIGHT;
}

System::KeyNumber AbstractNavigator::GetKeyAccept() const {
    return VK_ACCEPT;
}

System::KeyNumber AbstractNavigator::GetKeyReject() const {
    return VK_ESCAPE;
}

void AbstractNavigator::Accept() {
    this->SetCurrentState(State::Successful);
}

void AbstractNavigator::Reject() {
    this->SetCurrentState(State::Failed);
}

System::KeyNumber AbstractNavigator::GetCurrentKey() const {
    return _currentKey;
}

State AbstractNavigator::GetCurrentState() const {
    return _currentState;
}

void AbstractNavigator::SetCurrentState(State const& state) {
    _currentState = state;
}

State AbstractNavigator::Execute() {
    // Waits for the user for an instant key.
    _currentKey = System::InstantKey();

    // Checks the entered instant key.
    if(_currentKey == this->GetKeyTop()) {
        MoveTop();
    }
    else if(_currentKey == this->GetKeyLeft()) {
        MoveLeft();
    }
    else if(_currentKey == this->GetKeyReject()) {
        Reject();
    }
    else if(_currentKey == this->GetKeyRight()) {
        MoveRight();
    }
    else if(_currentKey == this->GetKeyAccept()) {
        Accept();
    }
    else if(_currentKey == this->GetKeyBottom()) {
        MoveBottom();
    }
}