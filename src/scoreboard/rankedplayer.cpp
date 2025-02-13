#include "include/scoreboard/rankedplayer.h"

std::ostream& RankedPlayer::ToBinary(std::ostream &stream) const
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
