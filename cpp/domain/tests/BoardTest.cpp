#include <gtest/gtest.h>
#include "domain/Board.hpp"

namespace tictactoe::domain {

class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
    }

    void TearDown() override {
        // Cleanup code after each test
    }
};

TEST_F(BoardTest, NewBoardIsEmpty) {
    Board board;
    EXPECT_TRUE(board.isEmpty());
}

TEST_F(BoardTest, SkeletonTestPasses) {
    // Placeholder test to verify test infrastructure works
    EXPECT_EQ(1 + 1, 2);
}

} // namespace tictactoe::domain
