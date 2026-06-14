#!/usr/bin/env sh
set -eu

PROJECT_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
SRC_DIR="$PROJECT_ROOT/src"
BUILD_DIR="$PROJECT_ROOT/build"
OUTPUT_PATH="$BUILD_DIR/app"
LIBS_DIR="$PROJECT_ROOT/libs"
RES_DIR="$PROJECT_ROOT/res"

mkdir -p "$BUILD_DIR"

SRC_FILES=$(find "$SRC_DIR" -type f -name '*.cpp' | sort)
if [ -z "$SRC_FILES" ]; then
    echo "No .cpp files found in $SRC_DIR" >&2
    exit 1
fi

IFS='\
'
set -- $SRC_FILES
unset IFS

if command -v c++ >/dev/null 2>&1; then
    COMPILER="c++"
elif command -v g++ >/dev/null 2>&1; then
    COMPILER="g++"
elif command -v clang++ >/dev/null 2>&1; then
    COMPILER="clang++"
else
    echo "No supported C++ compiler found. Install GCC or Clang." >&2
    exit 1
fi

"$COMPILER" -std=c++20 -Wall -Wextra -pedantic \
    -I"$LIBS_DIR" \
    -I"$LIBS_DIR/clay" \
    -I"$LIBS_DIR/raylib" \
    "$@" -o "$OUTPUT_PATH"
echo "Built $OUTPUT_PATH"

if [ -d "$RES_DIR" ]; then
    cp -R "$RES_DIR"/. "$BUILD_DIR"/
    echo "Copied resources from $RES_DIR to $BUILD_DIR"
fi
