#!/bin/bash
echo "🚀 Building the project..."

# --- Prerequisites ---
# Make sure required libraries are installed
sudo apt-get update
sudo apt-get install -y libsqlite3-dev libasio-dev libssl-dev

# --- Compilation ---
g++ -std=c++17 -g -I. -c main.cpp -o main.o
g++ -std=c++17 -g -I. -c Database.cpp -o Database.o
g++ -std=c++17 -g -I. -c game.cpp -o game.o

# --- Linking ---
# Link all object files (.o) into a single executable
# -o math_server: The name of the final program
# -lsqlite3: Link the SQLite3 library
# -lssl: Link the OpenSSL library
# -lcrypto: Link the OpenSSL crypto library (for hashing)
g++ -g main.o Database.o game.o -o math_server -lsqlite3 -lssl -lcrypto

# --- Cleanup ---
rm main.o Database.o game.o

# --- Run ---
if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "🚪 Starting server on port 18080..."
    ./math_server
else
    echo "❌ Build failed. Please check for errors."
fi

