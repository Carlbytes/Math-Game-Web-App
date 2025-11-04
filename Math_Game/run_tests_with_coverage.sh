#!/usr/bin/env bash
set -euo pipefail

# This script builds the unit tests with coverage flags, runs them,
# and generates an HTML coverage report using lcov + genhtml.

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build/tests-coverage"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

CXX=g++
CXXFLAGS="-std=c++17 -g -O0 -fprofile-arcs -ftest-coverage -pthread"
LDFLAGS="-lsqlite3"

# Source files to compile into the test binary. Adjust if you add/remove files.
SOURCES=(
  "$ROOT_DIR/tests/test_main.cpp"
  "$ROOT_DIR/tests/database_tests.cpp"
  "$ROOT_DIR/tests/database_error_paths.cpp"
  "$ROOT_DIR/tests/game_tests.cpp"
  "$ROOT_DIR/tests/main_tests.cpp"
  "$ROOT_DIR/Database.cpp"
  "$ROOT_DIR/Game.cpp"
  "$ROOT_DIR/main.cpp"
)

# Include paths: allow includes like "../catch2/catch.hpp" to resolve; we also
# add the repo root so headers like Database.h can be found if needed.
INCLUDES=( -I"$ROOT_DIR/catch2" -I"$ROOT_DIR" )

echo "Compiling tests (separate objects)..."
mkdir -p obj
objs=()
for src in "${SOURCES[@]}"; do
  src_base=$(basename "$src")
  obj="obj/${src_base%.cpp}.o"
  echo "  compiling $src -> $obj"
  # If compiling main.cpp for unit tests, define UNIT_TESTING so the
  # real server main() is replaced with a stub and we don't get duplicate mains
  if [[ "$src_base" == "main.cpp" ]]; then
    $CXX $CXXFLAGS -DUNIT_TESTING "${INCLUDES[@]}" -c "$src" -o "$obj"
  else
    $CXX $CXXFLAGS "${INCLUDES[@]}" -c "$src" -o "$obj"
  fi
  objs+=("$obj")
done

echo "Linking unit_tests binary..."
$CXX $CXXFLAGS "${objs[@]}" -o unit_tests $LDFLAGS

echo "Cleaning previous coverage counters..."
lcov --directory . --zerocounters || true
rm -f *.gcda *.gcno || true

echo "Running tests..."
./unit_tests

echo "Capturing coverage with lcov..."
lcov --capture --directory . --output-file coverage.info

echo "Filtering out system and third-party files..."
lcov --remove coverage.info '/usr/*' '*/catch2/*' '*/crow_all.h' --output-file coverage.info.cleaned

echo "Generating HTML report..."
genhtml coverage.info.cleaned --output-directory coverage_html

REPORT_PATH="$BUILD_DIR/coverage_html/index.html"
echo "Coverage report generated: $REPORT_PATH"

echo "To view it locally, run:
  python3 -m http.server --directory $BUILD_DIR/coverage_html 8000
and open http://localhost:8000/index.html"
