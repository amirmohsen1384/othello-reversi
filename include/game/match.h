#pragma once

#include "board.h"
#include "player.h"
#include <exception>

class IllegalPointException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "The selected point is not legal!";
    }
};

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

private:
    void PrintPanel() const;
    void UpdateState();
    void UpdateScores();

public:
    Match();
    Match(Match const &match);
    Match(TurnInfo const &turn);
    Match(Player const &user, Player const &opponent);
    Match(Dimension const &width, Dimension const &height);
    Match(Player const &user, Player const &opponent, TurnInfo const &turn);
    Match(Dimension const &width, Dimension const &height, Player const &user, Player const &opponent, TurnInfo const &turn);

    Match& operator=(Match const &match);
    Match& operator!();

    virtual std::ostream& ToBinary(std::ostream &stream) const override;
    virtual std::istream& FromBinary(std::istream &stream) override;

    void ToggleTurn();
    TurnInfo GetTurn() const;
    
    void PutPiece(const Point &point);

    std::string GetUserName() const;
    ScoreContainer GetUserScore() const;

    std::string GetOpponentName() const;
    ScoreContainer GetOpponentScore() const;

    void SetUserName(std::string const &value);
    void SetOpponentName(std::string const &value);

    Match::State GetState() const;

    void ResizePanel(Dimension const &width, Dimension const &height);
    void ResizePanel(Size const &size);
    Size GetPanelSize() const;
    bool IsPanelEmpty() const;

    bool MatchContinues();
    size_t Occurrences(Piece const& target) const;

    friend std::ostream& operator<<(std::ostream &stream, Match const &target);
    friend bool operator==(Match const &one, Match const &two);
    friend bool operator!=(Match const &one, Match const &two);
    
private:
    Board           _panel;
    Player          _user;
    Player          _opponent;
    TurnInfo        _turn = Piece::User;
    Match::State    _state = State::Unspecified;
};

std::ostream& operator<<(std::ostream &stream, Match const &target);
bool operator==(Match const &one, Match const &two);
bool operator!=(Match const &one, Match const &two);