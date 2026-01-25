#include <gtest/gtest.h>
#include "application/Session.hpp"

namespace tictactoe::application {

class SessionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
    }

    void TearDown() override {
        // Cleanup code after each test
    }
};

TEST_F(SessionTest, NewSessionIsNotActive) {
    Session session;
    EXPECT_FALSE(session.isActive());
}

TEST_F(SessionTest, SkeletonTestPasses) {
    // Placeholder test to verify test infrastructure works
    EXPECT_EQ(2 + 2, 4);
}

} // namespace tictactoe::application
