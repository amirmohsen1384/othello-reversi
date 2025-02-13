#pragma once
#include "include/game/player.h"

class RankedPlayer : public Player {
public:
    virtual std::ostream& ToBinary(std::ostream &stream) const override;
    virtual std::istream& FromBinary(std::istream &stream) override;

private:
    float _ratio = 0.00;
};