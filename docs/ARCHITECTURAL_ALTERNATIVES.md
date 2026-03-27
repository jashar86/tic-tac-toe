# Architectural Alternatives Analysis

## Overview

This document explores different architectural approaches for the tic-tac-toe core system, comparing trade-offs and providing multiple perspectives for consideration.

---

## Approach 1: Object-Oriented with Mutable State (Traditional)

### Description
Classic OOP approach with mutable objects managing their own state.

### Code Example

```typescript
class Game {
  private board: Board
  private currentPlayer: Marker
  private status: GameStatus

  constructor() {
    this.board = new Board()
    this.currentPlayer = Marker.X
    this.status = GameStatus.IN_PROGRESS
  }

  makeMove(position: Position): void {
    if (this.status !== GameStatus.IN_PROGRESS) {
      throw new Error('Game is finished')
    }

    if (!this.board.isEmpty(position)) {
      throw new Error('Cell is occupied')
    }

    this.board.setCell(position, this.currentPlayer)
    this.updateStatus()
    this.switchPlayer()
  }

  private updateStatus(): void {
    const winner = this.checkWinner()
    if (winner) {
      this.status = winner === Marker.X ? GameStatus.X_WINS : GameStatus.O_WINS
    } else if (this.board.isFull()) {
      this.status = GameStatus.DRAW
    }
  }
}
```

### Pros
- Familiar to most developers
- Intuitive state encapsulation
- Direct state mutation is efficient
- Easy to understand for simple cases

### Cons
- Hard to debug (state changes over time)
- Difficult to implement undo/redo
- Race conditions in async/concurrent scenarios
- Hard to test (need to set up state)
- Temporal coupling (order of method calls matters)
- Side effects make reasoning difficult

### Best For
- Simple applications
- Single-threaded environments
- When performance is absolutely critical
- Teams familiar with traditional OOP

---

## Approach 2: Functional with Immutable State (Recommended)

### Description
Immutable data structures with pure functions for transformations.

### Code Example

```typescript
interface GameState {
  readonly board: Board
  readonly currentPlayer: Marker
  readonly status: GameStatus
  readonly moveHistory: readonly Move[]
}

// Pure functions - no side effects
const GameLogic = {
  createInitialState(): GameState {
    return {
      board: Board.empty(),
      currentPlayer: Marker.X,
      status: GameStatus.IN_PROGRESS,
      moveHistory: []
    }
  },

  makeMove(state: GameState, position: Position): GameState {
    // Validation
    if (state.status !== GameStatus.IN_PROGRESS) {
      throw new Error('Game is finished')
    }

    // Create new state (no mutation)
    const newBoard = state.board.withMove(position, state.currentPlayer)
    const winner = GameRules.checkWinner(newBoard)
    const newStatus = winner
      ? (winner === Marker.X ? GameStatus.X_WINS : GameStatus.O_WINS)
      : newBoard.isFull() ? GameStatus.DRAW : GameStatus.IN_PROGRESS

    return {
      board: newBoard,
      currentPlayer: state.currentPlayer === Marker.X ? Marker.O : Marker.X,
      status: newStatus,
      moveHistory: [...state.moveHistory, { position, marker: state.currentPlayer }]
    }
  },

  undo(state: GameState): GameState {
    if (state.moveHistory.length === 0) {
      return state
    }

    // Rebuild state by replaying moves
    const newHistory = state.moveHistory.slice(0, -1)
    return newHistory.reduce(
      (s, move) => GameLogic.makeMove(s, move.position),
      GameLogic.createInitialState()
    )
  }
}
```

### Pros
- Easy to test (pure functions)
- Easy to reason about (no side effects)
- Time travel / undo/redo is trivial
- Thread-safe by default
- Easier debugging (state is explicit)
- Can serialize/deserialize state easily
- Predictable behavior

### Cons
- More memory allocations (creating new objects)
- Slightly less intuitive for OOP developers
- Can be verbose
- May have performance overhead (usually negligible)

### Best For
- Complex state management
- Applications requiring undo/redo
- Testing-heavy projects
- Teams familiar with functional programming
- **Recommended for this project**

---

## Approach 3: Event Sourcing

### Description
Store events instead of state. Rebuild state by replaying events.

### Code Example

```typescript
type GameEvent =
  | { type: 'GAME_STARTED'; timestamp: number }
  | { type: 'MOVE_MADE'; position: Position; marker: Marker; timestamp: number }
  | { type: 'GAME_FINISHED'; result: GameResult; timestamp: number }

class EventStore {
  private events: GameEvent[] = []

  append(event: GameEvent): void {
    this.events.push(event)
  }

  getEvents(): readonly GameEvent[] {
    return this.events
  }
}

class GameProjection {
  static buildState(events: readonly GameEvent[]): GameState {
    let state = GameLogic.createInitialState()

    for (const event of events) {
      switch (event.type) {
        case 'GAME_STARTED':
          state = GameLogic.createInitialState()
          break
        case 'MOVE_MADE':
          state = GameLogic.makeMove(state, event.position)
          break
      }
    }

    return state
  }
}

// Usage
const eventStore = new EventStore()
eventStore.append({ type: 'GAME_STARTED', timestamp: Date.now() })
eventStore.append({ type: 'MOVE_MADE', position: new Position(0), marker: Marker.X, timestamp: Date.now() })

const currentState = GameProjection.buildState(eventStore.getEvents())
```

### Pros
- Complete audit trail
- Perfect replay capability
- Easy to implement complex undo/redo
- Can build multiple projections from same events
- Debugging is easier (see exactly what happened)
- Temporal queries (what was state at time T?)

### Cons
- More complex
- Rebuilding state can be slow for long event streams
- Requires snapshots for performance
- More storage required
- Learning curve

### Best For
- Applications requiring audit trails
- Complex undo/redo requirements
- Debugging/analytics needs
- Multi-user scenarios with conflict resolution
- **Overkill for tic-tac-toe, but interesting for learning**

---

## Approach 4: Entity-Component-System (ECS)

### Description
Separate data (components) from behavior (systems). Used heavily in game engines.

### Code Example

```typescript
// Components - just data
interface PositionComponent {
  row: number
  col: number
}

interface MarkerComponent {
  marker: Marker
}

interface CellEntity {
  id: number
  position: PositionComponent
  marker?: MarkerComponent
}

// Systems - behavior
class WinCheckSystem {
  check(cells: CellEntity[]): Marker | null {
    // Check win conditions
  }
}

class MoveSystem {
  applyMove(cells: CellEntity[], cellId: number, marker: Marker): CellEntity[] {
    return cells.map(cell =>
      cell.id === cellId ? { ...cell, marker: { marker } } : cell
    )
  }
}

// World - manages entities and systems
class GameWorld {
  private cells: CellEntity[] = []
  private winCheckSystem = new WinCheckSystem()
  private moveSystem = new MoveSystem()

  constructor() {
    for (let i = 0; i < 9; i++) {
      this.cells.push({
        id: i,
        position: { row: Math.floor(i / 3), col: i % 3 }
      })
    }
  }

  makeMove(cellId: number, marker: Marker): void {
    this.cells = this.moveSystem.applyMove(this.cells, cellId, marker)
  }

  checkWinner(): Marker | null {
    return this.winCheckSystem.check(this.cells)
  }
}
```

### Pros
- Highly performant (data-oriented design)
- Excellent for complex simulations
- Easy to add new behaviors
- Good for parallel processing
- Cache-friendly

### Cons
- Overkill for simple games
- Steep learning curve
- Less intuitive for business logic
- Harder to debug

### Best For
- Complex game engines
- High-performance requirements
- Many entities with shared behaviors
- **Not recommended for tic-tac-toe (overkill)**

---

## Approach 5: State Machine Pattern

### Description
Explicit state machine for game phases.

### Code Example

```typescript
// States
interface GameStateNode {
  name: string
  canMakeMove(): boolean
  canStart(): boolean
  canReset(): boolean
  onEnter?(): void
  onExit?(): void
}

class NotStartedState implements GameStateNode {
  name = 'NOT_STARTED'
  canMakeMove() { return false }
  canStart() { return true }
  canReset() { return false }
}

class InProgressState implements GameStateNode {
  name = 'IN_PROGRESS'
  canMakeMove() { return true }
  canStart() { return false }
  canReset() { return true }
}

class FinishedState implements GameStateNode {
  name = 'FINISHED'
  constructor(public result: GameResult) {}
  canMakeMove() { return false }
  canStart() { return false }
  canReset() { return true }
}

// State machine
class GameStateMachine {
  private currentState: GameStateNode = new NotStartedState()

  transition(newState: GameStateNode): void {
    this.currentState.onExit?.()
    this.currentState = newState
    this.currentState.onEnter?.()
  }

  canMakeMove(): boolean {
    return this.currentState.canMakeMove()
  }

  getCurrentState(): GameStateNode {
    return this.currentState
  }
}

// Usage
class Game {
  private stateMachine = new GameStateMachine()
  private board = new Board()

  start(): void {
    if (this.stateMachine.getCurrentState().canStart()) {
      this.stateMachine.transition(new InProgressState())
    }
  }

  makeMove(position: Position, marker: Marker): void {
    if (!this.stateMachine.canMakeMove()) {
      throw new Error('Cannot make move in current state')
    }

    this.board.setCell(position, marker)

    const winner = this.checkWinner()
    if (winner) {
      this.stateMachine.transition(new FinishedState(
        winner === Marker.X ? GameResult.X_WINS : GameResult.O_WINS
      ))
    }
  }
}
```

### Pros
- Explicit state transitions
- Prevents invalid operations
- Self-documenting
- Easy to visualize
- Clear separation of concerns

### Cons
- Can be verbose
- Might be overkill for simple state
- Learning curve

### Best For
- Complex state transitions
- When invalid states must be prevented
- When state diagram is complex
- **Good fit for game phase management**

---

## Approach 6: Hexagonal Architecture (Ports & Adapters)

### Description
Core domain logic isolated from external dependencies.

### Code Structure

```
core/
  domain/
    Game.ts          # Pure domain logic
    Board.ts         # Domain model
    Player.ts        # Domain model
  ports/
    GameRepository.ts    # Interface
    DisplayPort.ts       # Interface
    InputPort.ts         # Interface
  services/
    GameService.ts       # Application logic

adapters/
  primary/              # Drive the application
    CliAdapter.ts
    WebAdapter.ts
  secondary/            # Driven by application
    InMemoryGameRepository.ts
    FileGameRepository.ts
    ConsoleDisplay.ts
    WebDisplay.ts
```

### Code Example

```typescript
// Core domain - no dependencies on external world
class Game {
  private board: Board
  private status: GameStatus

  makeMove(position: Position, marker: Marker): MoveResult {
    // Pure domain logic
  }
}

// Port - interface for external dependency
interface GameRepository {
  save(game: Game): Promise<void>
  load(id: string): Promise<Game>
}

interface DisplayPort {
  showBoard(board: Board): void
  showMessage(message: string): void
}

// Adapter - implements the port
class ConsoleDisplay implements DisplayPort {
  showBoard(board: Board): void {
    console.log(board.toString())
  }

  showMessage(message: string): void {
    console.log(message)
  }
}

class WebDisplay implements DisplayPort {
  showBoard(board: Board): void {
    document.getElementById('board').innerHTML = board.toHtml()
  }

  showMessage(message: string): void {
    document.getElementById('message').textContent = message
  }
}

// Application service - orchestrates domain with ports
class GameService {
  constructor(
    private repository: GameRepository,
    private display: DisplayPort
  ) {}

  async playMove(gameId: string, position: Position): Promise<void> {
    const game = await this.repository.load(gameId)
    const result = game.makeMove(position, /* marker */)

    if (result.success) {
      await this.repository.save(game)
      this.display.showBoard(game.getBoard())
    } else {
      this.display.showMessage(result.error)
    }
  }
}
```

### Pros
- Core domain is completely isolated
- Easy to swap adapters (CLI → Web → Mobile)
- Testable (can mock ports)
- Clear dependency direction
- Business logic independent of frameworks

### Cons
- More files and abstractions
- Can feel over-engineered for small projects
- Learning curve

### Best For
- Applications with multiple interfaces
- Long-term maintainability
- Large teams
- **Good fit if you want CLI, Web, and Mobile versions**

---

## Approach 7: CQRS (Command Query Responsibility Segregation)

### Description
Separate reads (queries) from writes (commands).

### Code Example

```typescript
// Commands - write operations
interface Command {
  execute(): void
}

class MakeMoveCommand implements Command {
  constructor(
    private game: Game,
    private position: Position,
    private marker: Marker
  ) {}

  execute(): void {
    this.game.applyMove(this.position, this.marker)
  }
}

class StartGameCommand implements Command {
  constructor(private game: Game) {}

  execute(): void {
    this.game.start()
  }
}

// Queries - read operations
interface Query<T> {
  execute(): T
}

class GetBoardQuery implements Query<Board> {
  constructor(private game: Game) {}

  execute(): Board {
    return this.game.getBoard()
  }
}

class GetValidMovesQuery implements Query<Position[]> {
  constructor(private game: Game) {}

  execute(): Position[] {
    return this.game.getValidMoves()
  }
}

// Command/Query handlers
class CommandBus {
  dispatch(command: Command): void {
    command.execute()
  }
}

class QueryBus {
  dispatch<T>(query: Query<T>): T {
    return query.execute()
  }
}

// Usage
const commandBus = new CommandBus()
const queryBus = new QueryBus()

commandBus.dispatch(new MakeMoveCommand(game, position, Marker.X))
const board = queryBus.dispatch(new GetBoardQuery(game))
```

### Pros
- Clear separation of reads and writes
- Can optimize separately
- Easy to add middleware (logging, validation, etc.)
- Explicit operations
- Good for complex domains

### Cons
- More boilerplate
- Overkill for simple apps
- Can be confusing

### Best For
- Complex domains
- Performance optimization needed
- Audit/logging requirements
- **Probably overkill for tic-tac-toe**

---

## Comparison Matrix

| Approach | Complexity | Testability | Performance | Maintainability | Learning Curve | Best Use Case |
|----------|-----------|-------------|-------------|-----------------|----------------|---------------|
| **OOP Mutable** | Low | Medium | High | Medium | Low | Simple apps |
| **Functional Immutable** ⭐ | Medium | High | Medium | High | Medium | Most apps |
| **Event Sourcing** | High | High | Medium | High | High | Audit needs |
| **ECS** | High | Medium | Very High | Medium | High | Game engines |
| **State Machine** | Medium | High | Medium | High | Medium | Complex states |
| **Hexagonal** | Medium-High | High | Medium | Very High | Medium-High | Multi-platform |
| **CQRS** | High | High | High | Medium | High | Complex domains |

⭐ = Recommended for this project

---

## Recommended Hybrid Approach

For the tic-tac-toe project, I recommend a **hybrid approach**:

### Core Architecture

```typescript
// 1. Functional immutable core (Approach 2)
interface GameState {
  readonly board: Board
  readonly phase: GamePhase
  readonly result: GameResult
  readonly currentTurn: Marker
  readonly moveHistory: readonly Move[]
}

const GameLogic = {
  applyMove(state: GameState, position: Position): GameState {
    // Pure function
  }
}

// 2. State machine for game phases (Approach 5)
class GamePhaseManager {
  private state: GameStateNode

  transition(event: GameEvent): void {
    // Handle transitions
  }
}

// 3. Ports & Adapters for display (Approach 6)
interface DisplayPort {
  showBoard(board: Board): void
  showMessage(message: string): void
  getPlayerMove(): Promise<Position>
}

// 4. Controller to orchestrate
class GameController {
  constructor(
    private phaseManager: GamePhaseManager,
    private display: DisplayPort
  ) {}

  async makeMove(position: Position): Promise<MoveResult> {
    // Orchestrate the pure logic with side effects
  }
}
```

### Why This Hybrid?

1. **Functional core** - Easy to test, reason about, and debug
2. **State machine** - Clear phase transitions, prevents invalid operations
3. **Ports & Adapters** - Display system completely decoupled
4. **Controller** - Orchestrates everything, manages side effects

### Directory Structure

```
src/
  core/
    domain/
      Board.ts              # Immutable board
      Position.ts           # Value object
      GameState.ts          # Immutable state
      Marker.ts             # Enum
    logic/
      GameLogic.ts          # Pure functions
      GameRules.ts          # Validation & win checking
      MoveValidator.ts      # Move validation
      WinConditionChecker.ts
    state/
      GamePhaseManager.ts   # State machine
      GamePhase.ts          # State definitions

  application/
    GameController.ts       # Orchestrator
    SessionManager.ts       # Multi-game session

  ports/
    DisplayPort.ts          # Interface
    InputPort.ts            # Interface
    StoragePort.ts          # Interface

  adapters/
    ConsoleDisplay.ts       # CLI implementation
    WebDisplay.ts           # Web implementation

  agents/
    Agent.ts                # Interface
    HumanAgent.ts
    RandomAI.ts
    MinimaxAI.ts
```

---

## Decision Matrix for Your Project

### Question 1: Do you need undo/redo?
- **Yes** → Functional Immutable (Approach 2)
- **No** → Could use OOP Mutable (Approach 1)

### Question 2: Will you have multiple display types (CLI, Web, Mobile)?
- **Yes** → Hexagonal Architecture (Approach 6)
- **No** → Simpler architecture is fine

### Question 3: Do you need audit trails / game replay?
- **Yes** → Event Sourcing (Approach 3)
- **No** → Skip it

### Question 4: Is state management complex?
- **Yes** → State Machine (Approach 5)
- **No** → Simple enums are fine

### Question 5: Is performance critical?
- **Yes** → Profile first, then optimize (maybe ECS for large scale)
- **No** → Focus on maintainability

### For Your Tic-Tac-Toe Project:

**Recommended:**
- ✅ Functional Immutable core (Approach 2)
- ✅ State Machine for phases (Approach 5)
- ✅ Ports & Adapters for display (Approach 6)
- ✅ Strategy pattern for AI (already in design)

**Optional:**
- ⚠️ Event Sourcing (nice for learning, but overkill)
- ⚠️ CQRS (overkill)

**Skip:**
- ❌ ECS (massive overkill)

---

## Real-World Examples

### React/Redux (Functional Immutable)
```typescript
// Redux uses immutable state + pure reducers
function gameReducer(state: GameState, action: Action): GameState {
  switch (action.type) {
    case 'MAKE_MOVE':
      return {
        ...state,
        board: applyMove(state.board, action.position)
      }
    default:
      return state
  }
}
```

### XState (State Machine)
```typescript
// XState for explicit state machines
import { createMachine } from 'xstate'

const gameMachine = createMachine({
  initial: 'notStarted',
  states: {
    notStarted: {
      on: { START: 'inProgress' }
    },
    inProgress: {
      on: {
        MAKE_MOVE: { actions: 'applyMove' },
        WIN: 'finished',
        DRAW: 'finished'
      }
    },
    finished: {
      on: { RESET: 'notStarted' }
    }
  }
})
```

### Clean Architecture (Hexagonal)
```typescript
// Uncle Bob's Clean Architecture
// Domain → Use Cases → Interface Adapters → Frameworks
```

---

## Final Recommendation

**Use Functional Immutable Core + Ports & Adapters**

```typescript
// core/domain - Pure, immutable, testable
export class Board { /* immutable */ }
export const GameLogic = { /* pure functions */ }
export const GameRules = { /* validation */ }

// core/application - Orchestration
export class GameController {
  constructor(private display: DisplayPort) {}
}

// ports - Interfaces
export interface DisplayPort { }

// adapters - Implementations
export class ConsoleDisplay implements DisplayPort { }
export class WebDisplay implements DisplayPort { }
```

This gives you:
- ✅ Clean, testable core
- ✅ Easy undo/redo
- ✅ Swappable display systems
- ✅ SOLID principles
- ✅ Maintainable long-term
- ✅ Not over-engineered
- ✅ Industry best practices

---

## Further Reading

1. **Functional Programming**:
   - "Domain Modeling Made Functional" by Scott Wlaschin
   - Redux documentation

2. **State Machines**:
   - XState documentation
   - "Designing Event-Driven Systems" by Ben Stopford

3. **Hexagonal Architecture**:
   - Alistair Cockburn's original article
   - "Get Your Hands Dirty on Clean Architecture" by Tom Hombergs

4. **Event Sourcing**:
   - "Versioning in an Event Sourced System" by Greg Young
   - Event Store documentation

5. **SOLID Principles**:
   - "Clean Architecture" by Robert C. Martin
   - "Agile Software Development" by Robert C. Martin
