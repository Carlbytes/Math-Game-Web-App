cmake_minimum_required(VERSION 3.15)
project(MathGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find Dependencies ---
find_package(asio CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# --- Create Your Executable ---
# List all the .cpp files your program needs
set(SOURCES
        main.cpp
        Database.cpp
        Game.cpp
        gameMedium.cpp
        gameHard.cpp
)

# Add the source files to the executable target
add_executable(MathGame ${SOURCES})

# --- Add Include Directories ---
# This is still needed for the header-only 'crow'
target_include_directories(MathGame PRIVATE
        ${VCPKG_INCLUDE_DIRS}
)

# --- Link Dependencies ---
target_link_libraries(MathGame PRIVATE
        asio::asio
        Threads::Threads
        unofficial::sqlite3::sqlite3
)

# --- Copy 'static' Directory ---
# Updated to copy the 'static' folder instead of 'www'
add_custom_command(TARGET MathGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:MathGame>/static
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:MathGame>/static
)