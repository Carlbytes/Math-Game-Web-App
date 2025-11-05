//#include "pch.h"
#include "Game.h"
#include <cstdlib>
#include <cmath>

int findClosestIndex(int target, const int options[3]) {
    int closestIndex = 0;
    int smallestDiff = std::abs(options[0] - target);

    for (int i = 1; i < 3; ++i) {
        int diff = std::abs(options[i] - target);
        if (diff < smallestDiff) {
            smallestDiff = diff;
            closestIndex = i;
        }
    }
    return closestIndex;
}

bool isValidChoice(int choice) {
    return choice >= 1 && choice <= 3;
}

int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void generateOptions(int options[3], int min, int max) {
    for (int i = 0; i < 3; ++i) {
        options[i] = generateRandomNumber(min, max);
    }
}