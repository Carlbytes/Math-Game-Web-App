//#include "pch.h"
#include "game.h" // <-- This is the fix (was "Game.h")
#include <cstdlib>
#include <cmath>
#include <crow.h>

// ===========================================
// GameEasy Class Functions
// ===========================================

void GameEasy::initialize() {
    // Seed the random number generator once
    srand(time(nullptr));
}

crow::json::wvalue GameEasy::get_question() {
    const int MIN_NUMBER = 1;
    const int MAX_NUMBER = 100;

    int target = generateRandomNumber(MIN_NUMBER, MAX_NUMBER);
    int options[3];
    generateOptions(options, MIN_NUMBER, MAX_NUMBER);

    int closestIndex = findClosestIndex(target, options);
    int answer = options[closestIndex];

    crow::json::wvalue res;
    res["target"] = target;
    res["options"][0] = options[0];
    res["options"][1] = options[1];
    res["options"][2] = options[2];
    res["answer"] = answer; // The correct (closest) number

    return res;
}

crow::json::wvalue GameEasy::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}


// ===========================================
// GameEasy Helper Functions
// ===========================================

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