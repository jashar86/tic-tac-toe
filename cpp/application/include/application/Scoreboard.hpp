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
///
/// \note This class is NOT thread-safe. External synchronization is required
///       if accessed from multiple threads.
class Scoreboard
{
public:
    Scoreboard() noexcept = default;
    ~Scoreboard() = default;

    /// \brief Get the number of player 1 wins
    /// \return The player 1 win count
    [[nodiscard]] int getPlayer1Wins() const noexcept;

    /// \brief Get the number of player 2 wins
    /// \return The player 2 win count
    [[nodiscard]] int getPlayer2Wins() const noexcept;

    /// \brief Get the number of draws
    /// \return The draw count
    [[nodiscard]] int getDraws() const noexcept;

    /// \brief Get the total number of games played
    /// \return Sum of wins and draws
    [[nodiscard]] int getTotalGames() const noexcept;

    /// \brief Record a win for player 1
    void recordPlayer1Win() noexcept;

    /// \brief Record a win for player 2
    void recordPlayer2Win() noexcept;

    /// \brief Record a draw
    void recordDraw() noexcept;

    /// \brief Reset all counters to zero
    void reset() noexcept;

private:
    int player1Wins{0};
    int player2Wins{0};
    int draws{0};
};

} // namespace game::app

#endif // GAME_APP_SCOREBOARD_HPP
