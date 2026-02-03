#ifndef GAME_APP_CONTINUATION_RESULT_HPP
#define GAME_APP_CONTINUATION_RESULT_HPP

/// \file ContinuationResult.hpp
/// \brief Defines the ContinuationResult enum for stage transition control

namespace game::app
{

/// \brief Represents the result of a stage transition callback
///
/// Used by listeners (GameStartListener, GameFinishedListener) to signal
/// how the application should proceed after the current stage completes.
enum class ContinuationResult
{
    CONTINUE,  ///< Proceed to the next stage normally
    RESET,     ///< Reset back to the welcome stage
    QUIT       ///< Exit the application
};

} // namespace game::app

#endif // GAME_APP_CONTINUATION_RESULT_HPP
