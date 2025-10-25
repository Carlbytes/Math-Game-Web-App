#!/bin/bash

# This script compiles and runs the C++ backend server.
# It's designed for Linux and macOS.

# --- Compilation ---
# -std=c++17: Use the C++17 standard.
# -o math_server: The name of the output executable file.
# -pthread: Link against the POSIX threads library.
# We no longer need Boost flags because new Crow versions use Standalone Asio.
echo "Compiling main.cpp..."
g++ -std=c++17 main.cpp -o math_server -pthread

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Please check for errors."
    echo ""
    echo "If you see 'asio.hpp: No such file or directory', run:"
    echo "sudo apt-get update && sudo apt-get install libasio-dev"
    exit 1
fi

echo "Compilation successful. Executable 'math_server' created."
echo ""

# --- Execution ---
echo "Starting the C++ server on http://localhost:18080"
echo "Press Ctrl+C to stop the server."
./math_server
