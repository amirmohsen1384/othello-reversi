#include "include/scoreboard/scoreboard.h"
#include "include/core/graphics.h"
#include <algorithm>
#include <fstream>

bool LessThan(RankedPlayer const &one, RankedPlayer const &two) {
    if(one._ratio < two._ratio) {
        return true;

    } else if(one._ratio > two._ratio) {
        return false;
    
    } else if(one.GetScore() < two.GetScore()) {
        return true;

    }
    return false;
}
void Scoreboard::Sort()
{
    std::sort(begin(), end(), LessThan);
}

std::filesystem::path Scoreboard::GetFilename() const
{
    const std::filesystem::path path = "saves/scoreboard.revf";
    std::filesystem::create_directories(path.parent_path());
    return path;
}

Scoreboard::Scoreboard() : RankedPlayerList()
{
    std::ifstream file(GetFilename().string());
    if(!file.is_open()) {
        return;
    }
    this->FromBinary(file);
}

Scoreboard::~Scoreboard()
{
    this->Commit();
}

void Scoreboard::Commit()
{
    std::ofstream file(GetFilename().string());
    if(!file.is_open()) {
        return;
    }
    this->Sort();
    this->ToBinary(file);
}

std::ostream &Scoreboard::ToBinary(std::ostream &stream) const
{
    const auto size = size_t { size() };
    if(stream.write(reinterpret_cast<const char*>(&size), sizeof(size)).bad()) {
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
    const auto size = size_t {0};
    if(stream.read(reinterpret_cast<char*>(&size), sizeof(size)).bad()) {
        return stream;
    }
    resize(size);
    while(!stream.eof()) {
        RankedPlayer player;
        if(player.FromBinary(stream).bad()) {
            return stream;
        }
        push_back(player);
    }
    return stream;
}

std::ostream& operator<<(std::ostream &output, const Scoreboard &board)
{
    // Finds the longest name in the scoreboard.
    auto longest = std::max_element(board.cbegin(), board.cend(), [](RankedPlayer const &one, RankedPlayer const &two) 
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
    }

    // Prints the name as a part of the header.
    {
        const auto title = std::string {"Name"};
        Graphics::SetForeground(Graphics::Color::BrightBlue);
        output << title << std::string(' ', length - title.size()) << '\t';
    }

    // Prints the score as a part of the header.
    {
        const auto title = std::string {"Score"};
        Graphics::SetForeground(Graphics::Color::BrightCyan);
        output << title << '\t';
    }

    // Prints the ratio as a part of the header.
    {
        const auto title = std::string {"Ratio"};
        Graphics::SetForeground(Graphics::Color::BrightMagenta);
        output << title <<;
    }
    Graphics::SetForeground(_default);

    Graphics::DrawSeperator(output, 32);

    // Prints the list of ranked players.
    auto rank = 1;
    for(const RankedPlayer &player : board) {
        Graphics::SetForeground(Graphics::Color::BrightRed);
        output << rank++ << '\t';

        Graphics::SetForeground(Graphics::Color::BrightBlue);
        output << player.GetName() << std::string(' ', length - title.size()) << '\t';

        Graphics::SetForeground(Graphics::Color::BrightCyan);
        output << player.GetScore() << '\t';

        Graphics::SetForeground(Graphics::Color::BrightMagenta);
        output << player.GetRatio() << '\n';
    }
    
    Graphics::SetForeground(_default);

    Graphics::DrawSeperator(output, 32);
    return output;
}