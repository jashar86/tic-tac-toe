#ifndef GAME_APP_SESSION_HPP
#define GAME_APP_SESSION_HPP

/// \file Session.hpp
/// \brief Game session management
///
/// The Session class manages a game session including players,
/// scoreboard, and current game state. This is part of the
/// Application layer and depends on the Core layer.

#include <memory>

#include "application/Player.hpp"
#include "application/Scoreboard.hpp"
#include "core/GameState.hpp"

namespace game::app
{

/// \brief Manages a game session with two players
///
/// A session tracks the players, their scores across multiple games,
/// and the current game being played. The session maintains mutable state
/// for the game board and scoreboard, allowing games to progress and
/// scores to accumulate.
///
/// \note This class is NOT thread-safe. External synchronization is required
///       if accessed from multiple threads.
class Session
{
public:
    /// \brief Construct a session with two players
    /// \param player1 The first player (plays X)
    /// \param player2 The second player (plays O)
    Session(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2);

    ~Session() = default;

    // Non-copyable to prevent accidental sharing of session state
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    // Movable for transfer of ownership
    Session(Session&&) = default;
    Session& operator=(Session&&) = default;

    /// \brief Get the first player
    /// \return Shared pointer to player 1
    [[nodiscard]] std::shared_ptr<Player> getPlayer1() const;

    /// \brief Get the second player
    /// \return Shared pointer to player 2
    [[nodiscard]] std::shared_ptr<Player> getPlayer2() const;

    /// \brief Get the scoreboard
    /// \return Const reference to the scoreboard
    [[nodiscard]] const Scoreboard& getScoreboard() const;

    /// \brief Record a win for player 1 (X)
    void recordPlayer1Win();

    /// \brief Record a win for player 2 (O)
    void recordPlayer2Win();

    /// \brief Record a draw
    void recordDraw();

    /// \brief Get the current game state
    /// \return Const reference to the game state
    [[nodiscard]] const game::core::GameState& getGameState() const;

    /// \brief Set the current game state
    /// \param state The new game state
    void setGameState(const game::core::GameState& state);

    /// \brief Reset the game to initial state for a new game
    void resetGame();

    /// \brief Check if the session is active
    /// \return true if a game is currently in progress
    [[nodiscard]] bool isActive() const;

    /// \brief Get the current player based on whose turn it is
    /// \return Shared pointer to the current player
    [[nodiscard]] std::shared_ptr<Player> getCurrentPlayer() const;

private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
    Scoreboard scoreboard;
    game::core::GameState gameState;
};

} // namespace game::app

#endif // GAME_APP_SESSION_HPP
