#pragma once

#include "board.h"
#include "player.h"
#include <exception>

// Describes the current turn in the match.
using TurnInfo = Piece;

// Describes a complete reversi match.
class Match : public ISerialize {
public:
    enum class State {
        Unspecified = -1,
        OpponentWon,
        GameDraw,
        UserWon
    };
    enum class Type {SinglePlayer = 0, DoublePlayer};

private:
    void PrintPanel() const;
    void UpdateState();
    void UpdateScores();

public:
    Match(Match const &match);
    Match(Type const &type = Type::DoublePlayer);
    Match(TurnInfo const &turn, Type const &type = Type::DoublePlayer);
    Match(Player const &user, Player const &opponent, Type const &type = Type::DoublePlayer);
    Match(Dimension const &width, Dimension const &height, Type const &type = Type::DoublePlayer);
    Match(Player const &user, Player const &opponent, TurnInfo const &turn, Type const &type = Type::DoublePlayer);
    Match(Dimension const &width, Dimension const &height, Player const &user, Player const &opponent, TurnInfo const &turn, Type const &type = Type::DoublePlayer);

    Match& operator=(Match const &match);
    Match& operator!();

    virtual std::ostream& ToBinary(std::ostream &stream) const override;
    virtual std::istream& FromBinary(std::istream &stream) override;

    void ToggleTurn();
    TurnInfo GetTurn() const;
    
    void PutPiece(const Point &point);

    Player GetUser() const;
    Player GetOpponent() const;

    void SetUserName(std::string const &value);
    void SetOpponentName(std::string const &value);

    Match::State GetState() const;
    Match::Type GetType() const;

    Board& GetPanel();
    const Board& GetPanel() const;

    bool MatchContinues();
    size_t Occurrences(Piece const& target) const;

    void Execute();
    void Narrate();

    friend std::ostream& operator<<(std::ostream &stream, Match const &target);
    friend bool operator==(Match const &one, Match const &two);
    friend bool operator!=(Match const &one, Match const &two);
    
private:
    Board           _panel;
    Player          _user;
    Player          _opponent;
    TurnInfo        _turn = Piece::User;
    Match::Type     _type = Type::DoublePlayer;
    Match::State    _state = State::Unspecified;
};

std::ostream& operator<<(std::ostream &stream, Match const &target);
bool operator==(Match const &one, Match const &two);
bool operator!=(Match const &one, Match const &two);