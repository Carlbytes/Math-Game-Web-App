#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>
#include <optional> // Used for session token generation
#include <mutex>

class Database {
public:
    // Constructor: Opens the database connection and creates tables if they don't exist
    Database(const std::string& db_name);
    
    // Destructor: Closes the database connection
    ~Database();

    // User Management
    bool create_user(const std::string& username, const std::string& password);
    bool check_login(const std::string& username, const std::string& password);

    // Session Management
    std::string create_session(const std::string& username);
    bool validate_session(const std::string& token);
    void clear_all_sessions();

private:
    sqlite3* db; // The database connection object
    std::mutex db_mutex; // Mutex to make database operations thread-safe

    // Helper to run the initial table creation SQL
    void create_tables();
    // Helper to hash passwords (simple, NOT cryptographically secure for production)
    std::string hash_password(const std::string& password); 
    // Helper to generate a random token
    std::string generate_token();
};

#endif // DATABASE_H

