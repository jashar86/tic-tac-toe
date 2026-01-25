#ifndef TICTACTOE_APPLICATION_SESSION_H
#define TICTACTOE_APPLICATION_SESSION_H

/**
 * @file Session.h
 * @brief Game session management
 *
 * The Session class manages a game session including players,
 * scoreboard, and current game state. This is part of the
 * Application layer and depends on the Domain layer.
 *
 * TODO: Implement full session logic including:
 * - Player management (player1, player2)
 * - Scoreboard tracking
 * - Current game state reference
 */

#include "domain/Board.h"

namespace tictactoe::application {

/**
 * @brief Manages a game session with two players
 *
 * A session tracks the players, their scores across multiple games,
 * and the current game being played.
 */
class Session {
public:
    Session();
    ~Session() = default;

    /**
     * @brief Check if the session is active
     * @return true if a game is currently in progress
     */
    bool isActive() const;
};

} // namespace tictactoe::application

#endif // TICTACTOE_APPLICATION_SESSION_H
