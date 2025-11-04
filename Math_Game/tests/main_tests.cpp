#include "../catch2/catch.hpp"
#include "../Database.h"
#include <fstream>
#include <unordered_set>
#include <string>

// Declare functions/types from main.cpp so we can call them from tests.
// These are declarations only; definitions live in main.o which is linked into the test binary.
namespace crow { struct request; struct response; }
extern crow::response load_static_file(const std::string& path);

// Declare the helper added to main.cpp so tests can call it directly.
extern bool auth_check(const std::shared_ptr<Database>& db_ptr,
                       const std::unordered_set<std::string>& public_paths,
                       const std::string& url,
                       const std::string& auth_header,
                       crow::response& res);

#include "../crow_all.h"

using namespace std;

TEST_CASE("load_static_file serves HTML, CSS and 404", "[main][static]") {
    // Ensure static dir exists
    const string static_dir = "static";
    system((string("mkdir -p ") + static_dir).c_str());

    // create an html file
    const string html_path = static_dir + "/test_tmp.html";
    {
        ofstream f(html_path);
        f << "<html><body>Hello</body></html>";
    }

    auto res_html = load_static_file("test_tmp.html");
    REQUIRE(res_html.code == 200);
    REQUIRE(res_html.body.find("Hello") != string::npos);
    REQUIRE(res_html.get_header_value("Content-Type") == "text/html");

    // create a css file
    const string css_path = static_dir + "/test_tmp.css";
    {
        ofstream f(css_path);
        f << "body { color: red; }";
    }
    auto res_css = load_static_file("test_tmp.css");
    REQUIRE(res_css.code == 200);
    REQUIRE(res_css.body.find("color: red") != string::npos);
    REQUIRE(res_css.get_header_value("Content-Type") == "text/css");

    // non-existent file -> 404
    auto res_404 = load_static_file("no_such_file.html");
    REQUIRE(res_404.code == 404);
    REQUIRE(res_404.body == "Not Found");

    // cleanup
    remove(html_path.c_str());
    remove(css_path.c_str());
}

TEST_CASE("AuthMiddleware before_handle public and protected paths", "[main][auth]") {
    auto db = make_shared<Database>(string(":memory:"));
    // Prepare request/response
    crow::request req;
    crow::response res;

    // Public path should not set response end
    req.url = "/";
    std::unordered_set<std::string> public_paths{ "/", "/main.css", "/api/login", "/api/register", "/game" };
    bool ok = auth_check(db, public_paths, req.url, req.get_header_value("Authorization"), res);
    // If url is public, auth_check should return true and not end response
    REQUIRE(ok == true);
    REQUIRE(res.is_completed() == false);

    // Protected path with no Authorization -> 401
    req.url = "/api/protected";
    res.clear();
    ok = auth_check(db, std::unordered_set<std::string>{"/" , "/main.css", "/api/login", "/api/register", "/game"}, req.url, req.get_header_value("Authorization"), res);
    REQUIRE(ok == false);
    REQUIRE(res.is_completed() == true);
    REQUIRE(res.code == 401);
    REQUIRE(res.body.find("Unauthorized") != string::npos);

    // Create a user and session and test valid token
    REQUIRE(db->create_user("tester", "pw") == true);
    string token = db->create_session("tester");
    REQUIRE(token.size() > 0);

    // Provide Authorization header (valid)
    req.url = "/api/protected";
    res.clear();
    ok = auth_check(db, std::unordered_set<std::string>{"/" , "/main.css", "/api/login", "/api/register", "/game"}, req.url, string("Bearer ") + token, res);
    REQUIRE(ok == true);
    REQUIRE(res.is_completed() == false);

    // Invalid token
    req.url = "/api/protected";
    res.clear();
    ok = auth_check(db, std::unordered_set<std::string>{"/" , "/main.css", "/api/login", "/api/register", "/game"}, req.url, string("Bearer not-a-token"), res);
    REQUIRE(ok == false);
    REQUIRE(res.is_completed() == true);
    REQUIRE(res.code == 401);
}
