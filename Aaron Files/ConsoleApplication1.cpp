#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
using namespace std;


#ifndef RUNNING_TESTS
int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    const int MIN_NUMBER = 1;
    const int MAX_NUMBER = 100;

    int target = generateRandomNumber(MIN_NUMBER, MAX_NUMBER);
    int options[3];
    generateOptions(options, MIN_NUMBER, MAX_NUMBER);

    cout << "=== Closest Number Game ===" << endl;
    cout << "Target number: " << target << endl;
    cout << "Which of the following is closest?" << endl;
    cout << "1) " << options[0] << endl;
    cout << "2) " << options[1] << endl;
    cout << "3) " << options[2] << endl;

    int choice;
    cout << "Enter your choice (1-3): ";
    cin >> choice;

    if (!isValidChoice(choice)) {
        cout << "Invalid choice!" << endl;
        return 0;
    }

    int chosen = options[choice - 1];
    int closestIndex = findClosestIndex(target, options);

    cout << "\nYou chose: " << chosen << endl;
    cout << "The closest number was: " << options[closestIndex] << endl;

    if (choice - 1 == closestIndex) {
        cout << "Correct! You picked the closest number." << endl;
    }
    else {
        cout << "Incorrect." << endl;
    }

    return 0;
}
#endif