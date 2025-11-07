#include "gameHard.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

void GameHard::initialize() {
    // Seeded by main, but good to have
    srand(time(nullptr)); 
}

crow::json::wvalue GameHard::get_question() {
    int a, b;
    std::string operation = " + ";
    int correct_answer;

    // Hard (level 3)
    // Numbers up to 100, multiplication and subtraction
    a = rand() % 100 + 1;
    b = rand() % 50 + 1;
    
    if (rand() % 2 == 0) {
        // Multiplication
        operation = " * ";
        correct_answer = a * b;
    } else {
        // Subtraction (ensure positive result)
        if (a < b) {
            std::swap(a, b); // Swap a and b
        }
        operation = " - ";
        correct_answer = a - b;
    }
    
    std::string question = "What is " + std::to_string(a) + operation + std::to_string(b) + "?";
    
    crow::json::wvalue res;
    res["question"] = question;
    res["answer"] = correct_answer;
    
    return res;
}

crow::json::wvalue GameHard::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}