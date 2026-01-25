# Core Layer - Pure game logic with zero external dependencies
# Contains: Board, GameState, Position, Marker, GameStatus, Agents

add_library(core
    src/Marker.cpp
    src/GameStatus.cpp
    src/Position.cpp
    src/Board.cpp
    src/GameState.cpp
)

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ============================================================================
# Core Tests
# ============================================================================
add_executable(core_tests
    tests/MarkerTest.cpp
    tests/GameStatusTest.cpp
    tests/PositionTest.cpp
    tests/BoardTest.cpp
    tests/GameStateTest.cpp
)

target_link_libraries(core_tests
    PRIVATE
        core
        GTest::gtest_main
)

gtest_discover_tests(core_tests)
