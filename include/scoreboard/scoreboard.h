#pragma once
#include <filesystem>
#include "include/scoreboard/player.h"

class Scoreboard : public ISerialize {
private:
    void Sort();
    std::filesystem::path GetFilename() const;

public:
    Scoreboard();

    void Insert(RankedPlayer const &player);
    Index Find(std::string const &name) const;
    bool Remove(size_t pos);

    const RankedPlayerList& Container() const;

    void Commit();
    ~Scoreboard();

    virtual std::istream& FromBinary(std::istream &stream) override;
    virtual std::ostream& ToBinary(std::ostream &stream) const override;

    friend std::ostream& operator<<(std::ostream &output, const Scoreboard &board);

private:
    RankedPlayerList _container;
};

std::ostream& operator<<(std::ostream &output, const Scoreboard &board);