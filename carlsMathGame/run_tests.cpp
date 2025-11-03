//#include <iostream>
//#include <random>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <cassert> // <-- Include for testing
//
//// --- Paste your (refactored) functions here ---
//// ... (getRandomInt, generateProblem, generateOrbs, etc.)
//
//// A helper function to find an item in a vector
//bool contains(const std::vector<int>& vec, int value) {
//    return std::find(vec.begin(), vec.end(), value) != vec.end();
//}
//
//void test_DivisionProblem() {
//    std::cout << "Running test: DivisionProblem..." << std::endl;
//    std::mt19937 test_rng(123); // Use a fixed seed
//
//    // Test 100 division problems
//    for (int i = 0; i < 100; ++i) {
//        MathProblem p = generateProblem(test_rng);
//        if (p.op == '/') {
//            // Test that division is always a whole number
//            assert(p.num1 % p.num2 == 0);
//            // Test that the answer is correct
//            assert(p.num1 / p.num2 == p.correctAnswer);
//        }
//    }
//    std::cout << "PASSED!\n";
//}
//
//void test_OrbGeneration() {
//    std::cout << "Running test: OrbGeneration..." << std::endl;
//    std::mt19937 test_rng(456); // A different fixed seed
//    int correctAnswer = 50;
//
//    std::vector<int> orbs = generateOrbs(correctAnswer, test_rng);
//
//    // Test 1: Are there exactly 3 orbs?
//    assert(orbs.size() == 3);
//
//    // Test 2: Is the correct answer (50) in the orbs?
//    assert(contains(orbs, 50));
//
//    // Test 3: Are the other two orbs different?
//    // We know with seed 456, the orbs are [50, 47, 52]
//    // and they shuffle to [47, 52, 50]
//    assert(orbs[0] == 47);
//    assert(orbs[1] == 52);
//    assert(orbs[2] == 50);
//
//    // Test 4: Are the distractors within 1-5 units?
//    assert(std::abs(orbs[0] - correctAnswer) >= 1 && std::abs(orbs[0] - correctAnswer) <= 5);
//    assert(std::abs(orbs[1] - correctAnswer) >= 1 && std::abs(orbs[1] - correctAnswer) <= 5);
//
//    std::cout << "PASSED!\n";
//}
//
//int main() {
//    test_DivisionProblem();
//    test_OrbGeneration();
//
//    std::cout << "\nAll tests passed!\n";
//    return 0;
//}