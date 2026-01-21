# Tic-Tac-Toe Game: Reference Architecture

## High-Level Design

```mermaid
flowchart TB

    subgraph core [Domain Layer]
        direction TB
        rules[Game Rules<br/><small>Move validation - Win check]
        model[Data Model<br/><small>Game State - Board - Marker]
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
        X_TURN
        X_WINS
        O_TURN
        O_WINS
        DRAW
    }

    

    GameState *-- "1" Board
    GameState *-- "1" GameStatus

    Board ..> Position : uses
    Board ..> Marker : uses
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

```mermaid
classDiagram
class Session {
        -Player player1
        -Player player2
        -ScoreBoard scores
        -GameState current_game

        +getPlayer1() : Player
        +getPlayer2() : Player
        +getNumDraws() : int
        +startGame() : GameState
    }

    class Player {
        +String name
        +Agent agent
    }

    class ScoreBoard {
        +int player1_wins
        +int player2_wins
        +int draws
    }

    class Agent {
        <<interface>>
        + getNextMove(board: Board) : Position
    }


    Session *-- "1" GameState
    Session *-- "2" Player
    Session *-- ScoreBoard

    Player *-- Agent
```