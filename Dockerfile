# --- Stage 1: Build the Game ---
FROM ubuntu:22.04 AS builder

# Install build tools
RUN apt-get update && apt-get install -y build-essential cmake git curl zip unzip tar pkg-config

# Install vcpkg (dependency manager)
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg
RUN /opt/vcpkg/bootstrap-vcpkg.sh

# Copy your project files into the image
WORKDIR /app
COPY . .

# Install dependencies & Build
# Note: We use Release mode for the smallest/fastest game
RUN /opt/vcpkg/vcpkg install
RUN cmake -B build -S MathGameMain -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build

# --- Stage 2: Run the Game ---
FROM ubuntu:22.04

# Install only what is needed to run (sqlite3)
RUN apt-get update && apt-get install -y libsqlite3-0 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy only the executable and static files from the builder stage
COPY --from=builder /app/build/MathGame .
COPY --from=builder /app/MathGameMain/static ./static

# Open the port
EXPOSE 18080

# Start the game
CMD ["./MathGame"]
