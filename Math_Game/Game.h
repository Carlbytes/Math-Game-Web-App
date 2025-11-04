#ifndef GAME_H
#define GAME_H

#include <string>
#include "crow_all.h" // We need this for crow::json::wvalue

// We will put all game functions inside a 'Game' namespace
// to avoid name collisions.
namespace Game {

    /**
     * @brief Initializes the game's random number generator.
     * Must be called once at server startup.
     */
    void initialize();

    /**
     * @brief Generates a new math question.
     * @return A crow::json::wvalue object containing the question text
     * and the correct answer.
     */
    crow::json::wvalue get_question();

    /**
     * @brief Checks if a user's answer is correct.
     * @param user_answer The answer submitted by the user.
     * @param correct_answer The true answer that was stored.
     * @return A crow::json::wvalue object with a boolean "correct" field.
     */
    crow::json::wvalue check_answer(int user_answer, int correct_answer);

} // namespace Game

#endif // GAME_H
