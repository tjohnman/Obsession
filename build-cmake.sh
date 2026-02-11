#!/bin/bash
# Obsession CMake Build Script for Linux
# Requires: Qt 6.10+, CMake 3.21+, C++23-capable compiler, ICU library

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Obsession CMake Build Script${NC}"
echo "================================"
echo ""

# Check for required tools
command -v cmake >/dev/null 2>&1 || { echo -e "${RED}Error: cmake is not installed${NC}"; exit 1; }
command -v qmake6 >/dev/null 2>&1 || command -v qmake >/dev/null 2>&1 || { echo -e "${RED}Error: qmake6/qmake not found (Qt 6 required)${NC}"; exit 1; }
command -v g++ >/dev/null 2>&1 || command -v clang++ >/dev/null 2>&1 || { echo -e "${RED}Error: C++ compiler not found${NC}"; exit 1; }

# Check for ICU library
if ! pkg-config --exists icu-uc icu-i18n 2>/dev/null; then
    echo -e "${YELLOW}Warning: ICU library may not be installed${NC}"
    echo -e "${YELLOW}On Debian/Ubuntu: sudo apt install libicu-dev${NC}"
    echo -e "${YELLOW}On Fedora: sudo dnf install libicu-devel${NC}"
    echo -e "${YELLOW}On Arch: sudo pacman -S icu${NC}"
    echo ""
fi

# Configuration
BUILD_TYPE="${1:-Release}"
BUILD_DIR="build"
JOBS=$(nproc 2>/dev/null || echo 4)

echo "Build configuration:"
echo "  Build type: $BUILD_TYPE"
echo "  Build directory: $BUILD_DIR"
echo "  Parallel jobs: $JOBS"
echo ""

# Create build directory
echo -e "${GREEN}Creating build directory...${NC}"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake
echo -e "${GREEN}Running CMake configuration...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_CXX_STANDARD=23 \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}Building Obsession...${NC}"
cmake --build . -j"$JOBS"

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "Executable: ${GREEN}$BUILD_DIR/Obsession${NC}"
echo ""
echo "To run Obsession:"
echo -e "  ${YELLOW}cd $BUILD_DIR && ./Obsession${NC}"
echo ""
echo "To install system-wide:"
echo -e "  ${YELLOW}sudo cmake --install $BUILD_DIR${NC}"
echo ""
