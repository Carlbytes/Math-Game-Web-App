#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// Include all the game headers
#include "game.h"
#include "gameMedium.h"
#include "gameHard.h"

#include <string>

// =========================================================================
// PART 1: "Easy" Game Helper Functions (Boundary & Logic Tests)
// =========================================================================

TEST_CASE("Easy Game Helpers - isValidChoice") {
    SECTION("Valid choices") {
        REQUIRE(isValidChoice(1) == true);
        REQUIRE(isValidChoice(2) == true);
        REQUIRE(isValidChoice(3) == true);
    }
    SECTION("Invalid (boundary) choices") {
        REQUIRE(isValidChoice(0) == false);  // Boundary: too low
        REQUIRE(isValidChoice(4) == false);  // Boundary: too high
        REQUIRE(isValidChoice(-1) == false); // Invalid
    }
}

TEST_CASE("Easy Game Helpers - findClosestIndex") {
    int options[3] = {10, 20, 30};

    REQUIRE(findClosestIndex(12, options) == 0); // Closest to 10
    REQUIRE(findClosestIndex(19, options) == 1); // Closest to 20
    REQUIRE(findClosestIndex(40, options) == 2); // Closest to 30
    REQUIRE(findClosestIndex(15, options) == 0); // Midpoint, should pick first
    REQUIRE(findClosestIndex(25, options) == 1); // Midpoint, should pick first
}

// =========================================================================
// PART 2: Game Logic "API" Functions
// =========================================================================

// --- Helper function to convert a wvalue to a string for testing ---
std::string getJsonString(const crow::json::wvalue& val) {
    return val.dump();
}

TEST_CASE("GameEasy - API Functions") {
    GameEasy::initialize(); // Seed the generator

    SECTION("get_question returns a valid question structure") {
        crow::json::wvalue q = GameEasy::get_question();
        std::string json_string = getJsonString(q);

        // We check that the string contains the keys we expect.
        REQUIRE(!json_string.empty());
        REQUIRE(json_string.find("\"question\":") != std::string::npos);
        REQUIRE(json_string.find("\"orbs\":") != std::string::npos);
        REQUIRE(json_string.find("\"answer\":") != std::string::npos);
    }

    SECTION("check_answer validates correctly") {
        // We check the exact JSON string output
        REQUIRE(getJsonString(GameEasy::check_answer(10, 10)) == "{\"correct\":true}");
        REQUIRE(getJsonString(GameEasy::check_answer(10, 5)) == "{\"correct\":false}");
    }
}

TEST_CASE("GameMedium - API Functions") {
    GameMedium::initialize(); // Seed the generator

    SECTION("get_question returns a valid question structure with all operators") {

        // We must loop to ensure we hit all branches of the random
        // switch statement in generateProblem()
        bool hasAdd = false, hasSub = false, hasMul = false, hasDiv = false;

        for (int i = 0; i < 100; ++i) {
            crow::json::wvalue q = GameMedium::get_question();
            std::string json_string = getJsonString(q);

            REQUIRE(!json_string.empty());
            REQUIRE(json_string.find("\"question\":") != std::string::npos);
            REQUIRE(json_string.find("\"orbs\":") != std::string::npos);
            REQUIRE(json_string.find("\"answer\":") != std::string::npos);

            // Check which operator was generated
            if (json_string.find(" + ") != std::string::npos) hasAdd = true;
            if (json_string.find(" - ") != std::string::npos) hasSub = true;
            if (json_string.find(" * ") != std::string::npos) hasMul = true;
            if (json_string.find(" / ") != std::string::npos) hasDiv = true;
        }

        // Require that all 4 problem types were generated
        REQUIRE(hasAdd);
        REQUIRE(hasSub);
        REQUIRE(hasMul);
        REQUIRE(hasDiv);
    }

    SECTION("check_answer validates correctly") {
        REQUIRE(getJsonString(GameMedium::check_answer(25, 25)) == "{\"correct\":true}");
        REQUIRE(getJsonString(GameMedium::check_answer(-5, 12)) == "{\"correct\":false}");
    }
}

TEST_CASE("GameHard - API Functions") {
    GameHard::initialize(); // Seed the generator

    SECTION("get_question returns a valid question structure with all operators") {

        // We must loop to ensure we hit both branches of the random
        // if/else statement in get_question()
        bool hasMul = false, hasSub = false;

        for (int i = 0; i < 100; ++i) {
            crow::json::wvalue q = GameHard::get_question();
            std::string json_string = getJsonString(q);

            REQUIRE(!json_string.empty());
            REQUIRE(json_string.find("\"question\":") != std::string::npos);
            REQUIRE(json_string.find("\"answer\":") != std::string::npos);

            // Check which operator was generated
            if (json_string.find(" * ") != std::string::npos) hasMul = true;
            if (json_string.find(" - ") != std::string::npos) hasSub = true;
        }

        // Require that both problem types were generated
        REQUIRE(hasMul);
        REQUIRE(hasSub);
    }

    SECTION("check_answer validates correctly") {
        REQUIRE(getJsonString(GameHard::check_answer(100, 100)) == "{\"correct\":true}");
        REQUIRE(getJsonString(GameHard::check_answer(0, 1)) == "{\"correct\":false}");
    }
}