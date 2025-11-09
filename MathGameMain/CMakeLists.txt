cmake_minimum_required(VERSION 3.15)
project(MathGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find Dependencies (from vcpkg) ---
# This finds crow, asio, etc.
find_package(Crow CONFIG REQUIRED)
# This finds sqlite3
find_package(unofficial-sqlite3 CONFIG REQUIRED)
# This finds Catch2
find_package(Catch2 CONFIG REQUIRED)
# This finds pthreads, etc.
find_package(Threads REQUIRED)

# --- Main Executable (MathGame) ---
set(SOURCES
        main.cpp
        Database.cpp
        Game.cpp
        gameMedium.cpp
        gameHard.cpp
)
add_executable(MathGame ${SOURCES})

target_link_libraries(MathGame PRIVATE
        Crow::Crow
        unofficial::sqlite3::sqlite3
        Threads::Threads
)

# --- Windows-Specific Libraries (for networking) ---
IF (WIN32)
    target_link_libraries(MathGame PRIVATE ws2_32 mswsock)
ENDIF()

# --- Copy 'static' Directory ---
add_custom_command(TARGET MathGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:MathGame>/static
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:MathGame>/static
)

# --- Unit Testing Setup ---
enable_testing()
add_executable(RunTests
        tests.cpp
        Game.cpp
        gameMedium.cpp
        gameHard.cpp
        Database.cpp
        main.cpp
)

target_compile_definitions(RunTests PRIVATE UNIT_TESTING)

add_custom_command(TARGET RunTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:RunTests>/static
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:RunTests>/static
)


target_link_libraries(RunTests PRIVATE
        Crow::Crow
        unofficial::sqlite3::sqlite3
        Threads::Threads
        # We link Catch2 *without* Main, so we can provide our own
        Catch2::Catch2
)

# --- Windows-Specific Libraries (for networking) ---
IF (WIN32)
    target_link_libraries(RunTests PRIVATE ws2_32 mswsock)
ENDIF()

add_test(NAME UnitTests COMMAND RunTests)

# --- Code Coverage Setup ---
option(ENABLE_COVERAGE "Enable code coverage" OFF)
IF(ENABLE_COVERAGE)
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Coverage enabled")
        # Add coverage flags to compiler
        target_compile_options(RunTests PRIVATE --coverage)
        # Add coverage flags to linker (to flush data on exit)
        target_link_libraries(RunTests PRIVATE --coverage)
    ENDIF()
ENDIF()