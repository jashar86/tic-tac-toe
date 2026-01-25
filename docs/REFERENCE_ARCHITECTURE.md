# Tic-Tac-Toe Game: Reference Architecture

## High-Level Design

```mermaid
flowchart TB

    subgraph core [Domain Layer]
        direction TB
        rules[Game Rules<br/><small>Move validation - Win check]
        model[Data Model<br/><small>Game State - Board - Marker]
        agent[Agents<br/><small>Random - MinMax - Learned]
    end
    subgraph app[Application Layer]
        direction TB
        session[Session Management]
        stage[Application Stages<br/><small>Welcome - Game - Result</small> ]
    end
    subgraph gui[Presentation Layer]
        direction TB
        views[Stage Views]
    end

    gui --> app
    app --> core

```

## Domain Layer Design

### Data Model

```mermaid
classDiagram

    

    class GameState {
        +Board board
        +Marker turn
        +GameStatus status
    }

    class Board {
        -Marker[3][3] cells
        +Board()
        +setMarker(position: Position, marker: Marker): bool
        +getMarker(position: Position): Marker
    }

    class Position {
        -index
        +Position(index: int)
        +Position(row: int, col: int)
        +int getRow()
        +int getCol()
        +int getIndex()
    }

    class Marker {
        <<enumeration>>
        X
        O
    }

    class GameStatus {
        <<enumeration>>
        IN_PROGRESS
        X_WINS
        O_WINS
        DRAW
    }

    

    GameState *-- "1" Board
    GameState *-- "1" GameStatus

    Board ..> Position : uses
    Board ..> Marker : uses
```

### Agent Classes

```mermaid
classDiagram

    class Agent {
        <<interface>>
        + calculateNextMove(board: Board, marker: Marker) Position
    }

    class RandomAgent 
    class MinmaxAgent
    class LearnedAgent

    Agent <|-- RandomAgent
    Agent <|-- MinmaxAgent
    Agent <|-- LearnedAgent
```

### Game Logic

The logic for a single turn of the game can be summarized as follows:

```python
def takeTurn(state: GameState, position: Position, marker: Marker) -> MoveResult :

    if isValid (state.board, position, marker):
        new_board = state.board.withMove(position, marker)
        new_status = checkStatus(new_board)
        return GameState(new_board, new_status)
    else:
        return Error("Invalid move")
```

### Game Rules

The rules of the game consist of move validation and win/draw checking. The rules are as follows:

```python
def isValidMove(board: Board, position: Position, marker: Marker) -> bool:
    return board.isPositionWithinBounds(position) and board.isEmpty(position)
```

```python
def checkGameStatus(board: Board) -> GameStatus: 
    if hasThreeInARow(Marker::X):
        return GameStatus::X_WINS
    elif hasThreeInARow(Marker::O):
        return GameStatus::O_WINS
    elif board.isFull():
        return GameStatus::DRAW
    elif board.count(Marker::X) > board.count(Marker::O):
        return GameStatus::O_TURN
    else
        return GameStatus::X_TURN
```

## Application Layer Design

### Stage Transition Diagram

The tic-tac-toe application consists of switching between five stages:
* **Welcome** - Sets up the players for the game (name, agent logic, etc.). Allows quitting or starting a game.
* **Game Start** - Entered whenever a new game starts. Transitions to the first player's turn.
* **Player 1 Turn** - Entered whenever it is the first player's turn to make a move. Allows the user to quit or choose a board position.
* **Player 2 Turn** - Entered whenever it is the second player's turn to make a move. Allows the user to quit or choose a board position.
* **Game Finished** - Entered whenever a game has finished. Allows the user to quit, play again, or restart.  


```mermaid
stateDiagram-v2

    welcome: Welcome
    start: Game Start
    turn1: Player 1 Turn
    turn2: Player 2 Turn
    end: Game Finished

    [*] --> welcome
    welcome --> start : start
    welcome --> [*] : quit
    start --> turn1 
    turn1 --> turn2 : no winner
    turn1 --> end : player 1 wins or draw
    turn2 --> turn1 : no winner
    turn2 --> end : player 2 wins or draw
    end --> [*] : quit
    end --> start : play again
    end --> welcome : restart
```

### Application Class Diagram

```mermaid
classDiagram

    class TicTacToeApp {
        + TicTacToeApp(player_generator: PlayerGenerator, game_start: GameStartListener, game_finished: GameFinishedListener)
        + run()
    }

    class Session {
        +Player player1
        +Player player2
        +Scoreboard scoreboard
        +GameState game
    }

    class Scoreboard {
        +int num_player1_wins
        +int num_player2_wins
        +int num_draws
    }

    class SessionGenerator {
        <<interface>>
        +startNewSession() Result~Session,Quit~
    }

    class Player {
        <<interface>>
        + getName() String
        + generateNextMove(board: GameBoard, maker: Marker) Result~Position,Quit~
    }

    class GameStartListener {
        <<interface>>
        +onGameStarted() ContinueResult
    }

    class GameFinishedListener {
        <<interface>>
        + onGameEnded(scoreboard: Scoreboard)
    }

    TicTacToeApp *-- SessionGenerator
    TicTacToeApp *-- GameStartListener
    TicTacToeApp *-- GameFinishedListener

    SessionGenerator --> Session : generates
    Session *-- "2" Player
    Session *-- Scoreboard
    Session *-- GameState
```

