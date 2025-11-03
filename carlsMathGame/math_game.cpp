#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <limits> // For std::cin.ignore
#include <thread>
#include <chrono>

// A helper struct to pass problem data around easily
struct MathProblem {
    int num1;
    int num2;
    char op;
    int correctAnswer;
};

// ===================================================================
// .Random System (SOF-15)
// This utility function is the core of the random system.
// ===================================================================
/**
 * @brief Generates a random integer within a specified range [min, max].
 * @param rng A reference to the random number engine.
 * @param min The minimum possible value (inclusive).
 * @param max The maximum possible value (inclusive).
 * @return A random integer in the range.
 */
int getRandomInt(std::mt19937& rng, int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// ===================================================================
// Adding System (SOF-21)
// Generates the logic for an addition problem.
// ===================================================================
MathProblem generateAdditionProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 + num2;
    return MathProblem{ num1, num2, '+', answer };
}

// ===================================================================
// Subtraction System (SOF-22)
// Generates the logic for a subtraction problem.
// ===================================================================
MathProblem generateSubtractionProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 - num2;
    return MathProblem{ num1, num2, '-', answer };
}

// ===================================================================
// Multiplication System (SOF-24)
// Generates the logic for a multiplication problem.
// ===================================================================
MathProblem generateMultiplicationProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 * num2;
    return MathProblem{ num1, num2, '*', answer };
}

// ===================================================================
// Division System (SOF-23)
// Generates the logic for a division problem (ensuring whole numbers).
// ===================================================================
MathProblem generateDivisionProblem(std::mt19937& rng) {
    int num2 = getRandomInt(rng, 2, 12); // Divisor
    int answer = getRandomInt(rng, 2, 12); // Result
    int num1 = num2 * answer; // Dividend
    return MathProblem{ num1, num2, '/', answer };
}

/**
 * @brief Main dispatcher for the ".Random System" (SOF-15).
 * It randomly selects one of the specific problem systems.
 * @param rng A reference to the random number engine.
 * @return A complete MathProblem struct.
 */
MathProblem generateProblem(std::mt19937& rng) {
    int problemType = getRandomInt(rng, 0, 3); // 0=Add, 1=Sub, 2=Mul, 3=Div

    switch (problemType) {
    case 0:  return generateAdditionProblem(rng);
    case 1:  return generateSubtractionProblem(rng);
    case 2:  return generateMultiplicationProblem(rng);
    case 3:  return generateDivisionProblem(rng);
    default: return generateAdditionProblem(rng); // Fallback
    }
}

/**
 * @brief Generates the three answer orbs (correct + 2 distractors).
 * This is also part of the ".Random System" (SOF-15).
 * @param rng A reference to the random number engine.
 * @param correctAnswer The correct answer (Z).
 * @return A shuffled vector of 3 integer choices.
 */
std::vector<int> generateOrbs(std::mt19937& rng, int correctAnswer) {
    int offset1 = getRandomInt(rng, 1, 5);
    int offset2 = getRandomInt(rng, 1, 5);

    // Ensure offsets aren't the same
    while (offset1 == offset2) {
        offset2 = getRandomInt(rng, 1, 5);
    }

    std::vector<int> orbs;
    orbs.push_back(correctAnswer);
    orbs.push_back(correctAnswer - offset1);
    orbs.push_back(correctAnswer + offset2);

    // Shuffle the orbs
    std::shuffle(orbs.begin(), orbs.end(), rng);
    return orbs;
}

/**
 * @brief Handles a single round of the math game.
 * This function now coordinates the other systems.
 * @param rng A reference to the random number engine.
 * @return True if the user was correct, false otherwise.
 */
bool playRound(std::mt19937& rng) {
    // 1. Generate the problem (using the new modular functions)
    MathProblem problem = generateProblem(rng);

    // 2. Generate the answer orbs
    std::vector<int> orbs = generateOrbs(rng, problem.correctAnswer);

    // 3. Display the problem and orbs
    std::cout << "=================================\n";
    std::cout << "What is... \n\n";
    std::cout << "  " << problem.num1 << " " << problem.op << " " << problem.num2 << " = ??\n\n";
    std::cout << "Choose an orb:\n";
    for (size_t i = 0; i < orbs.size(); ++i) {
        std::cout << "  " << (i + 1) << ". [ " << orbs[i] << " ]\n";
    }
    std::cout << "=================================\n";

    // 4. Get user input
    int userChoice;
    std::cout << "Your choice (1, 2, or 3): ";
    while (!(std::cin >> userChoice) || userChoice < 1 || userChoice > 3) {
        std::cout << "Invalid input. Please enter 1, 2, or 3: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // 5. Check the answer
    int chosenAnswer = orbs[userChoice - 1];

    if (chosenAnswer == problem.correctAnswer) {
        std::cout << "\n>>> Correct! <<<\n\n";
        return true;
    }
    else {
        std::cout << "\n>>> Wrong! The correct answer was " << problem.correctAnswer << ". <<<\n\n";
        return false;
    }
}

// ===================================================================
// Exit System (SOF-26)
// Handles the logic for asking the user to play again.
// ===================================================================
bool askToPlayAgain() {
    char playAgain;
    std::cout << "Play another round? (y/n): ";
    std::cin >> playAgain;

    // Clear any extra input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return (playAgain == 'y' || playAgain == 'Y');
}

/**
 * @brief Initializes the random number generator.
 * Part of the ".Random System" (SOF-15).
 */
std::mt19937 initializeRNG() {
    // Use a fixed seed (42) for predictable testing
    // return std::mt19937(42); 

    // Use a time-based seed for a real game
    return std::mt19937(static_cast<unsigned int>(std::time(0)));
}

// ===================================================================
// Main Game Loop
// ===================================================================
#ifndef RUNNING_TESTS
int main() {
    // 1. Seed the random number generator
    std::mt19937 rng = initializeRNG();

    std::cout << "Welcome to the C++ Math Orb Game!\n";
    int score = 0;
    int rounds = 0;
    bool keepPlaying = true;

    while (keepPlaying) {
        rounds++;
        if (playRound(rng)) {
            score++;
        }

        std::cout << "Your score: " << score << " / " << rounds << "\n";

        // 2. Use the "Exit System"
        keepPlaying = askToPlayAgain();
    }

    std::cout << "Thanks for playing! Final score: " << score << " / " << rounds << "\n";


	std::cout << "Program auto will exit in 5 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "4...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "3...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "2...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "1...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Goodbye!\n";



    return 0;
}
#endif // RUNNING_TESTS