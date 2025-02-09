#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "core/general.h"

using ScoreContainer = std::uint64_t;

class Player;
using PlayerList = std::vector<Player>;

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

    friend std::ostream& operator<<(std::ostream &output, Player const &target);
    friend bool operator==(Player const &one, Player const &two);
    friend bool operator!=(Player const &one, Player const &two);

    static size_t Print(PlayerList const &players);

private:
    std::string     _name;
    ScoreContainer  _score = 0;
};

bool operator==(Player const &one, Player const &two);
bool operator!=(Player const &one, Player const &two);
std::ostream& operator<<(std::ostream &output, Player const &target);