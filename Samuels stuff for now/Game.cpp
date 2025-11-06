#include "game.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <string>  // For std::to_string
#include <algorithm> // For std::swap


/**
 * @brief Initializes the game's random number generator.
 */

void GameEasy::initialize() {
    // Seed the random number generator once
    srand(time(nullptr));
}

/**
 * @brief Generates a new "Easy" math question.
 */
crow::json::wvalue GameEasy::get_question() {
    int a, b;
    std::string operation = " + ";
    int correct_answer;

    // Easy (level 1 or default)
    // Original logic: Addition with numbers up to 10
    a = rand() % 10 + 1;
    b = rand() % 10 + 1;
    correct_answer = a + b;
    
    std::string question = "What is " + std::to_string(a) + operation + std::to_string(b) + "?";
    
    crow::json::wvalue res;
    res["question"] = question;
    res["answer"] = correct_answer; // The correct answer
    
    return res;
}

/**
 * @brief Checks if a user's answer is correct.
 */
crow::json::wvalue GameEasy::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}