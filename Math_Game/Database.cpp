#include "Database.h"
#include <iostream>
#include <sstream>      // For std::stringstream
#include <iomanip>      // For std::setw, std::hex
#include <random>       // For session token generation

// Constructor
Database::Database(const std::string& db_name) : db(nullptr) {
    // Attempt to open the SQLite database file
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        // In a real app, you might throw an exception here
    } else {
        std::cout << "Database opened successfully." << std::endl;
        // Enable foreign keys
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
        // Create tables if they don't already exist
        create_tables();
        // Clear old sessions from previous server runs
        clear_all_sessions();
    }
}

// Destructor
Database::~Database() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

// Create 'users' and 'sessions' tables
void Database::create_tables() {
    // Use a lock for all database operations to ensure thread safety
    std::lock_guard<std::mutex> lock(db_mutex);

    char* err_msg = nullptr;
    const char* sql_users = 
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  password_hash TEXT NOT NULL"
        ");";

    if (sqlite3_exec(db, sql_users, 0, 0, &err_msg)) {
        std::cerr << "SQL error (users table): " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << "Users table OK." << std::endl;
    }

    const char* sql_sessions =
        "CREATE TABLE IF NOT EXISTS sessions ("
        "  token TEXT PRIMARY KEY NOT NULL,"
        "  username TEXT NOT NULL,"
        "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    if (sqlite3_exec(db, sql_sessions, 0, 0, &err_msg)) {
        std::cerr << "SQL error (sessions table): " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << "Sessions table OK." << std::endl;
    }
}

// --- HASHING (Warning: Not for production!) ---
// This is a simple placeholder. Real apps must use a slow, salted hash
// like Argon2, scrypt, or bcrypt.
std::string Database::hash_password(const std::string& password) {
    std::size_t hash = std::hash<std::string>{}(password + "somesalt"); // Simple salt
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

// Create a new user in the 'users' table
bool Database::create_user(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex);

    std::string hashed_pass = hash_password(password);
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO users (username, password_hash) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL error (prepare create_user): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_pass.c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error (execute create_user): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    return true;
}

// Check if a username and password match a record in 'users'
bool Database::check_login(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(db_mutex);

    std::string hashed_pass = hash_password(password);

    sqlite3_stmt* stmt;
    const char* sql = "SELECT 1 FROM users WHERE username = ? AND password_hash = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL error (prepare check_login): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_pass.c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_ROW); // We found a matching row
    sqlite3_finalize(stmt);
    
    return success;
}

// Generate a random 32-char hex token
std::string Database::generate_token() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        ss << std::setw(2) << dis(gen);
    }
    return ss.str();
}

// Create a new session token for a user
std::string Database::create_session(const std::string& username) {
    std::lock_guard<std::mutex> lock(db_mutex);

    std::string token = generate_token();
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO sessions (token, username) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL error (prepare create_session): " << sqlite3_errmsg(db) << std::endl;
        return ""; // Return empty string on failure
    }
    
    sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error (execute create_session): " << sqlite3_errmsg(db) << std::endl;
        return "";
    }
    
    return token;
}

// Check if a session token is valid
bool Database::validate_session(const std::string& token) {
    std::lock_guard<std::mutex> lock(db_mutex);

    sqlite3_stmt* stmt;
    // We also check if the token is recent (e.g., < 1 day old)
    // 'NOW', '-1 DAY'
    const char* sql = "SELECT 1 FROM sessions WHERE token = ? AND created_at > DATETIME('NOW', '-1 DAY');";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "SQL error (prepare validate_session): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_ROW); // Token was found and is valid
    sqlite3_finalize(stmt);
    
    return success;
}

// Clear all sessions (e.g., on server startup)
void Database::clear_all_sessions() {
    std::lock_guard<std::mutex> lock(db_mutex);
    
    char* err_msg = nullptr;
    const char* sql = "DELETE FROM sessions;";
    
    if (sqlite3_exec(db, sql, 0, 0, &err_msg)) {
        std::cerr << "SQL error (clear_all_sessions): " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << "All old sessions cleared." << std::endl;
    }
}

