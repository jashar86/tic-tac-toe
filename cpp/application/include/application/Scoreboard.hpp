#ifndef GAME_APP_SCOREBOARD_HPP
#define GAME_APP_SCOREBOARD_HPP

/// \file Scoreboard.hpp
/// \brief Tracks game statistics across a session

namespace game::app
{

/// \brief Tracks wins and draws across multiple games in a session
///
/// The Scoreboard maintains counts of wins for each player and draws.
/// It provides methods to record outcomes and query statistics.
class Scoreboard
{
public:
    Scoreboard();
    ~Scoreboard() = default;

    /// \brief Get the number of player 1 wins
    /// \return The player 1 win count
    int getPlayer1Wins() const;

    /// \brief Get the number of player 2 wins
    /// \return The player 2 win count
    int getPlayer2Wins() const;

    /// \brief Get the number of draws
    /// \return The draw count
    int getDraws() const;

    /// \brief Get the total number of games played
    /// \return Sum of wins and draws
    int getTotalGames() const;

    /// \brief Record a win for player 1
    void recordPlayer1Win();

    /// \brief Record a win for player 2
    void recordPlayer2Win();

    /// \brief Record a draw
    void recordDraw();

    /// \brief Reset all counters to zero
    void reset();

private:
    int player1Wins;
    int player2Wins;
    int draws;
};

} // namespace game::app

#endif // GAME_APP_SCOREBOARD_HPP
