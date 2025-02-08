#include "player.h"

Player::Player() {}

Player::Player(Player const &another)
{
    *this = another;
}

Player::Player(std::string const &name, ScoreContainer const &score) : _name(name), _score(score) {}

Player &Player::operator=(Player const &another)
{
    _name = another._name;
    _score = another._score;
    return *this;
}

std::string Player::GetName() const
{
    return _name;
}

std::uint64_t Player::GetScore() const
{
    return _score;
}

void Player::SetName(std::string const &value)
{
    _name = value;
}

void Player::SetScore(std::uint64_t const &value)
{
    _score = value;
}

std::istream &Player::FromBinary(std::istream &stream)
{
    if(ReadString(stream, _name).bad()) {
        return stream;
    }
    if(stream.read(reinterpret_cast<char*>(&_score), sizeof(_score)).bad()) {
        return stream;
    }
    return stream;
}

std::ostream &Player::ToBinary(std::ostream &stream) const
{
    if(WriteString(stream, _name).bad()) {
        return stream;
    }
    if(stream.write(reinterpret_cast<const char*>(&_score), sizeof(_score)).bad()) {
        return stream;
    }
    return stream;
}