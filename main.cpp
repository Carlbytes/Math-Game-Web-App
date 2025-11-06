#include <crow.h>
#include <fstream>
#include <sstream>

int main() {
    crow::SimpleApp app;

    // Serve main menu HTML
    CROW_ROUTE(app, "/")([](const crow::request&, crow::response& res){
        std::ifstream in("www/index.html");
        if (!in) {
            res.code = 404;
            res.end("Main menu not found");
            return;
        }
        std::ostringstream contents;
        contents << in.rdbuf();
        res.set_header("Content-Type", "text/html");
        res.write(contents.str());
        res.end();
    });

    // Serve Difficulty Settings page
    CROW_ROUTE(app, "/difficulty")([](const crow::request&, crow::response& res){
        std::ifstream in("www/DifficultySettings.html");
        if (!in) {
            res.code = 404;
            res.end("Difficulty page not found");
            return;
        }
        std::ostringstream contents;
        contents << in.rdbuf();
        res.set_header("Content-Type", "text/html");
        res.write(contents.str());
        res.end();
    });

    // API: Start game
    CROW_ROUTE(app, "/api/start")([](){
        crow::json::wvalue x;
        x["status"] = "Game started";
        return x;
    });

    // API: Set difficulty
    CROW_ROUTE(app, "/api/setDifficulty/<string>")([](const std::string& level){
        crow::json::wvalue x;
        x["difficulty"] = level;
        return x;
    });

    // API: High scores
    CROW_ROUTE(app, "/api/highscores")([](){
        crow::json::wvalue x;
        x["scores"] = std::vector<int> {100, 90, 80};
        return x;
    });

    // API: Quit
    CROW_ROUTE(app, "/api/quit")([](){
        crow::json::wvalue x;
        x["status"] = "Game exited";
        return x;
    });

    // Optional: Favicon suppression
    CROW_ROUTE(app, "/favicon.ico")([](const crow::request&, crow::response& res){
        res.code = 204;
        res.end();
    });

    app.port(8081).multithreaded().run();
}
