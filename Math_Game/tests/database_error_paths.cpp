#include "../catch2/catch.hpp"
#include "../Database.h"

TEST_CASE("Database methods report errors when DB closed", "[database][error_paths]") {
    Database db(":memory:");

    // Ensure DB works normally first
    REQUIRE(db.create_user("ephemeral", "pw") == true);

    // Now force-close the underlying sqlite handle to simulate errors
    db.force_close_for_tests();

    // All following operations should exercise error handling paths and return failure/empty values
    // create_user should return false due to prepare/execute failures
    REQUIRE(db.create_user("should_fail", "pw") == false);

    // check_login should return false
    REQUIRE(db.check_login("ephemeral", "pw") == false);

    // create_session should return empty string on failure
    REQUIRE(db.create_session("any") == "");

    // validate_session should return false
    REQUIRE(db.validate_session("nope") == false);

    // clear_all_sessions should not crash (just exercise error path)
    db.clear_all_sessions();
}
