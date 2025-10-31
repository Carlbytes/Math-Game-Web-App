#include <crow.h>
#include <fstream>
#include <sstream>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
 ([](const crow::request&, crow::response& res){
     std::filesystem::path fullPath = std::filesystem::path("www") / "index.html";
     std::cout << "Trying to open: " << std::filesystem::absolute(fullPath) << std::endl;

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

    // Handle all other paths
    CROW_ROUTE(app, "/<path>")
    ([](const crow::request&, crow::response& res, std::string path){
        if (path.empty()) path = "index.html"; // fallback
        std::filesystem::path fullPath = std::filesystem::path("www") / path;
        std::cout << "Trying to open: " << std::filesystem::absolute(fullPath) << std::endl;

        std::ifstream in(fullPath, std::ios::in | std::ios::binary);
        if (!in) {
            res.code = 404;
            res.end("Not Found");
            return;
        }

        std::ostringstream contents;
        contents << in.rdbuf();
        res.write(contents.str());
        res.end();
    });

    CROW_ROUTE(app, "/api/start")([](){
    crow::json::wvalue x;
    x["status"] = "Game started";
    return x;
});

    CROW_ROUTE(app, "/api/settings")([](){
        crow::json::wvalue x;
        x["status"] = "Opened Difficulty settings";
        return x;
    });

    CROW_ROUTE(app, "/api/quit")([](){
        crow::json::wvalue x;
        x["status"] = "Quit requested";
        return x;
    });

    app.port(8081).multithreaded().run();
}