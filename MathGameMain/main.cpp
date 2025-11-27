#include <crow.h>
#include "Database.h" // Include our database class
#include "game.h"     
#include "gameMedium.h"
#include "gameHard.h"
#include <fstream>
#include <sstream>     // Make sure this is included
#include <memory>      // For std::make_shared
#include <unordered_set> // For public_paths
#include <string>      // For std::stoi
#include <iostream>    // For std::cout

// Helper function, this will be used to load the web pages
crow::response load_static_file(const std::string& path) {

    std::ifstream file("static/" + path);
    if (!file.is_open()) {
        return crow::response(404, "Not Found");
    }
    
    std::stringstream buffer;

    buffer << file.rdbuf();
    
    crow::response res(buffer.str());
    if (path.find(".html") != std::string::npos) {
        res.set_header("Content-Type", "text/html");
    } else if (path.find(".css") != std::string::npos) {
        res.set_header("Content-Type", "text/css");
    }
    return res;
}


bool auth_check(const std::shared_ptr<Database>& db_ptr,
                const std::unordered_set<std::string>& public_paths,
                const std::string& url,
                const std::string& auth_header,
                crow::response& res) {
    if (public_paths.count(url)) {
        return true;
    }
    auto token = auth_header;
    if (token.rfind("Bearer ", 0) != 0 || !db_ptr->validate_session(token.substr(7))) {
        res.code = 401;
        res.body = "{\"error\": \"Unauthorized\"}";
        res.end();
        return false;
    }
    return true;
}

struct AuthMiddleware {
    std::shared_ptr<Database> db_ptr;
    
    std::unordered_set<std::string> public_paths = {
        "/", 
        "/main.css", 
        "/api/login", 
        "/api/register",
        "/game" ,
        "/gameMedium", 
        "/gameHard",  
        "/menu",
        "/settings",
        "/api/easy/question",
        "/api/easy/answer",
        "/api/medium/question",
        "/api/medium/answer",
        "/api/hard/question",
        "/api/hard/answer"
    };

    AuthMiddleware(std::shared_ptr<Database> db) : db_ptr(db) {}
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& /*ctx*/) {
        // Delegate actual checking to testable helper
        auto token = req.get_header_value("Authorization");
        auth_check(db_ptr, public_paths, req.url, token, res);
    }
    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/) {}
};


// When building unit tests we define UNIT_TESTING and skip the server main.
#ifndef UNIT_TESTING
int main() {
    //initialize game logic
    GameEasy::initialize();     // Call easy
    GameMedium::initialize();   // Call medium
    GameHard::initialize();     // Call hard
    auto db = std::make_shared<Database>("math_game.db");
    crow::App<AuthMiddleware> app(AuthMiddleware{db});

    //public routes
    CROW_ROUTE(app, "/")([] { return load_static_file("login.html"); });
    CROW_ROUTE(app, "/main.css")([] { return load_static_file("main.css"); });
    CROW_ROUTE(app, "/game")([] { return load_static_file("game.html"); }); 
    // Routes for medium and hard pages
    CROW_ROUTE(app, "/gameMedium")([] { return load_static_file("gameMedium.html"); }); 
    CROW_ROUTE(app, "/gameHard")([] { return load_static_file("gameHard.html"); }); 
    // Serve the menu page so logged-in users can be redirected here
    CROW_ROUTE(app, "/menu")([] { return load_static_file("Menu.html"); });
    CROW_ROUTE(app, "/settings")([] { return load_static_file("DifficultySettings.html"); });

    // Added favicon route from DifficultySettings.cpp
    CROW_ROUTE(app, "/favicon.ico")([](const crow::request&, crow::response& res){
        res.code = 204; // No Content
        res.end();
    });

    CROW_ROUTE(app, "/api/register").methods("POST"_method)

        ([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "{\"error\": \"Missing username or password\"}");
        }
        std::string user = body["username"].s();
        std::string pass = body["password"].s();
        if (db->create_user(user, pass)) {
            return crow::response(201, "{\"success\": \"User created\"}");
        } else {
            return crow::response(409, "{\"error\": \"Username already exists\"}");
        }
    });

    CROW_ROUTE(app, "/api/login").methods("POST"_method)

        ([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "{\"error\": \"Missing username or password\"}");
        }
        std::string user = body["username"].s();
        std::string pass = body["password"].s();
        if (db->check_login(user, pass)) {
            std::string token = db->create_session(user);
            if (!token.empty()) {
                crow::json::wvalue res;
                res["token"] = token;
                return crow::response(200, res);
            }
        }
        return crow::response(401, "{\"error\": \"Invalid username or password\"}");
    });

    // Protected Routes
    
    // easy api
    CROW_ROUTE(app, "/api/easy/question").methods("GET"_method)
        ([] {
        return crow::response(200, GameEasy::get_question());
    });

    CROW_ROUTE(app, "/api/easy/answer").methods("POST"_method)
        ([](const crow::request& req) {

        auto body = crow::json::load(req.body);
        if (!body || !body.has("user_answer") || !body.has("correct_answer")) {
            return crow::response(400, "Bad Request");
        }
        int user_ans = body["user_answer"].i();
        int correct_ans = body["correct_answer"].i();
        return crow::response(200, GameEasy::check_answer(user_ans, correct_ans));
    });

    // medium api
    CROW_ROUTE(app, "/api/medium/question").methods("GET"_method)
        ([] {
        return crow::response(200, GameMedium::get_question());
    });

    CROW_ROUTE(app, "/api/medium/answer").methods("POST"_method)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("user_answer") || !body.has("correct_answer")) {
            return crow::response(400, "Bad Request");
        }
        int user_ans = body["user_answer"].i();
        int correct_ans = body["correct_answer"].i();
        return crow::response(200, GameMedium::check_answer(user_ans, correct_ans));
    });

    // hard api
    CROW_ROUTE(app, "/api/hard/question").methods("GET"_method)
        ([] {
        return crow::response(200, GameHard::get_question());
    });

    CROW_ROUTE(app, "/api/hard/answer").methods("POST"_method)
        ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("user_answer") || !body.has("correct_answer")) {
            return crow::response(400, "Bad Request");
        }
        int user_ans = body["user_answer"].i();
        int correct_ans = body["correct_answer"].i();
        return crow::response(200, GameHard::check_answer(user_ans, correct_ans));
    });


    // Start the server
    app.port(18080).multithreaded().run();
}
#endif