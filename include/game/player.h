#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "core/interfaces.h"

using ScoreContainer = std::uint64_t;

class Player : public ISerialize {
public:
    Player();
    Player(Player const &another);
    Player(std::string const &name, ScoreContainer const &score = 0);

    Player& operator=(Player const &another);

    std::string GetName() const;
    std::uint64_t GetScore() const;

    void SetName(std::string const& value);
    void SetScore(std::uint64_t const& value);

    virtual std::istream& FromBinary(std::istream &stream) override;
    virtual std::ostream& ToBinary(std::ostream &stream) const override;

    friend bool operator==(Player const &one, Player const &two);
    friend bool operator!=(Player const &one, Player const &two);

private:
    std::string     _name;
    ScoreContainer  _score = 0;
};

using PlayerList = std::vector<Player>;

bool operator==(Player const &one, Player const &two);
bool operator!=(Player const &one, Player const &two);