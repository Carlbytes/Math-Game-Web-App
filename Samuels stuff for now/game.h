#ifndef GAME_H
#define GAME_H

#include <crow.h>

class GameEasy {
private:
    

public:
    /**
     * @brief Initializes the game's random number generator.
     */
    static void initialize();


    /**
     * @brief Generates a new "Easy" math question.
     */
    static crow::json::wvalue get_question();

    /**
     * @brief Checks if a user's answer is correct.
     */
    static crow::json::wvalue check_answer(int user_answer, int correct_answer);
};

#endif // GAME_H