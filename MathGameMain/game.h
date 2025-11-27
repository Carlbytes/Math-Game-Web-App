#pragma once

#include "crow.h"

// class functions
// This class holds the main API functions for the easy game
class GameEasy {
public:
 static void initialize();
 static crow::json::wvalue get_question();
 static crow::json::wvalue check_answer(int user_answer, int correct_answer);
};

// helper functions
/**
 * @brief Returns the index (0..2) of the option closest to target
 */
int findClosestIndex(int target, const int options[3]);

/**
 * @brief Validates if the choice is within valid range (1-3)
 */
bool isValidChoice(int choice);

/**
 * @brief Generates random number between min and max (inclusive)
 */
int generateRandomNumber(int min, int max);

/**
 * @brief Fills the options array with random numbers
 */
void generateOptions(int options[3], int min, int max);