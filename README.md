# Math Game Web App

[![Live Demo](https://img.shields.io/badge/Live_Demo-Play_Now-success?style=for-the-badge)](https://sam-mathgame.duckdns.org) [![Build Status](https://img.shields.io/badge/Build-Passing-success?style=flat&logo=jenkins)](https://github.com/Carlbytes/SoftwareDevelopment3GP)

This is a web-based math game built with a C++ backend and an HTML/CSS/JS frontend. It uses the Crow C++ micro-framework to serve web pages and handle API requests, and SQLite3 for user authentication and session management.

The project allows for standard development builds via CMake/vcpkg, but also features a fully automated **CI/CD pipeline** that containerizes the application with Docker and deploys it to a live production server.

## 🚀 Live Deployment
The project is currently hosted on an Oracle Cloud VPS using a Dockerized environment behind a secure Caddy reverse proxy.

**[Play the Game Here](https://sam-mathgame.duckdns.org)**

## Features
- **User Authentication:** Users can register and log in to their accounts.
- **Persistent Sessions:** A simple token-based session system stored in SQLite.
- **Three Difficulties:**
  - **Easy:** "Which number is closest?"
  - **Medium:** Arithmetic with multiple-choice orbs (+, -, *, /).
  - **Hard:** Arithmetic with manual number input (+, -, *, /).
- **Unit Tests:** Backend logic is thoroughly tested using the Catch2 framework.
- **CI/CD Pipeline:** Fully automated Build, Test, and Deploy workflow using Jenkins.
- **Dockerized:** Runs in a lightweight container for consistency across environments.
- **HTTPS/SSL:** Automatically secured via Let's Encrypt and Caddy.
 
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
## ⚙️ CI/CD & Architecture

This project utilizes a modern DevOps pipeline to ensure code quality and automated deployment.
1. Source Control: Code pushed to GitHub triggers the pipeline.
2. Jenkins Pipeline:
   - **Build:** Compiles the C++ code using CMake.
   - **Test:** Runs Catch2 unit tests. If tests fail, the pipeline stops.
   - **Package:** Builds a Docker image containing the optimized binary and static assets.
   - **Publish:** Pushes the Docker image to Docker Hub (sammcaulay/mathgame).
3. Production:
   - The pipeline SSHs into an **Oracle Cloud VPS.**
   - Pulls the new image from Docker Hub.
   - Restarts the containers using a Caddy sidecar container to handle HTTPS and Reverse Proxying.
  
## 🐳 Running via Docker (Recommended)

You can run the game locally without installing C++ compilers or vcpkg if you have Docker installed.

```
# 1. Build the image
docker build -t mathgame .

# 2. Run the container (Maps internal port 18080 to local 8080)
docker run -p 8080:18080 mathgame
```

Open your browser to `http://localhost:8080`.

## 🛠️ Manual Development Setup

If you wish to modify the C++ code or run the project without Docker, follow these steps.

### Prerequisites
- Git
- CMake (v3.15 or higher)
- C++ Compiler (Visual Studio 2019+ or GCC/Clang)

1. Install vcpkg

It is recommended to install vcpkg in a central location.

### Windows (PowerShell):
```
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git) C:\dev\vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe integrate install
```
### Linux / WSL (Bash):
```
git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git) ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
~/vcpkg/vcpkg integrate install
```

### 2. Clone Repository
```
git clone [https://github.com/Carlbytes/SoftwareDevelopment3GP.git](https://github.com/Carlbytes/SoftwareDevelopment3GP.git)
cd SoftwareDevelopment3GP
```

3. Build & Run

### Linux / WSL (Terminal):
```
# Set path to your vcpkg toolchain
# Adjust path if your vcpkg is elsewhere
VCPKG_TOOLCHAIN=~/vcpkg/scripts/buildsystems/vcpkg.cmake

# Configure (Source is in MathGameMain folder)
cmake -B build -S MathGameMain -DCMAKE_TOOLCHAIN_FILE=$VCPKG_TOOLCHAIN

# Build
cmake --build build

# Run
./build/MathGame
```
Access at `http://localhost:18080`.

### Windows (Visual Studio):
1. Open Visual Studio and select "Open a local folder".
2. Select the `MathGameMain` folder.
3. Visual Studio should auto-detect `CMakeLists.txt` and configure dependencies via vcpkg.
4. Right-click `CMakeLists.txt` -> Build.
5. Select `MathGame.exe` from the startup item dropdown and click Run.

## Running Unit Tests

This project uses Catch2.
### From Terminal:
```
./build/RunTests
```







































