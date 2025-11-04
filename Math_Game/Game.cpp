#include "game.h"
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

/**
 * @brief Initializes the game's random number generator.
 */
void Game::initialize() {
    // Seed the random number generator once
    srand(time(nullptr));
}

/**
 * @brief Generates a new math question.
 */
crow::json::wvalue Game::get_question() {
    int a = rand() % 10 + 1;
    int b = rand() % 10 + 1;
    std::string question = "What is " + std::to_string(a) + " + " + std::to_string(b) + "?";
    
    crow::json::wvalue res;
    res["question"] = question;
    res["answer"] = a + b; // The correct answer
    
    return res;
}

/**
 * @brief Checks if a user's answer is correct.
 */
crow::json::wvalue Game::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}
