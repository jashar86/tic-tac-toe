# Implementation Guide: Best Practices & Patterns

## Overview

This guide provides practical implementation details, testing strategies, common pitfalls, and specific code patterns for building the tic-tac-toe core architecture.

---

## Part 1: Immutability Patterns

### Why Immutability Matters

**Problem with Mutable State:**
```typescript
// Bad: Mutable state leads to bugs
class Board {
  private cells: (Marker | null)[] = new Array(9).fill(null)

  setCell(position: Position, marker: Marker): void {
    this.cells[position.getIndex()] = marker
  }
}

// Usage - unexpected behavior!
const board = new Board()
const savedBoard = board  // Just a reference, not a copy!
board.setCell(new Position(0), Marker.X)

console.log(savedBoard === board)  // true - they're the same object!
// You can't "undo" because you lost the previous state
```

**Solution with Immutable State:**
```typescript
// Good: Immutable state is predictable
class Board {
  private readonly cells: ReadonlyArray<Marker | null>

  constructor(cells: ReadonlyArray<Marker | null> = new Array(9).fill(null)) {
    this.cells = cells
  }

  withMove(position: Position, marker: Marker): Board {
    const newCells = [...this.cells]  // Create a copy
    newCells[position.getIndex()] = marker
    return new Board(newCells)  // Return new instance
  }
}

// Usage - safe and predictable!
const board1 = new Board()
const board2 = board1.withMove(new Position(0), Marker.X)
const board3 = board2.withMove(new Position(1), Marker.O)

console.log(board1 === board2)  // false - different objects
console.log(board1 === board3)  // false
// You can go back to any previous state!
```

### Deep vs Shallow Immutability

```typescript
// Shallow immutability - WRONG!
class GameState {
  readonly board: Board  // Board itself might be mutable
  readonly currentTurn: Marker
}

// Deep immutability - CORRECT!
interface GameState {
  readonly board: Board  // Board is also immutable
  readonly currentTurn: Marker
  readonly moveHistory: readonly Move[]  // Array is readonly
}

// Enforce deep immutability with TypeScript
type DeepReadonly<T> = {
  readonly [K in keyof T]: T[K] extends object ? DeepReadonly<T[K]> : T[K]
}

type ImmutableGameState = DeepReadonly<GameState>
```

### Structural Sharing for Performance

```typescript
// Naive approach - copies everything
class Board {
  withMove(position: Position, marker: Marker): Board {
    const newCells = [...this.cells]  // Copies all 9 cells
    newCells[position.getIndex()] = marker
    return new Board(newCells)
  }
}

// Optimized approach - structural sharing (for larger structures)
import { produce } from 'immer'  // Library for structural sharing

class Board {
  withMove(position: Position, marker: Marker): Board {
    return produce(this, draft => {
      draft.cells[position.getIndex()] = marker
    })
  }
}

// Note: For a 3x3 tic-tac-toe board, structural sharing is overkill.
// Simple copying is fine. But good to know for larger structures!
```

---

## Part 2: Error Handling Patterns

### Result Type Pattern (No Exceptions for Control Flow)

```typescript
// Bad: Using exceptions for control flow
class GameController {
  makeMove(position: Position): void {
    if (!this.validator.isValid(position)) {
      throw new Error('Invalid move')  // Exception for expected case!
    }
    // ...
  }
}

// Usage - ugly and error-prone
try {
  controller.makeMove(position)
} catch (error) {
  // Is this an expected error or unexpected error?
  console.error(error.message)
}
```

```typescript
// Good: Result type for expected errors
type Result<T, E> =
  | { ok: true; value: T }
  | { ok: false; error: E }

enum MoveError {
  INVALID_POSITION = 'INVALID_POSITION',
  CELL_OCCUPIED = 'CELL_OCCUPIED',
  WRONG_TURN = 'WRONG_TURN',
  GAME_FINISHED = 'GAME_FINISHED'
}

class GameController {
  makeMove(position: Position): Result<GameState, MoveError> {
    const validation = this.validator.validate(position, this.state)

    if (!validation.isValid) {
      return { ok: false, error: validation.error }
    }

    const newState = GameLogic.applyMove(this.state, position)
    return { ok: true, value: newState }
  }
}

// Usage - clear and type-safe
const result = controller.makeMove(position)

if (result.ok) {
  console.log('Move successful!', result.value)
} else {
  switch (result.error) {
    case MoveError.INVALID_POSITION:
      console.log('That position is invalid')
      break
    case MoveError.CELL_OCCUPIED:
      console.log('That cell is already taken')
      break
    case MoveError.WRONG_TURN:
      console.log('It\'s not your turn')
      break
    case MoveError.GAME_FINISHED:
      console.log('The game is already finished')
      break
  }
}
```

### Option Type Pattern (No Null/Undefined)

```typescript
// Bad: Nullable returns
class WinConditionChecker {
  checkWinner(board: Board): Marker | null {
    // Returns null if no winner
  }
}

// Usage - forgot to check for null!
const winner = checker.checkWinner(board)
console.log(winner.toString())  // Runtime error if null!
```

```typescript
// Good: Option type
type Option<T> =
  | { hasValue: true; value: T }
  | { hasValue: false }

class WinConditionChecker {
  checkWinner(board: Board): Option<Marker> {
    const winner = this.calculateWinner(board)
    return winner
      ? { hasValue: true, value: winner }
      : { hasValue: false }
  }
}

// Usage - forced to handle both cases
const result = checker.checkWinner(board)

if (result.hasValue) {
  console.log(`${result.value} wins!`)
} else {
  console.log('No winner yet')
}

// Alternative: Use a library like fp-ts
import { Option, some, none } from 'fp-ts/Option'

class WinConditionChecker {
  checkWinner(board: Board): Option<Marker> {
    const winner = this.calculateWinner(board)
    return winner ? some(winner) : none
  }
}
```

---

## Part 3: Testing Strategies

### Unit Testing Pure Functions

```typescript
// Pure functions are trivial to test
describe('GameLogic.applyMove', () => {
  it('should switch turns after move', () => {
    const initialState: GameState = {
      board: new Board(),
      phase: GamePhase.IN_PROGRESS,
      result: GameResult.NOT_DETERMINED,
      currentTurn: Marker.X,
      moveHistory: []
    }

    const newState = GameLogic.applyMove(initialState, new Position(0))

    expect(newState.currentTurn).toBe(Marker.O)
    expect(newState.board.getCell(new Position(0))).toBe(Marker.X)
    expect(initialState.currentTurn).toBe(Marker.X)  // Original unchanged!
  })

  it('should detect win condition', () => {
    const board = new Board([
      Marker.X, Marker.X, null,
      null, null, null,
      null, null, null
    ])

    const initialState: GameState = {
      board,
      phase: GamePhase.IN_PROGRESS,
      result: GameResult.NOT_DETERMINED,
      currentTurn: Marker.X,
      moveHistory: []
    }

    const newState = GameLogic.applyMove(initialState, new Position(2))

    expect(newState.result).toBe(GameResult.X_WINS)
    expect(newState.phase).toBe(GamePhase.FINISHED)
  })
})
```

### Property-Based Testing

```typescript
import { fc } from 'fast-check'

describe('Board properties', () => {
  it('should maintain exactly 9 cells', () => {
    fc.assert(
      fc.property(
        fc.array(fc.constantFrom(Marker.X, Marker.O, null), { minLength: 9, maxLength: 9 }),
        (cells) => {
          const board = new Board(cells)
          expect(board.getAllCells().length).toBe(9)
        }
      )
    )
  })

  it('withMove should only change the specified cell', () => {
    fc.assert(
      fc.property(
        fc.integer({ min: 0, max: 8 }),
        fc.constantFrom(Marker.X, Marker.O),
        (index, marker) => {
          const board = new Board()
          const newBoard = board.withMove(new Position(index), marker)

          // Check that only one cell changed
          const changes = board.getAllCells().filter((cell, i) =>
            cell !== newBoard.getAllCells()[i]
          )

          expect(changes.length).toBe(1)
          expect(newBoard.getCell(new Position(index))).toBe(marker)
        }
      )
    )
  })
})
```

### Table-Driven Tests

```typescript
describe('WinConditionChecker', () => {
  const checker = new WinConditionChecker()

  const winScenarios: Array<{
    name: string
    cells: Array<Marker | null>
    expected: Marker | null
  }> = [
    {
      name: 'top row X wins',
      cells: [
        Marker.X, Marker.X, Marker.X,
        null, null, null,
        null, null, null
      ],
      expected: Marker.X
    },
    {
      name: 'middle column O wins',
      cells: [
        null, Marker.O, null,
        null, Marker.O, null,
        null, Marker.O, null
      ],
      expected: Marker.O
    },
    {
      name: 'diagonal X wins',
      cells: [
        Marker.X, null, null,
        null, Marker.X, null,
        null, null, Marker.X
      ],
      expected: Marker.X
    },
    {
      name: 'no winner',
      cells: [
        Marker.X, Marker.O, Marker.X,
        Marker.O, Marker.X, Marker.O,
        Marker.O, Marker.X, Marker.O
      ],
      expected: null
    }
  ]

  winScenarios.forEach(({ name, cells, expected }) => {
    it(`should correctly handle: ${name}`, () => {
      const board = new Board(cells)
      const result = checker.checkWinner(board)
      expect(result).toBe(expected)
    })
  })
})
```

### Testing with Mocks (for impure code)

```typescript
// When you need to test code with side effects
describe('GameController with DisplayPort', () => {
  it('should display board after move', async () => {
    // Create mock
    const mockDisplay: DisplayPort = {
      showBoard: jest.fn(),
      showMessage: jest.fn(),
      getPlayerMove: jest.fn()
    }

    const controller = new GameController(mockDisplay)
    controller.start()

    const result = await controller.makeMove(new Position(0))

    expect(result.ok).toBe(true)
    expect(mockDisplay.showBoard).toHaveBeenCalledTimes(1)
    expect(mockDisplay.showBoard).toHaveBeenCalledWith(
      expect.any(Board)
    )
  })
})
```

---

## Part 4: Common Pitfalls and Anti-Patterns

### Pitfall 1: God Objects

```typescript
// Bad: Game class does everything
class Game {
  private board: Board
  private currentPlayer: Marker
  private status: GameStatus
  private players: Player[]
  private moveHistory: Move[]
  private winChecker: WinConditionChecker

  constructor() { /* ... */ }

  start() { /* ... */ }
  makeMove() { /* ... */ }
  checkWinner() { /* ... */ }
  validateMove() { /* ... */ }
  switchPlayer() { /* ... */ }
  getStats() { /* ... */ }
  saveGame() { /* ... */ }
  loadGame() { /* ... */ }
  displayBoard() { /* ... */ }
  getPlayerInput() { /* ... */ }
  // 20 more methods...
}
```

```typescript
// Good: Separation of concerns
class GameState { /* Just data */ }
const GameLogic = { /* Pure functions */ }
class WinConditionChecker { /* One responsibility */ }
class MoveValidator { /* One responsibility */ }
class GameController { /* Orchestration */ }
class DisplayPort { /* Interface for display */ }
```

### Pitfall 2: Primitive Obsession

```typescript
// Bad: Using primitives everywhere
function makeMove(row: number, col: number, player: string): boolean {
  // What if row is -1? What if player is "purple"?
}

const winner = checkWinner()  // Returns "X", "O", or null
if (winner === "X") { }  // Typo: "x" instead of "X" - no error!
```

```typescript
// Good: Strong types
class Position {
  constructor(private index: number) {
    if (index < 0 || index > 8) {
      throw new Error('Invalid position')
    }
  }
}

enum Marker { X = 'X', O = 'O' }

function makeMove(position: Position, marker: Marker): Result<GameState, MoveError> {
  // Impossible to pass invalid values!
}
```

### Pitfall 3: Leaked Abstractions

```typescript
// Bad: Implementation details leak through interface
class Board {
  private cells: Map<string, Marker>  // Implementation detail

  getCells(): Map<string, Marker> {  // LEAKING THE MAP!
    return this.cells
  }
}

// Client code now depends on Map
const cells = board.getCells()
cells.set('0', Marker.X)  // Directly mutating internal state!
```

```typescript
// Good: Hide implementation details
class Board {
  private cells: Map<string, Marker>  // Private implementation

  getCell(position: Position): Marker | null {
    return this.cells.get(position.toString()) ?? null
  }

  withMove(position: Position, marker: Marker): Board {
    // Returns new board, doesn't expose internal structure
  }
}
```

### Pitfall 4: Temporal Coupling

```typescript
// Bad: Methods must be called in specific order
class Game {
  private board: Board | null = null

  initialize(): void {
    this.board = new Board()
  }

  makeMove(position: Position): void {
    // Crashes if initialize() wasn't called first!
    this.board.setCell(position, Marker.X)
  }
}

// Usage - easy to forget initialization
const game = new Game()
game.makeMove(new Position(0))  // CRASH!
```

```typescript
// Good: Valid state always
class Game {
  private readonly board: Board  // Always initialized

  constructor() {
    this.board = new Board()  // Constructor ensures valid state
  }

  makeMove(position: Position): Result<GameState, MoveError> {
    // Always safe to call
  }
}
```

### Pitfall 5: Boolean Blindness

```typescript
// Bad: Boolean parameters
function setCell(position: Position, isX: boolean): void {
  const marker = isX ? Marker.X : Marker.O
  // ...
}

// Usage - what does 'true' mean here?
setCell(new Position(0), true)  // Is this X or O?
```

```typescript
// Good: Explicit types
function setCell(position: Position, marker: Marker): void {
  // ...
}

// Usage - clear and self-documenting
setCell(new Position(0), Marker.X)
```

### Pitfall 6: Anemic Domain Model

```typescript
// Bad: Data classes with no behavior
class Board {
  cells: (Marker | null)[] = []
}

class BoardService {
  setCell(board: Board, position: Position, marker: Marker): void { }
  getCell(board: Board, position: Position): Marker | null { }
  isEmpty(board: Board, position: Position): boolean { }
  isFull(board: Board): boolean { }
}

// All logic is outside the domain model!
```

```typescript
// Good: Rich domain model
class Board {
  private cells: ReadonlyArray<Marker | null>

  getCell(position: Position): Marker | null {
    return this.cells[position.getIndex()]
  }

  isEmpty(position: Position): boolean {
    return this.getCell(position) === null
  }

  isFull(): boolean {
    return this.cells.every(cell => cell !== null)
  }

  withMove(position: Position, marker: Marker): Board {
    // Behavior is part of the domain object
  }
}
```

---

## Part 5: Performance Considerations

### When to Optimize

```typescript
// 1. Don't optimize prematurely
// Bad: Premature optimization
class Board {
  private x_marks: number = 0  // Bit flags
  private o_marks: number = 0

  setMarker(pos: number): void {
    this.x_marks |= (1 << pos)  // Complex bit manipulation
  }
}

// Good: Start simple, optimize later if needed
class Board {
  private cells: (Marker | null)[] = new Array(9).fill(null)

  setCell(position: Position, marker: Marker): void {
    this.cells[position.getIndex()] = marker
  }
}

// 2. Measure before optimizing
// Use profiling tools to identify bottlenecks
// For tic-tac-toe, performance is rarely an issue
```

### Memoization for Expensive Calculations

```typescript
class MinimaxAI implements Agent {
  private cache = new Map<string, {score: number, position: Position}>()

  getNextMove(board: Board): Position {
    const boardKey = this.serializeBoard(board)

    if (this.cache.has(boardKey)) {
      return this.cache.get(boardKey)!.position
    }

    const result = this.minimax(board, Marker.O, 0)
    this.cache.set(boardKey, result)

    return result.position
  }

  private serializeBoard(board: Board): string {
    // Create a unique key for this board state
    return board.getAllCells().join(',')
  }
}
```

---

## Part 6: Dependency Injection Patterns

### Constructor Injection (Preferred)

```typescript
// Good: Dependencies injected via constructor
class GameController {
  constructor(
    private readonly validator: MoveValidator,
    private readonly winChecker: WinConditionChecker,
    private readonly display: DisplayPort
  ) {}

  makeMove(position: Position): MoveResult {
    // Use injected dependencies
  }
}

// Usage
const controller = new GameController(
  new MoveValidator(),
  new WinConditionChecker(),
  new ConsoleDisplay()
)

// Testing
const mockDisplay = new MockDisplay()
const testController = new GameController(
  new MoveValidator(),
  new WinConditionChecker(),
  mockDisplay  // Easy to inject mock!
)
```

### Factory Pattern

```typescript
// Centralize object creation
class GameFactory {
  static createStandardGame(display: DisplayPort): GameController {
    return new GameController(
      new MoveValidator(),
      new WinConditionChecker(),
      display
    )
  }

  static createTestGame(
    validator: MoveValidator = new MoveValidator(),
    winChecker: WinConditionChecker = new WinConditionChecker(),
    display: DisplayPort = new MockDisplay()
  ): GameController {
    return new GameController(validator, winChecker, display)
  }
}

// Usage
const game = GameFactory.createStandardGame(new ConsoleDisplay())
const testGame = GameFactory.createTestGame()
```

---

## Part 7: Code Organization Best Practices

### File Structure

```
src/
  core/
    domain/
      types.ts              # Enums, interfaces, types
      Position.ts           # Value object
      Board.ts              # Domain model
      GameState.ts          # State interface

    logic/
      GameLogic.ts          # Pure functions
      GameRules.ts          # Business rules
      MoveValidator.ts      # Validation
      WinConditionChecker.ts

    errors/
      MoveError.ts          # Error types
      Result.ts             # Result type

  application/
    GameController.ts       # Application logic
    SessionManager.ts

  ports/
    DisplayPort.ts          # Interface
    InputPort.ts
    StoragePort.ts

  adapters/
    display/
      ConsoleDisplay.ts
      WebDisplay.ts
    input/
      ConsoleInput.ts
      WebInput.ts
    storage/
      InMemoryStorage.ts
      FileStorage.ts

  agents/
    Agent.ts                # Interface
    HumanAgent.ts
    RandomAI.ts
    MinimaxAI.ts

  __tests__/
    unit/
      Board.test.ts
      GameLogic.test.ts
      WinConditionChecker.test.ts
    integration/
      GameController.test.ts
      Session.test.ts
```

### Module Exports

```typescript
// core/domain/index.ts
export { Board } from './Board'
export { Position } from './Position'
export { GameState } from './GameState'
export { Marker, GamePhase, GameResult } from './types'

// Usage in other files
import { Board, Position, Marker } from '@/core/domain'
```

### Barrel Files (index.ts)

```typescript
// core/index.ts - Single entry point
export * from './domain'
export * from './logic'
export * from './errors'

// Usage
import { Board, GameLogic, MoveError } from '@/core'
```

---

## Part 8: Documentation Best Practices

### JSDoc Comments

```typescript
/**
 * Represents an immutable tic-tac-toe board.
 *
 * The board uses a 3x3 grid indexed from 0-8:
 * ```
 * 0 | 1 | 2
 * ---------
 * 3 | 4 | 5
 * ---------
 * 6 | 7 | 8
 * ```
 *
 * @example
 * ```typescript
 * const board = new Board()
 * const newBoard = board.withMove(new Position(0), Marker.X)
 * console.log(newBoard.getCell(new Position(0))) // Marker.X
 * ```
 */
export class Board {
  /**
   * Creates a new board with the specified move applied.
   * This method does not mutate the original board.
   *
   * @param position - The position to place the marker
   * @param marker - The marker to place (X or O)
   * @returns A new Board instance with the move applied
   *
   * @example
   * ```typescript
   * const board1 = new Board()
   * const board2 = board1.withMove(new Position(4), Marker.X)
   * // board1 remains unchanged
   * ```
   */
  withMove(position: Position, marker: Marker): Board {
    // Implementation
  }
}
```

### README for Each Module

```markdown
# Game Logic Module

## Purpose
Contains pure functions for game state transformations.

## Key Principles
- All functions are pure (no side effects)
- All state is immutable
- Functions are easily testable

## Usage
```typescript
import { GameLogic } from '@/core/logic'

const initialState = GameLogic.createInitialState()
const newState = GameLogic.applyMove(initialState, new Position(0))
```

## Dependencies
- `@/core/domain` - Domain models
- None external
```

---

## Part 9: Build and Tooling

### TypeScript Configuration

```json
{
  "compilerOptions": {
    "target": "ES2020",
    "module": "ESNext",
    "lib": ["ES2020"],
    "outDir": "./dist",
    "rootDir": "./src",

    // Strict type checking
    "strict": true,
    "noImplicitAny": true,
    "strictNullChecks": true,
    "strictFunctionTypes": true,
    "strictBindCallApply": true,
    "strictPropertyInitialization": true,
    "noImplicitThis": true,
    "alwaysStrict": true,

    // Additional checks
    "noUnusedLocals": true,
    "noUnusedParameters": true,
    "noImplicitReturns": true,
    "noFallthroughCasesInSwitch": true,
    "noUncheckedIndexedAccess": true,

    // Module resolution
    "moduleResolution": "node",
    "esModuleInterop": true,
    "resolveJsonModule": true,
    "isolatedModules": true,

    // Path mapping
    "baseUrl": ".",
    "paths": {
      "@/core/*": ["src/core/*"],
      "@/application/*": ["src/application/*"],
      "@/adapters/*": ["src/adapters/*"]
    }
  },
  "include": ["src/**/*"],
  "exclude": ["node_modules", "dist", "**/*.test.ts"]
}
```

### ESLint Configuration

```json
{
  "extends": [
    "eslint:recommended",
    "plugin:@typescript-eslint/recommended",
    "plugin:@typescript-eslint/recommended-requiring-type-checking"
  ],
  "rules": {
    "@typescript-eslint/explicit-function-return-type": "error",
    "@typescript-eslint/no-explicit-any": "error",
    "@typescript-eslint/no-unused-vars": "error",
    "prefer-const": "error",
    "no-var": "error"
  }
}
```

---

## Part 10: Checklist for Implementation

### Phase 1: Core Domain (Week 1)
- [ ] Define types (Marker, GamePhase, GameResult)
- [ ] Implement Position value object
- [ ] Implement immutable Board
- [ ] Write unit tests for Board
- [ ] Implement WinConditionChecker
- [ ] Write unit tests for WinConditionChecker
- [ ] Implement MoveValidator
- [ ] Write unit tests for MoveValidator

### Phase 2: Game Logic (Week 2)
- [ ] Define GameState interface
- [ ] Implement GameLogic pure functions
- [ ] Write unit tests for GameLogic
- [ ] Implement Result/Error types
- [ ] Create GameController
- [ ] Write integration tests for GameController

### Phase 3: Display System (Week 3)
- [ ] Define DisplayPort interface
- [ ] Implement ConsoleDisplay adapter
- [ ] Test manual gameplay via console
- [ ] (Optional) Implement WebDisplay adapter

### Phase 4: AI Agents (Week 4)
- [ ] Define Agent interface
- [ ] Implement HumanAgent
- [ ] Implement RandomAI
- [ ] Implement MinimaxAI
- [ ] Test different agent combinations

### Phase 5: Session Management (Week 5)
- [ ] Implement Session class
- [ ] Add statistics tracking
- [ ] Test multi-game sessions
- [ ] Add game history/replay

### Phase 6: Polish (Week 6)
- [ ] Add comprehensive error handling
- [ ] Add logging
- [ ] Performance profiling
- [ ] Documentation
- [ ] Code review
- [ ] Refactoring

---

## Conclusion

Key takeaways:

1. **Start with immutable, pure functions** - easier to test and reason about
2. **Use strong types** - catch errors at compile time
3. **Separate concerns** - each class has one job
4. **Test thoroughly** - unit tests, integration tests, property tests
5. **Avoid premature optimization** - simple and correct first
6. **Document well** - code is read more than written
7. **Iterate** - refactor as you learn

**Most Important Rule:** Make it work, make it right, make it fast - in that order!
