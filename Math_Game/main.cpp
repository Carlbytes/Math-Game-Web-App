#include "crow_all.h"   // The main Crow framework header.
#include <fstream>      // For file input/output (reading index.html)
#include <sstream>      // For string streams (buffering file contents)
#include <cstdlib>      // For rand() and srand() (random numbers)
#include <ctime>        // For time() (seeding the random number generator)
/**
 * @brief Generates a simple addition question.
 * @return A pair containing the question string (e.g., "What is 5 + 3?")
 * and the integer answer (e.g., 8).
 */
std::pair<std::string, int> generate_question() {
    // Generate two random numbers between 1 and 10
    int a = rand() % 10 + 1;
    int b = rand() % 10 + 1;
    
    // Create the question string
    std::string question = "What is " + std::to_string(a) + " + " + std::to_string(b) + "?";
    
    // Return both the question and the correct answer
    return {question, a + b};
}

int main() {
    // Seed the random number generator once when the server starts
    srand(time(nullptr));

    // Create an instance of the Crow web application
    crow::SimpleApp app;

    // Server State
    // These variables hold the *current* question and answer.
    // Because we are using .multithreaded(), these are not
    // technically "thread-safe." 
    // since our game is simple, its fine and it works
    // but if we want to make anything bigger we should look at other options
    std::string current_question;
    int current_answer;

    // Web Page Route
    // This route handles requests for the main web page 
    CROW_ROUTE(app, "/")([] {
        // Open the index.html file
        // Assumes it's in a folder named "static" in the same
        // directory where the server executable is run
        // AKA, please dont change this file structure all too much
        std::ifstream file("static/index.html");
        if (!file.is_open()) {
            // If the file isn't found, send a 500 server error.
            return crow::response(500, "index.html not found.");
        }
        
        // Read the entire file into a string buffer.
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        // Send the file's contents as the response.
        // The browser will render this as a web page.
        return crow::response(buffer.str());
    });

    //API Routes 
    
    /**
     * @brief API endpoint to get a new math question.
     * @method GET
     * @url /api/question
     * @return JSON: {"question": "What is a + b?"}
     */
    CROW_ROUTE(app, "/api/question").methods("GET"_method)
    // The [&] captures the server state variables (current_question, current_answer)
    // by reference, so we can update them.
    ([&](const crow::request&) {
        // Generate a new question and answer
        auto [q, ans] = generate_question();
        
        // Store them as the "current" ones for the /api/answer route to check.
        current_question = q;
        current_answer = ans;
        
        // Create a JSON response object
        crow::json::wvalue response;
        response["question"] = q;
        
        // return JSON to front end
        return response;
    });

    /**
     * @brief API endpoint to check a user's answer
     * @method POST
     * @url /api/answer
     * @body JSON: {"answer": 5}
     * @return JSON: {"correct": true/false, "correct_answer": 8}
     */
    CROW_ROUTE(app, "/api/answer").methods("POST"_method)
    // Capture state variables by reference again
    ([&](const crow::request& req) {
        // Parse the JSON body sent from the frontend
        auto body = crow::json::load(req.body);
        
        // this checks if the JSON is valid, and has an answer field
        if (!body || !body.has("answer")) {
            return crow::response(400, "Bad Request: Missing 'answer' field.");
        }

        // pull the users answer as an integer from the JSON. this will maybe needed to be changed if we use doubles or floats
        int user_answer = body["answer"].i();
        
        // Create the JSON response
        crow::json::wvalue res;
        res["correct"] = (user_answer == current_answer); // Check if correct
        res["correct_answer"] = current_answer;          // Tell the frontend the right answer
        
        // Send the JSON response.
        return crow::response(res);
    });

    //this bit starts the server
    std::cout << "Starting server on port 18080..." << std::endl;
    app.port(18080)     // Set the port to 18080 (or whatever you want, i just had something running on 8080)
       .multithreaded() // Allow the server to handle multiple requests at once
       .run();          // Start the server and listen for connections
}
