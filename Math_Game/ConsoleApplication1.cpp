#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

// This minigame selected a number at random and then gives you three options. You must pick the option closest to the selected number.

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    int target = rand() % 100 + 1;  // Target number between 1 and 100
    int options[3];

    // Generate 3 random options (1–100)
    for (int i = 0; i < 3; ++i) {
        options[i] = rand() % 100 + 1;
    }

    cout << "=== Closest Number Game ===" << endl;
    cout << "Target number: " << target << endl;
    cout << "Which of the following is closest?" << endl;
    cout << "1) " << options[0] << endl;
    cout << "2) " << options[1] << endl;
    cout << "3) " << options[2] << endl;

    int choice;
    cout << "Enter your choice (1-3): ";
    cin >> choice;

    if (choice < 1 || choice > 3) {
        cout << "Invalid choice!" << endl;
        return 0;
    }

    int chosen = options[choice - 1];

    // Find which option is actually closest
    int closestIndex = 0;
    int smallestDiff = abs(options[0] - target);
    for (int i = 1; i < 3; ++i) {
        int diff = abs(options[i] - target);
        if (diff < smallestDiff) {
            smallestDiff = diff;
            closestIndex = i;
        }
    }

    cout << "\nYou chose: " << chosen << endl;
    cout << "The closest number was: " << options[closestIndex] << endl;

    if (choice - 1 == closestIndex) {
        cout << "Correct! You picked the closest number." << endl;
    }
    else {
        cout << "Not quite. Better luck next time!" << endl;
    }

    return 0;
}