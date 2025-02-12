#include "include/game/match.h"

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
    if(stream.write(reinterpret_cast<const char*>(&state), sizeof(state))) {
        return stream;
    }

    auto turn = static_cast<int8_t>(_turn);
    if(stream.write(reinterpret_cast<const char*>(&turn), sizeof(turn))) {
        return stream;
    }

    auto type = static_cast<int8_t>(_type);
    if(stream.write(reinterpret_cast<const char*>(&type), sizeof(type))) {
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
    if(stream.read(reinterpret_cast<char*>(&state), sizeof(state))) {
        return stream;
    }
    _state = static_cast<State>(state);

    auto turn = static_cast<int8_t>(0);
    if(stream.read(reinterpret_cast<char*>(&turn), sizeof(turn))) {
        return stream;
    }
    _turn = static_cast<TurnInfo>(turn);

    auto type = static_cast<int8_t>(0);
    if(stream.read(reinterpret_cast<char*>(&type), sizeof(type))) {
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
