#!/bin/bash

# Script to download and apply a patch, create a build directory, and build a Qt project

# Set the patch URL
PATCH_URL="https://aur.archlinux.org/cgit/aur.git/plain/qt5.patch?h=obsession-git"
PATCH_FILE="qt5.patch"
BUILD_DIR="build"

# Function to check for qmake
check_qmake() {
    if command -v qmake &> /dev/null; then
        QMAKE_CMD="qmake"
    elif command -v qmake-qt5 &> /dev/null; then
        QMAKE_CMD="qmake-qt5"
    else
        echo "Error: Neither qmake nor qmake-qt5 found in PATH."
        exit 1
    fi
}

# Download the patch
echo "Downloading the patch..."
wget "$PATCH_URL" -O "$PATCH_FILE"

if [ $? -ne 0 ]; then
    echo "Error: Failed to download the patch."
    exit 1
fi

# Apply the patch
echo "Applying the patch..."
patch -p1 < "$PATCH_FILE"

if [ $? -ne 0 ]; then
    echo "Error: Failed to apply the patch."
    exit 1
fi

# Create the build directory
echo "Creating the build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

if [ $? -ne 0 ]; then
    echo "Error: Failed to create the build directory."
    exit 1
fi

# Change to the build directory
echo "Changing directory to: $BUILD_DIR"
cd "$BUILD_DIR"

# Check for qmake
check_qmake

# Run qmake
echo "Running $QMAKE_CMD..."
$QMAKE_CMD -config release ../Obsession.pro

if [ $? -ne 0 ]; then
    echo "Error: $QMAKE_CMD failed."
    exit 1
fi

# Run make
echo "Running make..."
make -j

if [ $? -ne 0 ]; then
    echo "Error: make failed."
    exit 1
fi

echo "Build completed successfully!"
echo "You can find Obsession within the build directory."

rm "../$PATCH_FILE"

exit 0

