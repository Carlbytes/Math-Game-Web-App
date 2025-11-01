#include "gtest/gtest.h"
#include "../Game.h"

TEST(FindClosestIndex, ExactMatchFirst) {
    int options[3] = { 10, 20, 30 };
    EXPECT_EQ(findClosestIndex(10, options), 0);
}

TEST(FindClosestIndex, TiePrefersFirstOccurrence) {
    int options[3] = { 10, 20, 30 };
    // target 15 is equally distant from 10 and 20 -> picks 10 (first)
    EXPECT_EQ(findClosestIndex(15, options), 0);
}

TEST(FindClosestIndex, MiddleClosest) {
    int options[3] = { 1, 50, 100 };
    EXPECT_EQ(findClosestIndex(49, options), 1);
}

TEST(FindClosestIndex, ThirdClosest) {
    int options[3] = { 5, 6, 9 };
    EXPECT_EQ(findClosestIndex(10, options), 2);
}

TEST(FindClosestIndex, NegativeTarget) {
    int options[3] = { -10, 0, 10 };
    EXPECT_EQ(findClosestIndex(-5, options), 0);
}