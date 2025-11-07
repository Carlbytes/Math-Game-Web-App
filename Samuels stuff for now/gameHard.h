#ifndef GAMEHARD_H
#define GAMEHARD_H

#include "crow.h"

class GameHard {
public:
    static void initialize();
    static crow::json::wvalue get_question();
    static crow::json::wvalue check_answer(int user_answer, int correct_answer);
};

#endif // GAMEHARD_H