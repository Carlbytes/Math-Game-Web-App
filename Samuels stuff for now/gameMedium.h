#ifndef GAMEMEDIUM_H
#define GAMEMEDIUM_H

#include <crow.h>

class GameMedium {
public:
    static void initialize();
    static crow::json::wvalue get_question();
    static crow::json::wvalue check_answer(int user_answer, int correct_answer);
};

#endif // GAMEMEDIUM_H