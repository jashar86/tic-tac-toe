#include <gtest/gtest.h>
#include "domain/Position.hpp"

namespace game::core {

// Construction from index
TEST(PositionTest, ConstructFromIndex0) {
    Position pos(0);
    EXPECT_EQ(pos.index(), 0);
    EXPECT_EQ(pos.row(), 0);
    EXPECT_EQ(pos.col(), 0);
}

TEST(PositionTest, ConstructFromIndex4) {
    Position pos(4);
    EXPECT_EQ(pos.index(), 4);
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, ConstructFromIndex8) {
    Position pos(8);
    EXPECT_EQ(pos.index(), 8);
    EXPECT_EQ(pos.row(), 2);
    EXPECT_EQ(pos.col(), 2);
}

// Construction from row/col
TEST(PositionTest, ConstructFromRowCol00) {
    Position pos(0, 0);
    EXPECT_EQ(pos.index(), 0);
    EXPECT_EQ(pos.row(), 0);
    EXPECT_EQ(pos.col(), 0);
}

TEST(PositionTest, ConstructFromRowCol11) {
    Position pos(1, 1);
    EXPECT_EQ(pos.index(), 4);
    EXPECT_EQ(pos.row(), 1);
    EXPECT_EQ(pos.col(), 1);
}

TEST(PositionTest, ConstructFromRowCol22) {
    Position pos(2, 2);
    EXPECT_EQ(pos.index(), 8);
    EXPECT_EQ(pos.row(), 2);
    EXPECT_EQ(pos.col(), 2);
}

TEST(PositionTest, ConstructFromRowCol02) {
    Position pos(0, 2);
    EXPECT_EQ(pos.index(), 2);
    EXPECT_EQ(pos.row(), 0);
    EXPECT_EQ(pos.col(), 2);
}

TEST(PositionTest, ConstructFromRowCol20) {
    Position pos(2, 0);
    EXPECT_EQ(pos.index(), 6);
    EXPECT_EQ(pos.row(), 2);
    EXPECT_EQ(pos.col(), 0);
}

// All positions from index
TEST(PositionTest, AllIndexPositions) {
    for (int i = 0; i < 9; ++i) {
        Position pos(i);
        EXPECT_EQ(pos.index(), i);
        EXPECT_EQ(pos.row(), i / 3);
        EXPECT_EQ(pos.col(), i % 3);
    }
}

// Equality
TEST(PositionTest, EqualPositionsAreEqual) {
    Position pos1(4);
    Position pos2(1, 1);
    EXPECT_EQ(pos1, pos2);
}

TEST(PositionTest, DifferentPositionsAreNotEqual) {
    Position pos1(0);
    Position pos2(8);
    EXPECT_NE(pos1, pos2);
}

// Validity
TEST(PositionTest, ValidIndicesAreValid) {
    for (int i = 0; i < 9; ++i) {
        EXPECT_TRUE(Position::isValidIndex(i));
    }
}

TEST(PositionTest, NegativeIndexIsInvalid) {
    EXPECT_FALSE(Position::isValidIndex(-1));
}

TEST(PositionTest, Index9IsInvalid) {
    EXPECT_FALSE(Position::isValidIndex(9));
}

TEST(PositionTest, ValidRowColAreValid) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            EXPECT_TRUE(Position::isValidRowCol(r, c));
        }
    }
}

TEST(PositionTest, NegativeRowIsInvalid) {
    EXPECT_FALSE(Position::isValidRowCol(-1, 0));
}

TEST(PositionTest, NegativeColIsInvalid) {
    EXPECT_FALSE(Position::isValidRowCol(0, -1));
}

TEST(PositionTest, Row3IsInvalid) {
    EXPECT_FALSE(Position::isValidRowCol(3, 0));
}

TEST(PositionTest, Col3IsInvalid) {
    EXPECT_FALSE(Position::isValidRowCol(0, 3));
}

} // namespace game::core
