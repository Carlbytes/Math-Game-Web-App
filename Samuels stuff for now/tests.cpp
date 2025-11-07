// This defines the main() for Catch2
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

// Include all the game headers
#include "game.h"
#include "gameMedium.h"
#include "gameHard.h"
#include "Database.h" // <--- ADDED
#include "crow.h"     // <--- ADDED

#include <string>
#include <memory>         // <--- ADDED
#include <unordered_set>  // <--- ADDED
#include <cstdio>         // <--- ADDED (for std::remove)

// --- Prototypes for static functions in main.cpp ---
// We must declare them here to make them visible to the test runner.
crow::response load_static_file(const std::string& path);
bool auth_check(const std::shared_ptr<Database>& db_ptr,
                const std::unordered_set<std::string>& public_paths,
                const std::string& url,
                const std::string& auth_header,
                crow::response& res);


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
    GameHard::initialize(); // Seed the.generator

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

// =========================================================================
// PART 3: Database.cpp Tests
// =========================================================================
TEST_CASE("Database Logic") {
    const std::string test_db_name = "test_db.sqlite";

    // Ensure a clean state before each section
    std::remove(test_db_name.c_str());

    SECTION("Create and Login User") {
        Database db(test_db_name);
        REQUIRE(db.create_user("testuser", "pass123") == true);
        REQUIRE(db.check_login("testuser", "pass123") == true);
    }

    SECTION("Duplicate User Creation Fails") {
        Database db(test_db_name);
        REQUIRE(db.create_user("testuser", "pass123") == true);
        REQUIRE(db.create_user("testuser", "pass456") == false); // Duplicate
    }

    SECTION("Check Wrong Password Fails") {
        Database db(test_db_name);
        REQUIRE(db.create_user("testuser", "pass123") == true);
        REQUIRE(db.check_login("testuser", "wrongpass") == false);
    }

    SECTION("Check Non-existent User Fails") {
        Database db(test_db_name);
        REQUIRE(db.check_login("nonexistent", "pass123") == false);
    }

    SECTION("Session Management") {
        Database db(test_db_name);
        db.create_user("session_user", "pass");

        std::string token = db.create_session("session_user");
        REQUIRE(!token.empty());

        SECTION("Valid token works") {
            REQUIRE(db.validate_session(token) == true);
        }
        SECTION("Invalid token fails") {
            REQUIRE(db.validate_session("bad_token_12345") == false);
        }
        SECTION("Empty token fails") {
             REQUIRE(db.validate_session("") == false);
        }
    }

    SECTION("Clear All Sessions") {
        Database db(test_db_name);
        std::string token1 = db.create_session("user1");
        std::string token2 = db.create_session("user2");

        REQUIRE(db.validate_session(token1) == true);
        REQUIRE(db.validate_session(token2) == true);

        db.clear_all_sessions();

        REQUIRE(db.validate_session(token1) == false);
        REQUIRE(db.validate_session(token2) == false);
    }

    SECTION("Database error simulation") {
        // This tests the destructor's handling of a null db
        Database db(test_db_name);
        db.force_close_for_tests(); // Manually close db connection
        // When db goes out of scope, destructor runs on a null pointer, which is safe.
    }
}


// =========================================================================
// PART 4: main.cpp Helper Function Tests
// =========================================================================

TEST_CASE("Main.cpp Helpers - load_static_file") {

    SECTION("Load existing file") {
        // We know login.html exists
        crow::response res = load_static_file("login.html");
        REQUIRE(res.code == 200);
        // Check for some content we know is in login.html
        REQUIRE(res.body.find("<h1>Math Game</h1>") != std::string::npos);
        REQUIRE(res.get_header_value("Content-Type") == "text/html");
    }

    SECTION("Load existing css") {
        // We know main.css exists
        crow::response res = load_static_file("main.css");
        REQUIRE(res.code == 200);
        REQUIRE(res.body.find("background-color: #121b2d;") != std::string::npos);
        REQUIRE(res.get_header_value("Content-Type") == "text/css");
    }

    SECTION("Load non-existent file") {
        crow::response res = load_static_file("nonexistent_file.html");
        REQUIRE(res.code == 404);
    }
}

TEST_CASE("Main.cpp Helpers - auth_check") {
    const std::string auth_db_name = "auth_test.sqlite";
    std::remove(auth_db_name.c_str());

    auto db = std::make_shared<Database>(auth_db_name);
    db->create_user("auth_user", "pass");
    std::string token = db->create_session("auth_user");
    std::string bearer_token = "Bearer " + token;

    std::unordered_set<std::string> public_paths = {"/api/login", "/", "/main.css"};
    crow::response res;

    SECTION("Public path is allowed") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/", "", res) == true);
        REQUIRE(res.code == 200); // Code shouldn't be changed
    }

    SECTION("Public path is allowed even with bad token") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/api/login", "Bearer bad", res) == true);
        REQUIRE(res.code == 200);
    }

    SECTION("Protected path with valid token is allowed") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/api/protected_route", bearer_token, res) == true);
        REQUIRE(res.code == 200);
    }

    SECTION("Protected path with invalid token is denied") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/api/protected_route", "Bearer bad_token", res) == false);
        REQUIRE(res.code == 401);
    }

    SECTION("Protected path with no token is denied") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/api/protected_route", "", res) == false);
        REQUIRE(res.code == 401);
    }

    SECTION("Protected path with wrong token format is denied") {
        res = crow::response();
        REQUIRE(auth_check(db, public_paths, "/api/protected_route", "Basic some_other_auth", res) == false);
        REQUIRE(res.code == 401);
    }
}