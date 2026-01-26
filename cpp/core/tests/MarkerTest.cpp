#include "core/Marker.hpp"

#include <gtest/gtest.h>

namespace game::core
{

TEST(MarkerTest, HasXValue)
{
    Marker x = Marker::X;
    EXPECT_EQ(x, Marker::X);
}

TEST(MarkerTest, HasOValue)
{
    Marker o = Marker::O;
    EXPECT_EQ(o, Marker::O);
}

TEST(MarkerTest, XAndOAreDifferent)
{
    EXPECT_NE(Marker::X, Marker::O);
}

TEST(MarkerTest, OpponentOfXIsO)
{
    EXPECT_EQ(opponentOf(Marker::X), Marker::O);
}

TEST(MarkerTest, OpponentOfOIsX)
{
    EXPECT_EQ(opponentOf(Marker::O), Marker::X);
}

} // namespace game::core
