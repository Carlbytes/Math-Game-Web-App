# Math Game Web App

This is a simple, web-based math game built with a C++ backend and an HTML/CSS/JS frontend. It uses the Crow C++ micro-framework to serve web pages and handle API requests, and SQLite3 for user authentication and session management.
The project is built using CMake and manages its dependencies with vcpkg.

## Features
- User Authentication: Users can register and log in to their accounts.
- Persistent Sessions: A simple token-based session system is stored in the database.
- Three Difficulties:
   - Easy: "Which number is closest?"
   - Medium: Arithmetic with multiple-choice orbs (+, -, *, /).
   - Hard: Arithmetic with manual number input (+, -, *, /).
   - Unit Tests: Backend logic is tested using the Catch2 framework.
 
## Project Structure
```
.
├── static/             # All frontend HTML, CSS, and JS files
│   ├── login.html      # Login and registration page
│   ├── Menu.html       # Main menu after login
│   ├── game.html       # Easy difficulty game
│   ├── gameMedium.html # Medium difficulty game
│   ├── gameHard.html   # Hard difficulty game
│   └── main.css        # Shared stylesheet
├── Game.cpp / game.h       # Backend logic for Easy mode
├── gameMedium.cpp / .h # Backend logic for Medium mode
├── gameHard.cpp / .h   # Backend logic for Hard mode
├── Database.cpp / .h   # SQLite3 database wrapper class
├── main.cpp            # Main C++ file: sets up Crow server and API routes
├── tests.cpp           # All Catch2 unit tests
├── CMakeLists.txt      # Main CMake build script
└── vcpkg.json          # vcpkg manifest for dependencies (crow, sqlite3, catch2)
```

## Prerequisites
Before you begin, ensure you have the following tools installed:
- Git: https://git-scm.com/downloads
- CMake (v3.15 or higher): https://cmake.org/download/
- A C++ Compiler:
   - Windows: Visual Studio 2019 or later (with the "Desktop development with C++" workload).
   - Linux/WSL: `build-essential` (which includes `g++`) or `clang`.
     ```
      sudo apt update
      sudo apt install build-essential g++

     ```

## Installation & Setup
These instructions use `vcpkg` for dependency management. We will install `vcpkg` first.

### 1. Install vcpkg
It is recommended to install `vcpkg` in a central location outside of this project folder.

### Windows (PowerShell)
```
# We recommend a central location, e.g., C:\dev\vcpkg
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git) C:\dev\vcpkg
.\vcpkg\bootstrap-vcpkg.bat
# This next step makes vcpkg available system-wide for Visual Studio & CLion
.\vcpkg\vcpkg.exe integrate install
```

### Linux / WSL (Bash)
```
# We recommend a central location, e.g., $HOME/vcpkg
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git) ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
# This next step makes vcpkg available system-wide for your user
~/vcpkg/vcpkg integrate install
```

### 2. Clone This Repository
Now, clone this project repository to your local machine.
```
git clone [https://github.com/your-username/math-game.git](https://github.com/your-username/math-game.git)
cd math-game
```

## Building and Running the Project
You can build the project using an IDE (recommended) or manually from the terminal.

### Windows / Linux / WSL: CLion
1. Open CLion
2. Go to `File > Open`  and select the cloned `MathGameMain` folder.
3. CLion will automatically detect the `CMakeLists.txt`.
4. Go to `File > Settings > Build, Execution, Deployment > CMake`.
5. Find your profile (e.g., "Debug") and add the following to the CMake options box. This tells CLion where to find `vcpkg`.
   - (Remember to use forward slashes / for paths)
     ```
      -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake

      ### FOR CODE COVERAGE:
      -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DENABLE_COVERAGE=ON
     ```
6. Click `OK`. CLion should reload CMake and successfully find all dependencies.
7. Select the `MathGame` target from the configuration drop-down (top right).
8. Click the `Run` (▶) button.
9. Open your browser to `http://localhost:18080.`

## Windows: Visual Studio (2019 or later)
Visual Studio's "Open Folder" feature works perfectly with CMake and `vcpkg`.

1. Open Visual Studio.
2. On the welcome screen, select "Open a local folder".
3. Navigate to and select the cloned `MathGameMain` folder.
4. Visual Studio will read the `CMakeLists.txt` and configure the project. Because you ran `vcpkg integrate install`, it should automatically find the dependencies.
5. In the Solution Explorer, right-click on `CMakeLists.txt` and select "Build".
6. Once built, select `MathGame.exe` from the "Select Startup Item" drop-down in the toolbar.
7. Click the green `Run` (▶) button.
8. Open your browser to `http://localhost:18080`.

## Linux / WSL: 
This is the classic CMake workflow. Run these commands from the root of the `math-game` project directory.
```
# Set the path to your vcpkg toolchain file
VCPKG_TOOLCHAIN=~/vcpkg/scripts/buildsystems/vcpkg.cmake

# 1. Configure CMake
#    (We'll create a build directory named 'build')
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_TOOLCHAIN

# 2. Build the project
cmake --build build

# 3. Run the server
./build/MathGame

```
Your server is now running. Open a web browser to `http://localhost:18080` to play the game.

## Running the Unit Tests
This project uses Catch2 for unit testing. The tests are defined in the `RunTests` target.

### From CLion / Visual Studio:
1. Change the active build target from `MathGame` to `RunTests`.
2. Click the `Run` (▶) button. The tests will run in the console.

### From the Terminal (Linux / WSL):
1. Make sure you have already built the project (see manual build steps above).
2. Run the test executable:
   ```
   ./build/RunTests
   ```






























































