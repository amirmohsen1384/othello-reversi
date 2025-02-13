#include <random>
#include <iostream>
#include "include/menu/menu.h"
#include "include/game/match.h"
#include "include/core/system.h"
#include "include/core/graphics.h"

void Match::UpdateState()
{
    if(MatchContinues() == false) {
        if(_user.GetScore() > _opponent.GetScore()) {
            _state = State::UserWon;
        }
        else if(_user.GetScore() < _opponent.GetScore()) {
            _state = State::OpponentWon;
        }
        else {
            _state = State::GameDraw;
        }
    }
    else {
        _state = State::Unspecified;
    }
}

void Match::UpdateScores()
{
    _user.SetScore(_panel.Occurrences(Piece::User));
    _opponent.SetScore(_panel.Occurrences(Piece::Opponent));
}

Match::Match(Type const &type)
{
    _type = type;
}

Match::Match(Match const &match)
{
    *this = match;
}

Match::Match(TurnInfo const &turn, Type const &type) : Match(type)
{
    _turn = turn;
}

Match::Match(Player const &user, Player const &opponent, Type const &type) : Match(type)
{
    _user = user;
    _opponent = opponent;
}

Match::Match(Dimension const &width, Dimension const &height, Type const &type) : Match(type)
{
    _panel.Reset(width, height);
}

Match::Match(Player const &user, Player const &opponent, TurnInfo const &turn, Type const &type) : Match(user, opponent, type)
{
    _turn = turn;
}

Match::Match(Dimension const &width, Dimension const &height, 
    Player const &user, Player const &opponent, 
    TurnInfo const &turn, Type const &type) : Match(type)
{
    _panel.Reset(width, height);
    _opponent = opponent;
    _user = user;
    _turn = turn;
}

bool Match::MatchContinues()
{
    bool result = true;
    PointList legals = _panel.GetLegals(_turn);
    if(legals.empty()) {
        ToggleTurn();
        legals = _panel.GetLegals(_turn);
        result = !legals.empty();
    }
    return result;
}

size_t Match::Occurrences(Piece const &target) const
{
    return _panel.Occurrences(target);
}

Match &Match::operator=(Match const &match)
{
    this->_panel = match._panel;
    this->_user = match._user;
    this->_opponent = match._opponent;
    this->_turn = match._turn;
    this->_state = match._state;
    this->_type = match._type;
    return *this;
}

Match &Match::operator!()
{
    this->ToggleTurn();
    return *this;
}

std::ostream &Match::ToBinary(std::ostream &stream) const
{
    if(_panel.ToBinary(stream).bad()) {
        return stream;
    }

    if(_user.ToBinary(stream).bad()) {
        return stream;
    }

    if(_opponent.ToBinary(stream).bad()) {
        return stream;
    }

    auto state = static_cast<int8_t>(_state);
    if(stream.write(reinterpret_cast<const char*>(&state), sizeof(state)).bad()) {
        return stream;
    }

    auto turn = static_cast<int8_t>(_turn);
    if(stream.write(reinterpret_cast<const char*>(&turn), sizeof(turn)).bad()) {
        return stream;
    }

    auto type = static_cast<int8_t>(_type);
    if(stream.write(reinterpret_cast<const char*>(&type), sizeof(type)).bad()) {
        return stream;
    }

    return stream;
}

std::istream &Match::FromBinary(std::istream &stream)
{
    if(_panel.FromBinary(stream).bad()) {
        return stream;
    }

    if(_user.FromBinary(stream).bad()) {
        return stream;
    }

    if(_opponent.FromBinary(stream).bad()) {
        return stream;
    }

    auto state = static_cast<int8_t>(0);
    if(stream.read(reinterpret_cast<char*>(&state), sizeof(state)).bad()) {
        return stream;
    }
    _state = static_cast<State>(state);

    auto turn = static_cast<int8_t>(0);
    if(stream.read(reinterpret_cast<char*>(&turn), sizeof(turn)).bad()) {
        return stream;
    }
    _turn = static_cast<TurnInfo>(turn);

    auto type = static_cast<int8_t>(0);
    if(stream.read(reinterpret_cast<char*>(&type), sizeof(type)).bad()) {
        return stream;
    }
    _type = static_cast<Type>(type);
    
    return stream;
}

void Match::ToggleTurn()
{
    if(_turn == Piece::User) {
        _turn = Piece::Opponent;
    }
    else if(_turn == Piece::Opponent) {
        _turn = Piece::User;
    }
}

TurnInfo Match::GetTurn() const
{
    return _turn;
}

void Match::PutPiece(const Point &point)
{
    if(!_panel.IsLegal(_turn, point)) {
        throw IllegalPointException();
    }
    _panel.At(point) = _turn;
    _panel.UpdateSurroundedPieces(point);
    UpdateScores();
    UpdateState();
}

Player Match::GetUser() const {
    return _user;
}

Player Match::GetOpponent() const {
    return _opponent;
}

void Match::SetUserName(std::string const &value)
{
    _user.SetName(value);
}

void Match::SetOpponentName(std::string const &value)
{
    _opponent.SetName(value);
}

Match::State Match::GetState() const
{
    return _state;
}

Match::Type Match::GetType() const
{
    return _type;
}

Board &Match::GetPanel()
{
    return _panel;
}
const Board &Match::GetPanel() const
{
    return _panel;
}

bool operator==(Match const &one, Match const &two) {
    if(one._panel != two._panel) {
        return false;
    }
    else if(one._user != two._user) {
        return false;
    }
    else if(one._opponent != two._opponent) {
        return false;
    }
    else if(one._turn != two._turn) {
        return false;
    }
    return true;
}
bool operator!=(Match const &one, Match const &two) {
    return !(one == two);
}

int GuessIndex(size_t min_index, size_t max_index) {
    if(min_index > max_index) {
        return -1;
    }
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distro(min_index, max_index);
    return distro(generator);
}

void Match::Execute()
{
    const PointList legals = _panel.GetLegals(_turn);
    if(_type == Type::SinglePlayer && _turn == Piece::Opponent) {
        int index = GuessIndex(0, legals.size() - 1);
        this->PutPiece(legals.at(index));
        this->ToggleTurn();
        return;
    }
    while(true) {
        std::cout << *this;
        switch(_turn) {
            case Piece::User: {
                Graphics::Draw(_user.GetName(), static_cast<Graphics::Color>(Graphics::Action::User));
                break;
            }
            case Piece::Opponent: {
                Graphics::Draw(_opponent.GetName(), static_cast<Graphics::Color>(Graphics::Action::Opponent));
                break;
            }
            default: {
                throw BlankPieceException();
            }
        }
        int index = 0;
        std::cout << ':' << ' ' << "Enter the number of location to continue: ";
        std::cin >> index;
        if(std::cin.bad()) {
            throw BadInputException();
        }
        else if(index < 1 || index > static_cast<int>(legals.size())) {
            if(index == -1) {
                throw SavegameException();

            } else {
                throw InvalidPointException();

            }
        }
        this->PutPiece(legals.at(index - 1));
        break;
    }
    this->ToggleTurn();
}

std::string GetName(std::string const &message) {
    std::string name;
    using namespace std;
    using namespace Graphics;
    while(true) {
        try {
            Draw(message, Color::Green);
            std::getline(std::cin, name);
            if (name.empty()) {
                System::EraseConsole();
                throw EmptyStringException();
            }
            break;
        }
        catch(EmptyStringException const &exception) {
            Draw("The player's name should be entered.\n", Color::Red);
        }
    }
    return name;
}

void Match::Initialize() {
    using namespace std;
    using namespace Graphics;

    Menu typeMessage;
    typeMessage.push_back("Single Player");
    typeMessage.push_back("Multi Player");
    typeMessage.SetTitle("How is the match going to be played?");

    _type = static_cast<Type>(typeMessage.Execute());
    System::EraseConsole();

    switch(_type) {
        case Match::Type::SinglePlayer: {
            _opponent.SetName("Computer");
            _user.SetName(GetName("Enter your name: "));
            System::EraseConsole();
            break;
        }
        case Match::Type::DoublePlayer: {
            _user.SetName(GetName("Enter the name of player 1: "));
            System::EraseConsole();

            _opponent.SetName(GetName("Enter the name of player 2: "));
            System::EraseConsole();
            break;
        }
    }

    Menu sizeMenu;
    sizeMenu.SetTitle("Select one of the following dimensions to start:");
    sizeMenu.SetOrientation(Orientation::Horizontal);
    sizeMenu.push_back("6 x 6");
    sizeMenu.push_back("8 x 8");
    sizeMenu.push_back("10 x 10");
    switch(sizeMenu.Execute()) {
        case 0: {
            _panel.Reset(6, 6);
            break;
        }
        case 1: {
            _panel.Reset(8, 8);
            break;
        }
        case 2: {
            _panel.Reset(10, 10);
            break;
        }
    }
    System::EraseConsole();

    Menu turnMenu;
    turnMenu.SetTitle("Who wants to go first in this match:");
    turnMenu.SetOrientation(Orientation::Vertical);
    turnMenu.push_back(_user.GetName() + " goes first.");
    turnMenu.push_back(_opponent.GetName() + " goes first.");
    if(turnMenu.Execute() == 1) {
        ToggleTurn();
    }
    System::EraseConsole();
}