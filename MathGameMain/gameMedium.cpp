#include "gameMedium.h"
#include <random>       // For std::mt19937
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>        // For std::time
#include <sstream>      // For std::ostringstream

// A helper struct to pass problem data around easily
struct MathProblem {
    int num1;
    int num2;
    char op;
    int correctAnswer;
};


// static member def
// This RNG will be initialized once and reused
static std::mt19937 rng;

// helper functions
// These are kept internal to this .cpp file

// generates an integer within a range
int getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// generates the math for an addition problem
MathProblem generateAdditionProblem() {
    int num1 = getRandomInt(1, 20);
    int num2 = getRandomInt(1, 20);
    int answer = num1 + num2;
    return MathProblem{ num1, num2, '+', answer };
}

// generates the math for a subtraction problem
MathProblem generateSubtractionProblem() {
    int num1 = getRandomInt(1, 20);
    int num2 = getRandomInt(1, 20);
    // Ensure positive-ish results, but allow negatives
    if (num1 < num2) {
        std::swap(num1, num2);
    }
    int answer = num1 - num2;
    return MathProblem{ num1, num2, '-', answer };
}

// generates the logic for a multiplication problem
MathProblem generateMultiplicationProblem() {
    int num1 = getRandomInt(1, 12); // Kept smaller for multiplication
    int num2 = getRandomInt(1, 12);
    int answer = num1 * num2;
    return MathProblem{ num1, num2, '*', answer };
}

// generates the logic for a division problem
MathProblem generateDivisionProblem() {
    int num2 = getRandomInt(2, 12); // Divisor
    int answer = getRandomInt(2, 12); // Result
    int num1 = num2 * answer; // Dividend
    return MathProblem{ num1, num2, '/', answer };
}

// main dispatcher for the random problem solutions
MathProblem generateProblem() {
    int problemType = getRandomInt(0, 3); // 0=Add, 1=Sub, 2=Mul, 3=Div

    switch (problemType) {
    case 0:  return generateAdditionProblem();
    case 1:  return generateSubtractionProblem();
    case 2:  return generateMultiplicationProblem();
    case 3:  return generateDivisionProblem();
    default: return generateAdditionProblem(); // Fallback
    }
}

// generates three answer orbs, one correct two wrong
std::vector<int> generateOrbs(int correctAnswer) {
    int offset1 = getRandomInt(1, 5);
    int offset2 = getRandomInt(1, 5);

    // Ensure offsets aren't the same
    while (offset1 == offset2) {
        offset2 = getRandomInt(1, 5);
    }

    std::vector<int> orbs;
    orbs.push_back(correctAnswer);
    orbs.push_back(correctAnswer - offset1);
    // Ensure the third orb is different from the second
    int thirdOrb = correctAnswer + offset2;
    while(thirdOrb == (correctAnswer - offset1)) {
        thirdOrb = correctAnswer + getRandomInt(1, 5);
    }
    orbs.push_back(thirdOrb);

    // Shuffle the orbs
    std::shuffle(orbs.begin(), orbs.end(), rng);
    return orbs;
}


// main class implementation

// initialises a random number generator
void GameMedium::initialize() {
    // Use a time-based seed for a real game
    rng = std::mt19937(static_cast<unsigned int>(std::time(0)));
}

// generates a new question and three orbs
crow::json::wvalue GameMedium::get_question() {
    // 1. Generate the problem
    MathProblem problem = generateProblem();

    // 2. Generate the answer orbs
    std::vector<int> orbs = generateOrbs(problem.correctAnswer);

    // 3. Create the question string
    std::ostringstream oss;
    oss << "What is " << problem.num1 << " " << problem.op << " " << problem.num2 << " = ??";
    std::string question_str = oss.str();

    // 4. Build the JSON response
    crow::json::wvalue res;
    res["question"] = question_str;
    res["answer"] = problem.correctAnswer; // The correct answer

    // 5. Add the orbs as a JSON list
    crow::json::wvalue::list orb_list;
    for(int orb_val : orbs) {
        orb_list.push_back(orb_val);
    }
    res["orbs"] = std::move(orb_list);
    
    return res;
}

// checks if the user is correct
crow::json::wvalue GameMedium::check_answer(int user_answer, int correct_answer) {
    crow::json::wvalue res;
    res["correct"] = (user_answer == correct_answer);
    return res;
}