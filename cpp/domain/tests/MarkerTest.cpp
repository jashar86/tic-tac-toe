#include <gtest/gtest.h>
#include "domain/Marker.hpp"

namespace game::core {

TEST(MarkerTest, HasXValue) {
    Marker x = Marker::X;
    EXPECT_EQ(x, Marker::X);
}

TEST(MarkerTest, HasOValue) {
    Marker o = Marker::O;
    EXPECT_EQ(o, Marker::O);
}

TEST(MarkerTest, HasEmptyValue) {
    Marker empty = Marker::Empty;
    EXPECT_EQ(empty, Marker::Empty);
}

TEST(MarkerTest, XAndOAreDifferent) {
    EXPECT_NE(Marker::X, Marker::O);
}

TEST(MarkerTest, EmptyIsDifferentFromXAndO) {
    EXPECT_NE(Marker::Empty, Marker::X);
    EXPECT_NE(Marker::Empty, Marker::O);
}

TEST(MarkerTest, OpponentOfXIsO) {
    EXPECT_EQ(opponent(Marker::X), Marker::O);
}

TEST(MarkerTest, OpponentOfOIsX) {
    EXPECT_EQ(opponent(Marker::O), Marker::X);
}

} // namespace game::core
