//#include "pch.h"
#include "gtest/gtest.h"
#include "C:\Users\aaron\OneDrive - TUS MM\THIRD YEAR\Visualstudio files\ConsoleApplication1\Game.cpp"
#include "pch.h"

TEST(FindClosestIndex, ExactMatch) {
    int options[3] = { 10, 20, 30 };
    EXPECT_EQ(findClosestIndex(10, options), 0);
}

TEST(FindClosestIndex, ClosestToMiddle) {
    int options[3] = { 10, 20, 30 };
    EXPECT_EQ(findClosestIndex(19, options), 1);
}

TEST(FindClosestIndex, TieBreakerReturnsFirstOption) {
    int options[3] = { 10, 20, 30 };

    EXPECT_EQ(findClosestIndex(15, options), 0);
}

TEST(FindClosestIndexTests, HandlesNegativeNumbers) {
    int options[3] = { -10, 0, 10 };
    EXPECT_EQ(findClosestIndex(-5, options), 0);
}

TEST(IsValidChoiceTest, ValidInputsCases) {
    EXPECT_TRUE(isValidChoice(1));
    EXPECT_TRUE(isValidChoice(2));
    EXPECT_TRUE(isValidChoice(3));
}

TEST(IsValidChoice, InvalidInputs) {
    EXPECT_FALSE(isValidChoice(0));
    EXPECT_FALSE(isValidChoice(4));
    EXPECT_FALSE(isValidChoice(-1));
}

TEST(GenerateRandomNumber, WithinRange) {
    for (int i = 0; i < 100; i++) {
        int result = generateRandomNumber(1, 100);
        EXPECT_GE(result, 1);
        EXPECT_LE(result, 100);
    }
}

TEST(GenerateOptions, GeneratesThreeNumbers) {
    int options[3];
    generateOptions(options, 1, 100);

    for (int i = 0; i < 3; i++) {
        EXPECT_GE(options[i], 1);
        EXPECT_LE(options[i], 100);
    }
}