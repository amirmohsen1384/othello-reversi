#pragma once
#include "include/core/size.h"
#include "include/game/player.h"

class RankedPlayer;
using RankedPlayerList = std::vector<RankedPlayer>;

class RankedPlayer : public Player {
private:
    void UpdateRatio();

public:
    RankedPlayer();
    RankedPlayer(RankedPlayer const &player);
    RankedPlayer(std::string const &name, ScoreContainer const &score = 0, Size const &size = Size(6, 6));

    RankedPlayer& operator=(RankedPlayer const &player);

    Size GetSize() const;
    float GetRatio() const;
    
    virtual void SetSize(Size const &size);
    virtual void SetScore(std::uint64_t const& value);

    virtual std::ostream& ToBinary(std::ostream &stream) const override;
    virtual std::istream& FromBinary(std::istream &stream) override;

    friend bool operator==(RankedPlayer const &one, RankedPlayer const &two);
    friend bool operator!=(RankedPlayer const &one, RankedPlayer const &two);

private:
    float _ratio = 0.00;
    Size _size = Size(6, 6);
};