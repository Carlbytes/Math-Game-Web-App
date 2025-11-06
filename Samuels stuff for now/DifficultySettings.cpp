//
// Created by sambd on 04/11/2025.
//
#include <crow.h>
#include <fstream>
#include <sstream>

int main() {
    crow::SimpleApp app;

    // Serve the difficulty.html page
    CROW_ROUTE(app, "/")([](const crow::request&, crow::response& res){
    std::filesystem::path fullPath = std::filesystem::path("static") / "DifficultySettings.html";
        std::ifstream in(fullPath, std::ios::in | std::ios::binary);
        if (!in) {
            res.code = 404;
            res.end("Not Found");
            return;
        }
        std::ostringstream contents;
        contents << in.rdbuf();
        res.set_header("Content-Type", "text/html");
        res.write(contents.str());
        res.end();
    });

    // API endpoint to set difficulty
    CROW_ROUTE(app, "/api/setDifficulty/<string>")
    ([](const std::string& level){
        crow::json::wvalue x;
        x["difficulty"] = level;
        std::cout << "Difficulty set to: " << level << std::endl;
        return x;
    });

    CROW_ROUTE(app, "/favicon.ico")([](const crow::request&, crow::response& res){
    res.code = 204; // No Content
    res.end();
});

    app.port(8082).multithreaded().run();
}


