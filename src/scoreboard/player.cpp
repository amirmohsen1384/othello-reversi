#include "include/scoreboard/player.h"
#include "player.h"

std::ostream &RankedPlayer::ToBinary(std::ostream &stream) const
{
    if(Player::ToBinary(stream).bad()) {
        return stream;
    }
    if(stream.write(reinterpret_cast<const char*>(&_ratio), sizeof(_ratio)).bad()) {
        return stream;
    }
    return stream;
}

std::istream& RankedPlayer::FromBinary(std::istream &stream)
{
    if(Player::FromBinary(stream).bad()) {
        return stream;
    }
    if(stream.read(reinterpret_cast<char*>(&_ratio), sizeof(_ratio)).bad()) {
        return stream;
    }
    return stream;
}

void RankedPlayer::UpdateRatio()
{
    float score = GetScore();
    float area = _size.GetArea();
    _ratio = score / area;
}

RankedPlayer::RankedPlayer() : Player()
{
}

RankedPlayer::RankedPlayer(RankedPlayer const &player)
{
    *this = player;
}

RankedPlayer::RankedPlayer(std::string const &name, ScoreContainer const &score, Size const &size) : Player(name, score)
{
}

Size RankedPlayer::GetSize() const
{
    return _size;
}

void RankedPlayer::SetSize(Size const &size)
{
    _size = size;
    UpdateRatio();
}
void RankedPlayer::SetScore(std::uint64_t const &value)
{
    Player::SetScore(value);
    UpdateRatio();
}

RankedPlayer& RankedPlayer::operator=(RankedPlayer const &another)
{
    static_cast<RankedPlayer>(*this) = static_cast<RankedPlayer>(another);
    this->_ratio = another._ratio;
    this->_size = another._size;
    return *this;
}

bool operator==(RankedPlayer const &one, RankedPlayer const &two)
{
    if(static_cast<RankedPlayer>(one) != static_cast<RankedPlayer>(two)) {
        return false;
    }
    else if(one._size != two._size) {
        return false;
    }
    return true;
}

bool operator!=(RankedPlayer const &one, RankedPlayer const &two)
{
    return !(one == two);
}
