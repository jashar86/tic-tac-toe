# Core Architecture Design Review & Improvement Recommendations

## Executive Summary

This document provides a comprehensive analysis of the tic-tac-toe core architecture design with specific recommendations for improvement based on SOLID principles, design patterns, and industry best practices.

**Critical Priority Issues:** 7 major design flaws requiring immediate attention
**Design Pattern Opportunities:** 10+ applicable patterns identified
**Recommended Refactoring:** Medium-scale architectural improvements

---

## Critical Design Issues

### 1. **CRITICAL: Game Class Violates Single Responsibility Principle**

**Problem:** The `Game.applyMove(position: Position)` method likely handles:
- Move validation (is position valid? is cell empty? is it the right player's turn?)
- Move application (updating the board)
- Win condition checking (did this move win the game?)
- Game status updates (updating from X_TURN to O_TURN or X_WINS)
- Turn management

**Impact:**
- Hard to test each concern independently
- Changes to one aspect affect others
- Difficult to extend or modify

**Recommendation:** Extract responsibilities into separate classes:
```typescript
class MoveValidator {
  validate(position: Position, board: Board, currentTurn: Marker): ValidationResult
}

class WinConditionChecker {
  checkWinner(board: Board): Marker | null
  checkDraw(board: Board, movesCount: number): boolean
}

class GameController {
  applyMove(position: Position): MoveResult // Orchestrates the above
}
```

### 2. **CRITICAL: GameStatus Enum Mixes Orthogonal Concerns**

**Problem:** GameStatus combines:
- Turn state (X_TURN, O_TURN)
- Game outcome (X_WINS, O_WINS, DRAW)

**Why This Is Bad:**
```
Current: X_TURN | O_TURN | X_WINS | O_WINS | DRAW

Question: How do you represent "Game is over, X won, and it was O's turn when X won"?
Answer: You can't, because turn and outcome are conflated.
```

**Recommendation:** Separate concerns:
```typescript
enum GamePhase {
  NOT_STARTED,
  IN_PROGRESS,
  FINISHED
}

enum GameResult {
  NOT_DETERMINED,
  X_WINS,
  O_WINS,
  DRAW
}

enum Marker {
  X,
  O
}

class GameState {
  readonly phase: GamePhase
  readonly result: GameResult
  readonly currentTurn: Marker
  readonly moveCount: number
}
```

### 3. **CRITICAL: No Clear Separation Between Game State and Game Logic**

**Problem:** Game state (data) and game logic (behavior) are entangled.

**Recommendation:** Use **Immutable State + Pure Functions** pattern:

```typescript
// Immutable state - just data
interface GameState {
  readonly board: Board
  readonly currentTurn: Marker
  readonly phase: GamePhase
  readonly result: GameResult
  readonly moveHistory: readonly Move[]
}

// Pure logic - functions that transform state
class GameLogic {
  static applyMove(state: GameState, move: Move): GameState {
    // Returns NEW state, doesn't mutate
  }

  static getNextTurn(current: Marker): Marker {
    return current === Marker.X ? Marker.O : Marker.X
  }
}

// Rules - validation and checking
class GameRules {
  static isValidMove(position: Position, board: Board): boolean
  static getWinner(board: Board): Marker | null
  static isDraw(board: Board, moveCount: number): boolean
}
```

**Benefits:**
- Easier to test (pure functions)
- Easier to reason about (no side effects)
- Enables time travel / undo-redo
- Thread-safe
- Easier to serialize/deserialize

### 4. **CRITICAL: Board Bit Manipulation Is Premature Optimization**

**Problem:** Using `uint16 x_marks` and `uint16 o_marks` for a 3x3 board.

**Analysis:**
```
Pros:
- Memory efficient (2 bytes vs 9 bytes for array)
- Fast bitwise operations

Cons:
- Harder to understand and maintain
- Error-prone bit manipulation logic
- Harder to debug (can't easily inspect state)
- Harder to test
- Not significantly faster for 3x3 grid
- Doesn't extend to larger boards
- Optimization before profiling = premature
```

**Recommendation:** Start with clear, simple design:

```typescript
class Board {
  private cells: ReadonlyArray<Marker | null>  // Length 9

  constructor(cells?: ReadonlyArray<Marker | null>) {
    this.cells = cells ?? new Array(9).fill(null)
  }

  getCell(position: Position): Marker | null {
    return this.cells[position.getIndex()]
  }

  // Immutable update - returns new board
  withMove(position: Position, marker: Marker): Board {
    const newCells = [...this.cells]
    newCells[position.getIndex()] = marker
    return new Board(newCells)
  }

  isEmpty(position: Position): boolean {
    return this.cells[position.getIndex()] === null
  }

  isFull(): boolean {
    return this.cells.every(cell => cell !== null)
  }
}
```

**Later Optimization:** If profiling shows board operations are a bottleneck, add bit manipulation as an internal implementation detail while keeping the interface clean.

### 5. **CRITICAL: Missing Move Abstraction**

**Problem:** Moves are represented only as `Position`. This loses important information.

**Recommendation:** Introduce Move as a first-class concept:

```typescript
interface Move {
  readonly position: Position
  readonly marker: Marker
  readonly timestamp?: number
  readonly moveNumber?: number
}

class MoveHistory {
  private moves: readonly Move[] = []

  add(move: Move): MoveHistory {
    return new MoveHistory([...this.moves, move])
  }

  get lastMove(): Move | null {
    return this.moves[this.moves.length - 1] ?? null
  }

  get all(): readonly Move[] {
    return this.moves
  }

  get count(): number {
    return this.moves.length
  }
}
```

**Benefits:**
- Can track move history
- Can implement undo/redo
- Can replay games
- Better debugging
- Can analyze AI decision-making

### 6. **CRITICAL: No Error Handling Strategy**

**Problem:** No defined error types or result types.

**Questions:**
- What if Agent returns an invalid position?
- What if a player tries to move when it's not their turn?
- What if someone tries to move on an occupied cell?

**Recommendation:** Use Result/Error types:

```typescript
enum MoveError {
  INVALID_POSITION = "Position is out of bounds",
  CELL_OCCUPIED = "Cell is already occupied",
  WRONG_TURN = "It's not your turn",
  GAME_NOT_IN_PROGRESS = "Game is not in progress",
  GAME_ALREADY_FINISHED = "Game has already finished"
}

type MoveResult =
  | { success: true; newState: GameState }
  | { success: false; error: MoveError }

class GameController {
  applyMove(position: Position): MoveResult {
    // Validate
    const validation = this.validator.validate(position, this.state)
    if (!validation.isValid) {
      return { success: false, error: validation.error }
    }

    // Apply
    const newState = GameLogic.applyMove(this.state, { position, marker: this.state.currentTurn })
    return { success: true, newState }
  }
}
```

### 7. **CRITICAL: Unclear Responsibility for Win Checking**

**Problem:** Who checks if the game is won? Where does this logic live?

**Recommendation:** Extract into dedicated class:

```typescript
class WinConditionChecker {
  private readonly WIN_PATTERNS = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8], // Rows
    [0, 3, 6], [1, 4, 7], [2, 5, 8], // Columns
    [0, 4, 8], [2, 4, 6]              // Diagonals
  ]

  checkWinner(board: Board): Marker | null {
    for (const pattern of this.WIN_PATTERNS) {
      const [a, b, c] = pattern
      const cells = [
        board.getCell(new Position(a)),
        board.getCell(new Position(b)),
        board.getCell(new Position(c))
      ]

      if (cells[0] !== null && cells[0] === cells[1] && cells[1] === cells[2]) {
        return cells[0]
      }
    }
    return null
  }

  checkDraw(board: Board, moveCount: number): boolean {
    return moveCount === 9 && this.checkWinner(board) === null
  }
}
```

**Benefits:**
- Single responsibility
- Easy to test
- Easy to extend (different board sizes, different win conditions)
- Reusable across different game implementations

---

## Design Pattern Opportunities

### 1. **State Pattern - For Game Phase Management**

**Use Case:** Different game phases have different valid operations.

```typescript
interface GamePhaseState {
  canMove(): boolean
  canStart(): boolean
  canReset(): boolean
  processMove(position: Position): MoveResult
}

class NotStartedState implements GamePhaseState {
  canMove(): boolean { return false }
  canStart(): boolean { return true }
  canReset(): boolean { return false }
  processMove(): MoveResult {
    return { success: false, error: MoveError.GAME_NOT_IN_PROGRESS }
  }
}

class InProgressState implements GamePhaseState {
  canMove(): boolean { return true }
  canStart(): boolean { return false }
  canReset(): boolean { return true }
  processMove(position: Position): MoveResult {
    // Actual move logic
  }
}

class FinishedState implements GamePhaseState {
  canMove(): boolean { return false }
  canStart(): boolean { return false }
  canReset(): boolean { return true }
  processMove(): MoveResult {
    return { success: false, error: MoveError.GAME_ALREADY_FINISHED }
  }
}
```

**Benefits:**
- Eliminates complex if/else chains
- Each state is self-contained
- Easy to add new states
- Impossible to call invalid operations

### 2. **Command Pattern - For Move History and Undo**

**Use Case:** Enable undo/redo, move replay, game analysis.

```typescript
interface Command {
  execute(): GameState
  undo(): GameState
}

class MakeMove implements Command {
  constructor(
    private readonly previousState: GameState,
    private readonly move: Move
  ) {}

  execute(): GameState {
    return GameLogic.applyMove(this.previousState, this.move)
  }

  undo(): GameState {
    return this.previousState
  }
}

class CommandHistory {
  private history: Command[] = []
  private currentIndex = -1

  execute(command: Command): GameState {
    const newState = command.execute()
    this.history = this.history.slice(0, this.currentIndex + 1)
    this.history.push(command)
    this.currentIndex++
    return newState
  }

  undo(): GameState | null {
    if (this.currentIndex < 0) return null
    const command = this.history[this.currentIndex]
    this.currentIndex--
    return command.undo()
  }

  redo(): GameState | null {
    if (this.currentIndex >= this.history.length - 1) return null
    this.currentIndex++
    return this.history[this.currentIndex].execute()
  }
}
```

### 3. **Observer Pattern - For Decoupled Event Notifications**

**Use Case:** UI updates, statistics tracking, logging, AI analysis.

```typescript
enum GameEventType {
  MOVE_MADE = 'MOVE_MADE',
  TURN_CHANGED = 'TURN_CHANGED',
  GAME_FINISHED = 'GAME_FINISHED',
  GAME_RESET = 'GAME_RESET'
}

interface GameEvent {
  type: GameEventType
  timestamp: number
  data: any
}

interface GameObserver {
  onGameEvent(event: GameEvent): void
}

class GameEventEmitter {
  private observers: GameObserver[] = []

  subscribe(observer: GameObserver): () => void {
    this.observers.push(observer)
    return () => {
      this.observers = this.observers.filter(o => o !== observer)
    }
  }

  emit(event: GameEvent): void {
    this.observers.forEach(observer => observer.onGameEvent(event))
  }
}

// Example usage
class GameController {
  private eventEmitter = new GameEventEmitter()

  applyMove(position: Position): MoveResult {
    const result = // ... apply move logic

    if (result.success) {
      this.eventEmitter.emit({
        type: GameEventType.MOVE_MADE,
        timestamp: Date.now(),
        data: { position, marker: this.state.currentTurn }
      })

      if (result.newState.phase === GamePhase.FINISHED) {
        this.eventEmitter.emit({
          type: GameEventType.GAME_FINISHED,
          timestamp: Date.now(),
          data: { result: result.newState.result }
        })
      }
    }

    return result
  }
}
```

**Benefits:**
- Display system completely decoupled
- Easy to add new observers (analytics, logging, etc.)
- Follows Open/Closed Principle
- Enables reactive UI updates

### 4. **Strategy Pattern - Already Using for Agent (EXCELLENT!)**

**Current Design:** ✅ Already implemented correctly

```typescript
interface Agent {
  getNextMove(board: Board): Position
}

class HumanAgent implements Agent {
  getNextMove(board: Board): Position {
    // Get input from user interface
  }
}

class RandomAI implements Agent {
  getNextMove(board: Board): Position {
    const validMoves = this.getValidMoves(board)
    return validMoves[Math.floor(Math.random() * validMoves.length)]
  }
}

class MinimaxAI implements Agent {
  getNextMove(board: Board): Position {
    // Minimax algorithm
  }
}
```

**Suggestion:** Make it asynchronous to support thinking time / animations:

```typescript
interface Agent {
  getNextMove(board: Board): Promise<Position>
}
```

### 5. **Factory Pattern - For Object Creation**

**Use Case:** Centralize and standardize object creation.

```typescript
class GameFactory {
  static createStandardGame(): Game {
    const board = new Board()
    return new Game(board, new WinConditionChecker(), new MoveValidator())
  }

  static createGameFromState(state: GameState): Game {
    // Reconstruct game from saved state
  }
}

class AgentFactory {
  static createAgent(type: 'human' | 'random' | 'minimax'): Agent {
    switch (type) {
      case 'human': return new HumanAgent()
      case 'random': return new RandomAI()
      case 'minimax': return new MinimaxAI()
    }
  }
}

class SessionFactory {
  static createSession(
    player1Name: string,
    player1Type: AgentType,
    player2Name: string,
    player2Type: AgentType
  ): Session {
    const player1 = new Player(player1Name, AgentFactory.createAgent(player1Type))
    const player2 = new Player(player2Name, AgentFactory.createAgent(player2Type))
    return new Session(player1, player2)
  }
}
```

### 6. **Specification Pattern - For Complex Validation Rules**

**Use Case:** Composable validation logic.

```typescript
interface Specification<T> {
  isSatisfiedBy(candidate: T): boolean
  and(other: Specification<T>): Specification<T>
  or(other: Specification<T>): Specification<T>
  not(): Specification<T>
}

class PositionInBoundsSpec implements Specification<Position> {
  isSatisfiedBy(position: Position): boolean {
    const index = position.getIndex()
    return index >= 0 && index < 9
  }
}

class CellEmptySpec implements Specification<{position: Position, board: Board}> {
  isSatisfiedBy({position, board}): boolean {
    return board.isEmpty(position)
  }
}

class ValidMoveSpec implements Specification<{position: Position, board: Board, phase: GamePhase}> {
  isSatisfiedBy({position, board, phase}): boolean {
    return new PositionInBoundsSpec().isSatisfiedBy(position)
      && new CellEmptySpec().isSatisfiedBy({position, board})
      && phase === GamePhase.IN_PROGRESS
  }
}
```

### 7. **Null Object Pattern - For Empty Cells**

**Use Case:** Avoid null checks.

```typescript
interface CellState {
  isEmpty(): boolean
  getMarker(): Marker | null
  toString(): string
}

class EmptyCell implements CellState {
  isEmpty(): boolean { return true }
  getMarker(): Marker | null { return null }
  toString(): string { return ' ' }
}

class MarkedCell implements CellState {
  constructor(private marker: Marker) {}
  isEmpty(): boolean { return false }
  getMarker(): Marker { return this.marker }
  toString(): string { return this.marker === Marker.X ? 'X' : 'O' }
}

class Board {
  private cells: CellState[] = new Array(9).fill(null).map(() => new EmptyCell())

  getCell(position: Position): CellState {
    return this.cells[position.getIndex()] // Never null!
  }
}
```

---

## SOLID Principle Analysis

### Single Responsibility Principle (SRP)

**Violations:**
- ✗ Game class likely handles too many concerns
- ✗ Board might handle both state and validation

**Recommendations:**
- ✓ Extract WinConditionChecker
- ✓ Extract MoveValidator
- ✓ Extract GameController
- ✓ Keep Board as pure state container

### Open/Closed Principle (OCP)

**Violations:**
- ✗ GameStatus enum is closed to extension (can't add new states without modifying)
- ✗ Win conditions hardcoded (can't support different board sizes)

**Good Examples:**
- ✓ Agent interface is open for extension (can add new AI types without modifying existing code)

**Recommendations:**
- Replace enums with strategy/state pattern where extensibility is needed
- Extract win condition logic into configurable checker

### Liskov Substitution Principle (LSP)

**Good:**
- ✓ Any Agent implementation can substitute another (HumanAgent, RandomAI, MinimaxAI)

**Recommendations:**
- Ensure all Agent implementations handle the same input/output contracts
- Document preconditions and postconditions

### Interface Segregation Principle (ISP)

**Good:**
- ✓ Agent interface is minimal and focused

**Recommendations:**
- Consider splitting larger interfaces if they emerge
- Example: `Board` might need `ReadableBoard` vs `WritableBoard` interfaces

### Dependency Inversion Principle (DIP)

**Violations:**
- ✗ Game depends on concrete Board (minor issue, depends on design goals)
- ✗ Session depends on concrete Game

**Recommendations:**
```typescript
interface IBoard {
  getCell(position: Position): Marker | null
  isEmpty(position: Position): boolean
  isFull(): boolean
}

interface IGame {
  applyMove(position: Position): MoveResult
  getState(): GameState
  reset(): void
}

class GameController {
  constructor(
    private board: IBoard,  // Depend on abstraction
    private rules: WinConditionChecker,
    private validator: MoveValidator
  ) {}
}
```

---

## Recommended Architecture Refactoring

### Proposed Class Structure

```typescript
// ============================================================================
// DOMAIN MODEL - Pure data structures
// ============================================================================

enum Marker {
  X = 'X',
  O = 'O'
}

enum GamePhase {
  NOT_STARTED = 'NOT_STARTED',
  IN_PROGRESS = 'IN_PROGRESS',
  FINISHED = 'FINISHED'
}

enum GameResult {
  NOT_DETERMINED = 'NOT_DETERMINED',
  X_WINS = 'X_WINS',
  O_WINS = 'O_WINS',
  DRAW = 'DRAW'
}

class Position {
  private readonly index: number

  constructor(index: number)
  constructor(row: number, col: number)
  constructor(rowOrIndex: number, col?: number) {
    if (col === undefined) {
      this.index = rowOrIndex
    } else {
      this.index = rowOrIndex * 3 + col
    }
  }

  getIndex(): number { return this.index }
  getRow(): number { return Math.floor(this.index / 3) }
  getCol(): number { return this.index % 3 }

  equals(other: Position): boolean {
    return this.index === other.index
  }

  toString(): string {
    return `(${this.getRow()}, ${this.getCol()})`
  }
}

interface Move {
  readonly position: Position
  readonly marker: Marker
  readonly moveNumber: number
}

interface GameState {
  readonly board: Board
  readonly phase: GamePhase
  readonly result: GameResult
  readonly currentTurn: Marker
  readonly moveHistory: readonly Move[]
}

// ============================================================================
// BOARD - Immutable state container
// ============================================================================

class Board {
  private readonly cells: ReadonlyArray<Marker | null>

  constructor(cells?: ReadonlyArray<Marker | null>) {
    this.cells = cells ?? new Array(9).fill(null)
  }

  getCell(position: Position): Marker | null {
    return this.cells[position.getIndex()]
  }

  isEmpty(position: Position): boolean {
    return this.getCell(position) === null
  }

  isFull(): boolean {
    return this.cells.every(cell => cell !== null)
  }

  // Immutable update
  withMove(position: Position, marker: Marker): Board {
    const newCells = [...this.cells]
    newCells[position.getIndex()] = marker
    return new Board(newCells)
  }

  getAllCells(): ReadonlyArray<Marker | null> {
    return this.cells
  }
}

// ============================================================================
// GAME RULES - Validation and win checking
// ============================================================================

enum MoveError {
  INVALID_POSITION = 'Position is out of bounds',
  CELL_OCCUPIED = 'Cell is already occupied',
  WRONG_TURN = 'It is not your turn',
  GAME_NOT_IN_PROGRESS = 'Game is not in progress',
  GAME_ALREADY_FINISHED = 'Game has already finished'
}

interface ValidationResult {
  isValid: boolean
  error?: MoveError
}

class MoveValidator {
  validate(position: Position, board: Board, phase: GamePhase): ValidationResult {
    // Check game phase
    if (phase !== GamePhase.IN_PROGRESS) {
      return { isValid: false, error: MoveError.GAME_NOT_IN_PROGRESS }
    }

    // Check position bounds
    const index = position.getIndex()
    if (index < 0 || index >= 9) {
      return { isValid: false, error: MoveError.INVALID_POSITION }
    }

    // Check if cell is empty
    if (!board.isEmpty(position)) {
      return { isValid: false, error: MoveError.CELL_OCCUPIED }
    }

    return { isValid: true }
  }

  getValidMoves(board: Board): Position[] {
    const validMoves: Position[] = []
    for (let i = 0; i < 9; i++) {
      const position = new Position(i)
      if (board.isEmpty(position)) {
        validMoves.push(position)
      }
    }
    return validMoves
  }
}

class WinConditionChecker {
  private readonly WIN_PATTERNS = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8], // Rows
    [0, 3, 6], [1, 4, 7], [2, 5, 8], // Columns
    [0, 4, 8], [2, 4, 6]              // Diagonals
  ]

  checkWinner(board: Board): Marker | null {
    for (const pattern of this.WIN_PATTERNS) {
      const [a, b, c] = pattern
      const cells = [
        board.getCell(new Position(a)),
        board.getCell(new Position(b)),
        board.getCell(new Position(c))
      ]

      if (cells[0] !== null && cells[0] === cells[1] && cells[1] === cells[2]) {
        return cells[0]
      }
    }
    return null
  }

  checkDraw(board: Board, moveCount: number): boolean {
    return moveCount === 9 && this.checkWinner(board) === null
  }
}

// ============================================================================
// GAME LOGIC - Pure functions for state transformation
// ============================================================================

class GameLogic {
  static createInitialState(): GameState {
    return {
      board: new Board(),
      phase: GamePhase.NOT_STARTED,
      result: GameResult.NOT_DETERMINED,
      currentTurn: Marker.X,
      moveHistory: []
    }
  }

  static startGame(state: GameState): GameState {
    return {
      ...state,
      phase: GamePhase.IN_PROGRESS,
      board: new Board(),
      currentTurn: Marker.X,
      moveHistory: []
    }
  }

  static applyMove(
    state: GameState,
    position: Position,
    winChecker: WinConditionChecker
  ): GameState {
    // Create move
    const move: Move = {
      position,
      marker: state.currentTurn,
      moveNumber: state.moveHistory.length + 1
    }

    // Update board
    const newBoard = state.board.withMove(position, state.currentTurn)

    // Check for winner
    const winner = winChecker.checkWinner(newBoard)
    const moveCount = state.moveHistory.length + 1
    const isDraw = winChecker.checkDraw(newBoard, moveCount)

    // Determine new state
    let newPhase = state.phase
    let newResult = state.result
    let newTurn = state.currentTurn

    if (winner) {
      newPhase = GamePhase.FINISHED
      newResult = winner === Marker.X ? GameResult.X_WINS : GameResult.O_WINS
    } else if (isDraw) {
      newPhase = GamePhase.FINISHED
      newResult = GameResult.DRAW
    } else {
      newTurn = state.currentTurn === Marker.X ? Marker.O : Marker.X
    }

    return {
      board: newBoard,
      phase: newPhase,
      result: newResult,
      currentTurn: newTurn,
      moveHistory: [...state.moveHistory, move]
    }
  }

  static reset(state: GameState): GameState {
    return GameLogic.createInitialState()
  }
}

// ============================================================================
// GAME CONTROLLER - Orchestrates game flow
// ============================================================================

type MoveResult =
  | { success: true; state: GameState }
  | { success: false; error: MoveError }

class GameController {
  private state: GameState
  private readonly validator: MoveValidator
  private readonly winChecker: WinConditionChecker

  constructor() {
    this.state = GameLogic.createInitialState()
    this.validator = new MoveValidator()
    this.winChecker = new WinConditionChecker()
  }

  start(): void {
    this.state = GameLogic.startGame(this.state)
  }

  makeMove(position: Position): MoveResult {
    // Validate
    const validation = this.validator.validate(
      position,
      this.state.board,
      this.state.phase
    )

    if (!validation.isValid) {
      return { success: false, error: validation.error! }
    }

    // Apply
    this.state = GameLogic.applyMove(this.state, position, this.winChecker)

    return { success: true, state: this.state }
  }

  getState(): GameState {
    return this.state
  }

  reset(): void {
    this.state = GameLogic.reset(this.state)
  }

  getValidMoves(): Position[] {
    if (this.state.phase !== GamePhase.IN_PROGRESS) {
      return []
    }
    return this.validator.getValidMoves(this.state.board)
  }
}

// ============================================================================
// PLAYER & AGENT - Strategy pattern for different player types
// ============================================================================

interface Agent {
  getNextMove(board: Board, validMoves: Position[]): Promise<Position>
}

class Player {
  readonly name: string
  wins: number = 0
  readonly agent: Agent

  constructor(name: string, agent: Agent) {
    this.name = name
    this.agent = agent
  }
}

// ============================================================================
// SESSION - Manages multi-game session between two players
// ============================================================================

class Session {
  private readonly player1: Player
  private readonly player2: Player
  private draws: number = 0
  private currentGame: GameController | null = null

  constructor(player1: Player, player2: Player) {
    this.player1 = player1
    this.player2 = player2
  }

  getPlayer1(): Player { return this.player1 }
  getPlayer2(): Player { return this.player2 }
  getDraws(): number { return this.draws }

  startNewGame(): GameController {
    this.currentGame = new GameController()
    this.currentGame.start()
    return this.currentGame
  }

  getCurrentGame(): GameController | null {
    return this.currentGame
  }

  recordResult(result: GameResult): void {
    switch (result) {
      case GameResult.X_WINS:
        this.player1.wins++
        break
      case GameResult.O_WINS:
        this.player2.wins++
        break
      case GameResult.DRAW:
        this.draws++
        break
    }
  }

  getStats() {
    return {
      player1: { name: this.player1.name, wins: this.player1.wins },
      player2: { name: this.player2.name, wins: this.player2.wins },
      draws: this.draws
    }
  }
}
```

---

## Additional Recommendations

### 1. **Consider Event Sourcing for Game History**

Instead of just storing moves, store events:

```typescript
type GameEvent =
  | { type: 'GAME_STARTED'; timestamp: number }
  | { type: 'MOVE_MADE'; move: Move; timestamp: number }
  | { type: 'GAME_FINISHED'; result: GameResult; timestamp: number }

class EventStore {
  private events: GameEvent[] = []

  append(event: GameEvent): void {
    this.events.push(event)
  }

  getEvents(): readonly GameEvent[] {
    return this.events
  }

  replay(): GameState {
    // Rebuild game state from events
  }
}
```

### 2. **Add TypeScript Strict Mode**

```json
{
  "compilerOptions": {
    "strict": true,
    "noImplicitAny": true,
    "strictNullChecks": true,
    "strictFunctionTypes": true,
    "noUnusedLocals": true,
    "noUnusedParameters": true,
    "noImplicitReturns": true
  }
}
```

### 3. **Consider Readonly Everything**

Make all properties readonly by default. Use `Readonly<T>` and `ReadonlyArray<T>`.

### 4. **Add Comprehensive Unit Tests**

```typescript
describe('Board', () => {
  it('should start empty', () => {
    const board = new Board()
    expect(board.isEmpty(new Position(0))).toBe(true)
  })

  it('should be immutable on move', () => {
    const board1 = new Board()
    const board2 = board1.withMove(new Position(0), Marker.X)
    expect(board1.isEmpty(new Position(0))).toBe(true)
    expect(board2.getCell(new Position(0))).toBe(Marker.X)
  })
})

describe('WinConditionChecker', () => {
  it('should detect horizontal win', () => {
    const board = new Board([
      Marker.X, Marker.X, Marker.X,
      null, null, null,
      null, null, null
    ])
    const checker = new WinConditionChecker()
    expect(checker.checkWinner(board)).toBe(Marker.X)
  })
})
```

### 5. **Consider AI Agent Implementations**

```typescript
class RandomAI implements Agent {
  async getNextMove(board: Board, validMoves: Position[]): Promise<Position> {
    return validMoves[Math.floor(Math.random() * validMoves.length)]
  }
}

class MinimaxAI implements Agent {
  async getNextMove(board: Board, validMoves: Position[]): Promise<Position> {
    // Implement minimax algorithm
    return this.minimax(board, Marker.O, 0).position
  }

  private minimax(board: Board, player: Marker, depth: number): {score: number, position: Position} {
    // Minimax implementation
  }
}

class HumanAgent implements Agent {
  constructor(private inputHandler: () => Promise<Position>) {}

  async getNextMove(board: Board, validMoves: Position[]): Promise<Position> {
    return this.inputHandler()
  }
}
```

---

## Implementation Priority

### Phase 1: Critical Fixes (Must Do)
1. Separate GameStatus into GamePhase + GameResult + currentTurn
2. Extract WinConditionChecker
3. Extract MoveValidator
4. Add MoveResult/MoveError types
5. Simplify Board implementation (remove bit manipulation)

### Phase 2: Core Improvements (Should Do)
6. Make Board immutable
7. Add Move abstraction
8. Create GameController
9. Implement GameLogic with pure functions
10. Add proper GameState interface

### Phase 3: Advanced Patterns (Nice to Have)
11. Add Observer pattern for events
12. Add Command pattern for undo/redo
13. Add State pattern for game phases
14. Add Specification pattern for validation

### Phase 4: Polish (Optional)
15. Add event sourcing
16. Add comprehensive tests
17. Add TypeScript strict mode
18. Optimize performance if needed

---

## Conclusion

The current architecture has a solid foundation with good use of the Strategy pattern for Agents. However, there are several critical design issues that should be addressed:

**Must Fix:**
- Separation of concerns in Game class
- GameStatus enum conflation
- Missing error handling
- Board bit manipulation complexity

**Key Principles:**
- **Immutability**: Makes code predictable and testable
- **Single Responsibility**: Each class does one thing well
- **Separation of State and Logic**: State is data, logic is functions
- **Clear Abstractions**: Position, Move, Board, GameState, etc.
- **Proper Error Handling**: Result types instead of exceptions

**Design Patterns to Use:**
- Strategy (Agent) ✓ Already implemented
- State (GamePhase)
- Observer (Events)
- Command (Undo/Redo)

By implementing these recommendations, you'll have a robust, maintainable, testable, and extensible core architecture that will make your manager very happy and keep your families proud!
