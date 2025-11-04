#include "../catch2/catch.hpp"
#include "../Database.h"
#include <string>

TEST_CASE("Database create_user and check_login", "[database]") {
    // Use in-memory DB for isolation
    Database db(":memory:");

    REQUIRE(db.create_user("alice", "password123") == true);
    // duplicate username should fail (unique constraint)
    REQUIRE(db.create_user("alice", "password123") == false);

    // correct login
    REQUIRE(db.check_login("alice", "password123") == true);
    // wrong password
    REQUIRE(db.check_login("alice", "wrongpass") == false);
    // non-existent user
    REQUIRE(db.check_login("bob", "whatever") == false);
}

TEST_CASE("Database session lifecycle", "[database][session]") {
    Database db(":memory:");

    REQUIRE(db.create_user("tester", "p") == true);
    REQUIRE(db.check_login("tester", "p") == true);

    std::string token = db.create_session("tester");
    REQUIRE(token.size() > 0);
    REQUIRE(db.validate_session(token) == true);

    REQUIRE(db.validate_session("made-up-token") == false);

    // clear sessions and check token invalidated
    db.clear_all_sessions();
    REQUIRE(db.validate_session(token) == false);
}
