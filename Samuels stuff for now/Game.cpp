#include "game.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <cmath>   // For std::abs
#include <sstream> // For std::ostringstream

// --- Helper Functions (from your colleague's code) ---
// These are kept internal to this .cpp file

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

int generateRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void generateOptions(int options[3], int min, int max) {
    for (int i = 0; i < 3; ++i) {
        options[i] = generateRandomNumber(min, max);
    }
}

// --- GameEasy Class Implementation ---

/**
 * @brief Initializes the game's random number generator.
 */
void GameEasy::initialize() {
    // Seed the random number generator once
    srand(static_cast<unsigned int>(time(0))); 
}

/**
 * @brief Generates a new "Closest Number" question.
 */
crow::json::wvalue GameEasy::get_question() {
    const int MIN_NUMBER = 1;
    const int MAX_NUMBER = 100;

    int target = generateRandomNumber(MIN_NUMBER, MAX_NUMBER);
    int options[3];
    generateOptions(options, MIN_NUMBER, MAX_NUMBER);
    
    int closestIndex = findClosestIndex(target, options);
    int correctAnswer = options[closestIndex];

    // Create the question string
    std::ostringstream oss;
    oss << "Target: " << target << ". Which is closest?";
    
    crow::json::wvalue res;
    res["question"] = oss.str();
    res["answer"] = correctAnswer; // The correct answer

    // Add the orbs as a JSON list
    crow::json::wvalue::list orb_list;
    orb_list.push_back(options[0]);
    orb_list.push_back(options[1]);
    orb_list.push_back(options[2]);
    res["orbs"] = std::move(orb_list);
    
    return res;
}

/**
 * @brief Checks if a user's answer is correct.
 * (This function logic is unchanged)
 */
crow::json::wvalue GameEasy::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}