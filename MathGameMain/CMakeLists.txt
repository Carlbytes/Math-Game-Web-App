#set the minumum version of cmake and defines our project, this is important for package compatibility
cmake_minimum_required(VERSION 3.15)
project(MathGame LANGUAGES CXX)

# Specifies C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# finds dependencies from vcpkg
# This finds crow, asio, etc.
find_package(Crow CONFIG REQUIRED)
# This finds sqlite3
find_package(unofficial-sqlite3 CONFIG REQUIRED)
# This finds Catch2
find_package(Catch2 CONFIG REQUIRED)
# This finds pthreads, etc.
find_package(Threads REQUIRED)

# These are the defined sources use to create the main game executeable file
set(SOURCES
        main.cpp
        Database.cpp
        Game.cpp
        gameMedium.cpp
        gameHard.cpp
)
# This here directly builds the main runnning executable
add_executable(MathGame ${SOURCES})

target_link_libraries(MathGame PRIVATE
        Crow::Crow
        unofficial::sqlite3::sqlite3
        Threads::Threads
)

# these are libraries that are specific to the windows OS, a linux system will not have mswsock
IF (WIN32)
    target_link_libraries(MathGame PRIVATE ws2_32 mswsock)
ENDIF()

# this copies the static directory, all of our web files are stored here
add_custom_command(TARGET MathGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:MathGame>/static
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:MathGame>/static
)

# this adds the needed files, and defines the test case executeable
enable_testing()
add_executable(RunTests
        tests.cpp
        Game.cpp
        gameMedium.cpp
        gameHard.cpp
        Database.cpp
        main.cpp
)

# Define a preprocessor macro named unit testing for run tests, this will trigger the #ifndef UNIT_TESTING
target_compile_definitions(RunTests PRIVATE UNIT_TESTING)

# like the main game, the tests need access the the statcic library also.
add_custom_command(TARGET RunTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:RunTests>/static
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:RunTests>/static
)

# link the dependancies for the tests
target_link_libraries(RunTests PRIVATE
        Crow::Crow
        unofficial::sqlite3::sqlite3
        Threads::Threads
        # We link Catch2 *without* Main, so we can provide our own
        Catch2::Catch2
)

# again here we link the windows specific libraries
IF (WIN32)
    target_link_libraries(RunTests PRIVATE ws2_32 mswsock)
ENDIF()
# Registers RunTests with Ctest, executing the binary when run
add_test(NAME UnitTests COMMAND RunTests)

# this is where we set up the code coverage

# we add a configurable option to make CMAKE GUI
# using cmake - DENABLE_CONVERAGE=ON
option(ENABLE_COVERAGE "Enable code coverage" OFF)
IF(ENABLE_COVERAGE)
    # Coverage is compiler specific, so we are checking for gcc here.
    # Clang also may work here as it has similar flags, however im going to stick with gcc
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Coverage enabled")
        # Add coverage flags to compiler
        target_compile_options(RunTests PRIVATE --coverage)
        # Add coverage flags to linker (to flush data on exit)
        target_link_libraries(RunTests PRIVATE --coverage)
    ENDIF()
ENDIF()
