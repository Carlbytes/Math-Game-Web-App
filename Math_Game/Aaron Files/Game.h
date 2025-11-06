#pragma once

// Returns the index (0..2) of the option closest to target
int findClosestIndex(int target, const int options[3]);

// Validates if the choice is within valid range (1-3)
bool isValidChoice(int choice);

// Generates random number between min and max (inclusive)
int generateRandomNumber(int min, int max);

// Fills the options array with random numbers
void generateOptions(int options[3], int min, int max);