# Project Guidelines

## Architecture

This project follows **Clean Architecture** with three layers:

1. **Core (Domain)** - Pure business logic, no external dependencies
2. **Application** - Use cases, orchestration, interfaces for external concerns
3. **Presentation** - UI, console I/O, user interaction

Dependencies flow inward: Presentation → Application → Core

## Design Principles

### SOLID
- **Single Responsibility**: Each class has one reason to change
- **Open/Closed**: Extend behavior through interfaces, not modification
- **Liskov Substitution**: Subtypes must be substitutable for base types
- **Interface Segregation**: Prefer small, focused interfaces
- **Dependency Inversion**: Depend on abstractions, not concretions

### General
- Prefer composition over inheritance
- Use dependency injection for testability
- Keep functions small and focused (extract when > 20 lines)
- Use meaningful names that reveal intent

## C++ Style

### Modern C++ (C++23)
- Use `std::expected<T, E>` for operations that can fail
- Use `std::optional<T>` for optional values
- Use `std::string_view` for non-owning string references
- Prefer `constexpr` where possible

### Attributes
- Add `[[nodiscard]]` to functions where ignoring return value is likely a bug
- Use `noexcept` for functions that don't throw

### Result Types
- Use tag types (empty structs) for error discrimination, not strings
- Example: `struct Quit {};` with `std::expected<T, Quit>`

## Testing

### Test-Driven Development
1. Write a failing test first
2. Write minimal code to pass
3. Refactor while keeping tests green

### Organization
- Use Google Test/Mock framework
- Place mocks in sibling `mocks/` folder (not inside `tests/`)
- One test file per production class

### Coverage
- Test public interfaces, not implementation details
- Include edge cases and error paths
- Aim for high coverage on core business logic

## Code Review

Before finalizing significant changes:
- Use a staff-level review agent to catch architectural issues
- Address critical and high-priority feedback before committing
- Document design decisions in commit messages
