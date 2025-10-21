#include <iostream>  // For console input/output (cin, cout)
#include <random>    // For modern random number generation
#include <vector>    // To store the answer orbs
#include <string>    // To use std::string
#include <algorithm> // For std::shuffle
#include <ctime>     // To seed the random number generator

/**
 * @brief Generates a random integer within a specified range [min, max].
 * @param rng A reference to the random number engine.
 * @param min The minimum possible value (inclusive).
 * @param max The maximum possible value (inclusive).
 * @return A random integer in the range.
 */
int getRandomInt(std::mt19937& rng, int min, int max) {
    // Create a distribution that will produce uniform integers in the given range
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

/**
 * @brief Handles a single round of the math game.
 * @param rng A reference to the random number engine.
 * @return True if the user was correct, false otherwise.
 */
bool playRound(std::mt19937& rng) {
    // 1. Generate the problem
    const char operators[] = { '+', '-', '*', '/' };
    char op = operators[getRandomInt(rng, 0, 3)]; // Pick a random operator

    int num1, num2;
    int correctAnswer;

    // Handle division separately to ensure whole numbers
    if (op == '/') {
        // To avoid fractions, we'll work backwards
        num2 = getRandomInt(rng, 2, 12); // Divisor (avoid 0 or 1)
        correctAnswer = getRandomInt(rng, 2, 12); // The result
        num1 = num2 * correctAnswer; // The dividend
    }
    else {
        // For +, -, *
        num1 = getRandomInt(rng, 1, 20);
        num2 = getRandomInt(rng, 1, 20);

        switch (op) {
        case '+': correctAnswer = num1 + num2; break;
        case '-': correctAnswer = num1 - num2; break;
        case '*': correctAnswer = num1 * num2; break;
            // Division is handled above, but default is good practice
        default:  correctAnswer = 0; break;
        }
    }

    // 2. Generate the 3 answer orbs
    // Per your requirements: Z, Z - (1-5), Z + (1-5)
    int offset1 = getRandomInt(rng, 1, 5);
    int offset2 = getRandomInt(rng, 1, 5);

    // Make sure offsets don't accidentally cancel out or create duplicates
    // e.g., if offset1 is 2, don't make offset2 -2
    while (offset1 == offset2 || (correctAnswer - offset1) == (correctAnswer + offset2)) {
        offset2 = getRandomInt(rng, 1, 5);
    }

    std::vector<int> orbs;
    orbs.push_back(correctAnswer);           // The correct answer (Z)
    orbs.push_back(correctAnswer - offset1); // Distractor 1
    orbs.push_back(correctAnswer + offset2); // Distractor 2

    // 3. Shuffle the orbs
    // This is vital so the correct answer isn't always in the same place!
    std::shuffle(orbs.begin(), orbs.end(), rng);

    // 4. Display the problem and orbs
    std::cout << "=================================\n";
    std::cout << "What is... \n\n";
    std::cout << "  " << num1 << " " << op << " " << num2 << " = ??\n\n";

    std::cout << "Choose an orb:\n";
    for (size_t i = 0; i < orbs.size(); ++i) {
        std::cout << "  " << (i + 1) << ". [ " << orbs[i] << " ]\n";
    }
    std::cout << "=================================\n";

    // 5. Get user input
    int userChoice;
    std::cout << "Your choice (1, 2, or 3): ";
    while (!(std::cin >> userChoice) || userChoice < 1 || userChoice > 3) {
        std::cout << "Invalid input. Please enter 1, 2, or 3: ";
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(10000, '\n'); // Discard the bad input
    }

    // 6. Check the answer
    // We adjust userChoice (1-based) to an index (0-based)
    int chosenAnswer = orbs[userChoice - 1];

    if (chosenAnswer == correctAnswer) {
        std::cout << "\n>>> Correct! <<<\n\n";
        return true;
    }
    else {
        std::cout << "\n>>> Wrong! The correct answer was " << correctAnswer << ". <<<\n\n";
        return false;
    }
}

int main() {
    // 1. Seed the random number generator
    // This should only be done ONCE at the start of the program.
    std::mt19937 rng(static_cast<unsigned int>(std::time(0)));

    std::cout << "Welcome to the C++ Math Orb Game!\n";
    int score = 0;
    int rounds = 0;

    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        rounds++;
        if (playRound(rng)) {
            score++;
        }

        std::cout << "Your score: " << score << " / " << rounds << "\n";
        std::cout << "Play another round? (y/n): ";
        std::cin >> playAgain;

        // Clear any extra input
        std::cin.ignore(10000, '\n');
    }

    std::cout << "Thanks for playing! Final score: " << score << " / " << rounds << "\n";
    return 0;
}