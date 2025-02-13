#pragma once
#include <filesystem>
#include "include/scoreboard/player.h"

class Scoreboard : public ISerialize, RankedPlayerList {
private:
    void Sort();
    std::filesystem::path GetFilename() const;

public:
    Scoreboard();

    void Commit();
    ~Scoreboard();

    friend std::ostream& operator<<(std::ostream &output, const Scoreboard &board);

    virtual std::istream& FromBinary(std::istream &stream) override;
    virtual std::ostream& ToBinary(std::ostream &stream) const override;
};

std::ostream& operator<<(std::ostream &output, const Scoreboard &board);