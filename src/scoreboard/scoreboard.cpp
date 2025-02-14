#include "include/scoreboard/scoreboard.h"
#include "include/core/graphics.h"
#include <algorithm>
#include <fstream>
#include <cctype>

bool LessThan(RankedPlayer const &one, RankedPlayer const &two) {
    if(one._ratio < two._ratio) {
        return false;

    } else if(one._ratio > two._ratio) {
        return true;
    
    } else if(one.GetScore() < two.GetScore()) {
        return false;

    }
    return true;
}
void Scoreboard::Sort()
{
    std::sort(_container.begin(), _container.end(), LessThan);
}

std::filesystem::path Scoreboard::GetFilename() const
{
    const std::filesystem::path path = "saves/scoreboard.revf";
    std::filesystem::create_directories(path.parent_path());
    return path;
}

Scoreboard::Scoreboard()
{
    std::ifstream file(GetFilename().string(), std::ios::binary | std::ios::in);
    if(!file.is_open()) {
        return;
    }
    this->FromBinary(file);
}

void Scoreboard::Insert(RankedPlayer const &player)
{
    Index pos = Find(player.GetName());
    if(pos != -1) {
        RankedPlayer &dist = _container[pos];
        dist.SetScore(player.GetScore());
        dist.SetSize(player.GetSize());

    } else {
        _container.push_back(player);

    }
    this->Sort();
}

bool IsEqual(std::string const& one, std::string const& two) {
    if(one.size() != two.size()) {
        return false;
    }
    for(size_t i = 0; i < one.size(); ++i) {
        if(std::toupper(one.at(i)) != std::toupper(two.at(i))) {
            return false;
        }
    }
    return true;
}
Index Scoreboard::Find(std::string const &name) const
{
    for(auto i = _container.cbegin(); i != _container.cend(); ++i) {
        if(IsEqual(i->GetName(), name)) {
            return std::distance(_container.cbegin(), i);
        }
    }
    return -1;
}

bool Scoreboard::Remove(size_t pos)
{
    if(pos < _container.size()) {
        _container.erase(_container.begin() + pos);
        return true;
    }
    return false;
}

const RankedPlayerList& Scoreboard::Container() const
{
    return _container;
}

Scoreboard::~Scoreboard()
{
    this->Commit();
}

void Scoreboard::Commit()
{
    std::ofstream file(GetFilename().string(), std::ios::binary | std::ios::out | std::ios::trunc);
    if(!file.is_open()) {
        return;
    }
    this->Sort();
    this->ToBinary(file);
}

std::ostream &Scoreboard::ToBinary(std::ostream &stream) const
{
    const auto length = size_t { _container.size() };
    if(stream.write(reinterpret_cast<const char*>(&length), sizeof(length)).bad()) {
        return stream;
    }
    for(const RankedPlayer &player : _container) {
        if(player.ToBinary(stream).bad()) {
            return stream;
        }
    }
    return stream;
}
std::istream &Scoreboard::FromBinary(std::istream &stream)
{
    auto size = size_t {0};
    if(stream.read(reinterpret_cast<char*>(&size), sizeof(size)).bad()) {
        return stream;
    }
    for(auto i = size_t {0}; i < size; ++i) {
        RankedPlayer player;
        if(player.FromBinary(stream).bad()) {
            return stream;
        }
        _container.push_back(player);
    }
    return stream;
}

std::ostream& operator<<(std::ostream &output, const Scoreboard &board)
{
    // Tab space will be a determined value.
    const std::string tab = std::string(4, ' ');
    size_t seperator = 0;

    // Finds the longest name in the scoreboard.
    auto longest = std::max_element(board.Container().cbegin(), board.Container().cend(), 
        [](RankedPlayer const &one, RankedPlayer const &two) 
        {
            return one.GetName().size() < two.GetName().size();
        }
    );

    // The length of the longest name will be determined as a base.
    const auto length = longest->GetName().size();

    // Prints the rank as a part of the header.
    {
        const auto title = std::string {"Rank"};
        Graphics::SetForeground(Graphics::Color::BrightRed);
        output << title << '\t';
        seperator += title.size() + tab.size();
    }

    // Prints the name as a part of the header.
    {
        const auto title = std::string {"Name"};
        Graphics::SetForeground(Graphics::Color::BrightBlue);
        output << title << std::string(length - title.size(), ' ') << '\t';
        seperator += tab.size() + length;
    }

    // Prints the score as a part of the header.
    {
        const auto title = std::string {"Score"};
        Graphics::SetForeground(Graphics::Color::BrightCyan);
        output << title << '\t';
        seperator += title.size() + tab.size();
    }

    // Prints the ratio as a part of the header.
    {
        const auto title = std::string {"Ratio"};
        Graphics::SetForeground(Graphics::Color::BrightMagenta);
        output << title << '\n';
        seperator += title.size();
    }
    Graphics::SetForeground(_default);

    output << std::string(seperator, '=') << '\n';

    // Prints the list of ranked players.
    auto rank = 1;
    output.precision(2);
    for(const RankedPlayer &player : board.Container()) {
        Graphics::SetForeground(Graphics::Color::BrightRed);
        output << rank++ << '\t';

        Graphics::SetForeground(Graphics::Color::BrightBlue);
        output << player.GetName() << std::string(length - player.GetName().size(), ' ') << '\t';

        Graphics::SetForeground(Graphics::Color::BrightCyan);
        output << player.GetScore() << '\t';

        Graphics::SetForeground(Graphics::Color::BrightMagenta);
        output << player.GetRatio() << '\n';
    }

    Graphics::SetForeground(_default);
    output << std::string(seperator, '=') << '\n';

    return output;
}