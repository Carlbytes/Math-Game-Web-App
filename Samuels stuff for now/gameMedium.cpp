#include "gameMedium.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

void GameMedium::initialize() {
    // Seeded by main, but good to have
    srand(time(nullptr)); 
}

crow::json::wvalue GameMedium::get_question() {
    int a, b;
    std::string operation = " + ";
    int correct_answer;

    // Normal (level 2)
    // Numbers up to 50, simple multiplication
    a = rand() % 50 + 1;
    b = rand() % 10 + 1;
    
    if (rand() % 2 == 0) {
        // Addition
        correct_answer = a + b;
    } else {
        // Multiplication
        operation = " * ";
        correct_answer = a * b;
    }
    
    std::string question = "What is " + std::to_string(a) + operation + std::to_string(b) + "?";
    
    crow::json::wvalue res;
    res["question"] = question;
    res["answer"] = correct_answer;
    
    return res;
}

crow::json::wvalue GameMedium::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}