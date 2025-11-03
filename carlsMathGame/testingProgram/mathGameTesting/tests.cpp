#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert> // The testing library!
#include <limits>  

// ===================================================================
// SECTION 1: CODE UNDER TEST
// (This is your game's logic)
// ===================================================================

struct MathProblem {
    int num1;
    int num2;
    char op;
    int correctAnswer;
};

int getRandomInt(std::mt19937& rng, int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

MathProblem generateAdditionProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 + num2;
    return MathProblem{ num1, num2, '+', answer };
}

MathProblem generateSubtractionProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 - num2;
    return MathProblem{ num1, num2, '-', answer };
}

MathProblem generateMultiplicationProblem(std::mt19937& rng) {
    int num1 = getRandomInt(rng, 1, 20);
    int num2 = getRandomInt(rng, 1, 20);
    int answer = num1 * num2;
    return MathProblem{ num1, num2, '*', answer };
}

MathProblem generateDivisionProblem(std::mt19937& rng) {
    int num2 = getRandomInt(rng, 2, 12); // Divisor
    int answer = getRandomInt(rng, 2, 12); // Result
    int num1 = num2 * answer; // Dividend
    return MathProblem{ num1, num2, '/', answer };
}

MathProblem generateProblem(std::mt19937& rng) {
    int problemType = getRandomInt(rng, 0, 3); // 0=Add, 1=Sub, 2=Mul, 3=Div
    switch (problemType) {
    case 0:  return generateAdditionProblem(rng);
    case 1:  return generateSubtractionProblem(rng);
    case 2:  return generateMultiplicationProblem(rng);
    case 3:  return generateDivisionProblem(rng);
    default: return generateAdditionProblem(rng);
    }
}

std::vector<int> generateOrbs(std::mt19937& rng, int correctAnswer) {
    int offset1 = getRandomInt(rng, 1, 5);
    int offset2 = getRandomInt(rng, 1, 5);
    while (offset1 == offset2) {
        offset2 = getRandomInt(rng, 1, 5);
    }
    std::vector<int> orbs = { correctAnswer, correctAnswer - offset1, correctAnswer + offset2 };
    std::shuffle(orbs.begin(), orbs.end(), rng);
    return orbs;
}

// ===================================================================
// SECTION 2: TEST FUNCTIONS (NOW WITH CORRECT ASSERTIONS)
// ===================================================================

/**
 * @brief Tests the core utility (getRandomInt)
 */
void test_RandomSystem_Utilities() {
    std::cout << "Running test: [Random System - Utilities]...\n";
    std::mt19937 test_rng(42);
    // Your discovered numbers:
    assert(getRandomInt(test_rng, 1, 100) == 38);
    assert(getRandomInt(test_rng, 1, 100) == 80);
    assert(getRandomInt(test_rng, 1, 100) == 96);
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the Adding System (SOF-21)
 */
void test_AdditionSystem() {
    std::cout << "Running test: [Adding System]...\n";
    std::mt19937 test_rng(42);
    MathProblem p = generateAdditionProblem(test_rng);
    // Your discovered numbers:
    assert(p.num1 == 8);
    assert(p.num2 == 16);
    assert(p.correctAnswer == 24);
    assert(p.op == '+');
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the Subtraction System (SOF-22)
 */
void test_SubtractionSystem() {
    std::cout << "Running test: [Subtraction System]...\n";
    std::mt19937 test_rng(42);
    MathProblem p = generateSubtractionProblem(test_rng);
    // Your discovered numbers:
    assert(p.num1 == 8);
    assert(p.num2 == 16);
    assert(p.correctAnswer == -8);
    assert(p.op == '-');
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the Multiplication System (SOF-24)
 */
void test_MultiplicationSystem() {
    std::cout << "Running test: [Multiplication System]...\n";
    std::mt19937 test_rng(42);
    MathProblem p = generateMultiplicationProblem(test_rng);
    // Your discovered numbers:
    assert(p.num1 == 8);
    assert(p.num2 == 16);
    assert(p.correctAnswer == 128);
    assert(p.op == '*');
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the Division System (SOF-23)
 */
void test_DivisionSystem() {
    std::cout << "Running test: [Division System]...\n";
    std::mt19937 test_rng(42);
    MathProblem p = generateDivisionProblem(test_rng);
    // Your discovered numbers:
    assert(p.num2 == 6);
    assert(p.correctAnswer == 10);
    assert(p.num1 == 60);
    assert(p.op == '/');
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the Orb Generation logic (.Random System SOF-15)
 */
void test_RandomSystem_OrbGeneration() {
    std::cout << "Running test: [Random System - Orb Generation]...\n";
    std::mt19937 test_rng(42);
    std::vector<int> orbs = generateOrbs(test_rng, 100);
    // Your discovered numbers:
    assert(orbs.size() == 3);
    assert(orbs[0] == 98);
    assert(orbs[1] == 104);
    assert(orbs[2] == 100);
    std::cout << "  PASSED!\n";
}

/**
 * @brief Tests the main problem dispatcher (.Random System SOF-15)
 */
void test_RandomSystem_Dispatcher() {
    std::cout << "Running test: [Random System - Dispatcher]...\n";
    std::mt19937 test_rng(42);
    MathProblem p1 = generateProblem(test_rng);
    MathProblem p2 = generateProblem(test_rng);
    MathProblem p3 = generateProblem(test_rng);
    // Your discovered ops:
    assert(p1.op == '-');
    assert(p2.op == '+');
    assert(p3.op == '*');
    std::cout << "  PASSED!\n";
}

// ===================================================================
// SECTION 3: TEST RUNNER
// ===================================================================

int main() {
    std::cout << "===== RUNNING ALL UNIT TESTS =====\n\n";

    // Run all test functions
    test_RandomSystem_Utilities();
    test_AdditionSystem();
    test_SubtractionSystem();
    test_MultiplicationSystem();
    test_DivisionSystem();
    test_RandomSystem_OrbGeneration();
    test_RandomSystem_Dispatcher();

    std::cout << "\n==================================\n";
    std::cout << "All tests passed successfully! \n";
    std::cout << "==================================\n";

    return 0; // Return 0 indicates success
}