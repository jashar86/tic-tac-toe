# Application Layer - Session lifecycle, stage transitions, player coordination
# Contains: TicTacToeApp, Session, Scoreboard, Stage FSM

add_library(application
    src/Session.cpp
)

target_include_directories(application
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Application layer depends on Core layer
target_link_libraries(application
    PUBLIC core
)

# ============================================================================
# Application Tests
# ============================================================================
add_executable(application_tests
    tests/SessionTest.cpp
)

target_link_libraries(application_tests
    PRIVATE
        application
        GTest::gtest_main
)

gtest_discover_tests(application_tests)
