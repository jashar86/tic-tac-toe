# Domain Layer - Pure game logic with zero external dependencies
# Contains: Board, GameState, Position, Marker, GameStatus, Agents

add_library(domain
    src/Board.cpp
)

target_include_directories(domain
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ============================================================================
# Domain Tests
# ============================================================================
add_executable(domain_tests
    tests/BoardTest.cpp
)

target_link_libraries(domain_tests
    PRIVATE
        domain
        GTest::gtest_main
)

gtest_discover_tests(domain_tests)
