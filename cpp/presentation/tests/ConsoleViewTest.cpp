#include <gtest/gtest.h>
#include "presentation/ConsoleView.hpp"

namespace game::view {

class ConsoleViewTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code before each test
    }

    void TearDown() override {
        // Cleanup code after each test
    }
};

TEST_F(ConsoleViewTest, ViewIsInitializedByDefault) {
    ConsoleView view;
    EXPECT_TRUE(view.isInitialized());
}

TEST_F(ConsoleViewTest, SkeletonTestPasses) {
    // Placeholder test to verify test infrastructure works
    EXPECT_EQ(3 + 3, 6);
}

} // namespace game::view
