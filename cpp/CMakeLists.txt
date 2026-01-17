cmake_minimum_required(VERSION 3.14)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Main executable
add_executable(tictactoe
    src/main.cpp
    src/TicTacToe.cpp
    src/Board.cpp
)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test executable
add_executable(board_test
    tests/BoardTest.cpp
    src/Board.cpp
)

target_link_libraries(board_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(board_test)
