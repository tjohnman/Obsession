# Building Obsession

Comprehensive build instructions for the modernized Obsession Hotline client.

## Prerequisites

### Required Software

- **Qt 6.10 or later** - Qt application framework
- **CMake 3.21 or later** - Build system generator  
- **C++23-capable compiler**:
  - GCC 11+ (Linux)
  - Clang 14+ (Linux/macOS)
  - MSVC 2022+ (Windows)
- **ICU library** - International Components for Unicode (for extended text encoding support)

### Platform-Specific Requirements

#### Linux (Debian/Ubuntu)

```bash
# Install Qt 6
sudo apt install qt6-base-dev qt6-multimedia-dev qt6-tools-dev

# Install CMake
sudo apt install cmake

# Install ICU
sudo apt install libicu-dev

# Install build tools
sudo apt install build-essential
```

#### Linux (Fedora)

```bash
# Install Qt 6
sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel qt6-qttools-devel

# Install CMake
sudo dnf install cmake

# Install ICU
sudo dnf install libicu-devel

# Install build tools
sudo dnf install gcc-c++ make
```

#### Linux (Arch)

```bash
# Install Qt 6
sudo pacman -S qt6-base qt6-multimedia qt6-tools

# Install CMake
sudo pacman -S cmake

# Install ICU
sudo pacman -S icu

# Build tools (usually pre-installed)
sudo pacman -S base-devel
```

#### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install Qt 6
brew install qt@6

# Install CMake
brew install cmake

# Install ICU
brew install icu4c

# You may need to add Qt to your PATH:
echo 'export PATH="/usr/local/opt/qt@6/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

#### Windows

1. **Install Qt 6**:
   - Download Qt Online Installer from https://www.qt.io/download
   - Select Qt 6.10+ with MinGW or MSVC compiler
   - Choose Qt Multimedia component during installation

2. **Install CMake**:
   - Download from https://cmake.org/download/
   - Add CMake to system PATH during installation

3. **Install ICU**:
   - Option A: Use vcpkg:
     ```powershell
     git clone https://github.com/Microsoft/vcpkg.git
     cd vcpkg
     .\bootstrap-vcpkg.bat
     .\vcpkg install icu
     ```
   - Option B: Download pre-built binaries from https://github.com/unicode-org/icu/releases

4. **Compiler**: Visual Studio 2022 Community Edition or Qt's bundled MinGW

---

## Building from Source

### Quick Start (Linux/macOS)

```bash
# Clone the repository
git clone https://github.com/tjohnman/Obsession.git
cd Obsession

# Run the build script
./build-cmake.sh

# The executable will be in build/Obsession
cd build && ./Obsession
```

### Manual Build (All Platforms)

```bash
# 1. Create build directory
mkdir build
cd build

# 2. Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. Build the project
cmake --build . -j$(nproc)  # Linux/macOS
cmake --build . -j%NUMBER_OF_PROCESSORS%  # Windows

# 4. Run Obsession
./Obsession  # Linux/macOS
.\Obsession.exe  # Windows
```

### Build Types

- **Release** (default): Optimized for performance
- **Debug**: Includes debugging symbols
- **RelWithDebInfo**: Optimized with debug info
- **MinSizeRel**: Optimized for size

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

---

## Installation

### Linux/macOS

```bash
# After building, install system-wide
cd build
sudo cmake --install .

# Obsession will be installed to /usr/local/bin by default
obsession
```

### Windows

The compiled `Obsession.exe` is standalone. You can copy it to any location along with required Qt DLLs:

```powershell
# Use Qt's windeployqt tool to package dependencies
cd build\Release
windeployqt.exe Obsession.exe
```

---

## Verifying the Build

### Check Qt Version

```bash
# Linux/macOS
qmake6 --version

# Should show Qt 6.10 or later
```

### Check C++ Standard Support

```bash
# GCC
g++ --version  # Should be 11+ for C++23

# Clang
clang++ --version  # Should be 14+ for C++23

# Check C++23 feature support
echo '#if __cplusplus >= 202302L
int main() { return 0; }
#endif' | g++ -std=c++23 -x c++ - -o /dev/null && echo "C++23 supported"
```

### Verify ICU Installation

```bash
# Linux/macOS
pkg-config --modversion icu-uc

# Should show ICU version (e.g., 70.1)
```

---

## Troubleshooting

### Qt Not Found

```
CMake Error: Could not find Qt6
```

**Solution**: Ensure Qt 6 is installed and findable:

```bash
# Set Qt6_DIR environment variable
export Qt6_DIR=/path/to/qt6/lib/cmake/Qt6

# Or specify during cmake configuration
cmake .. -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6
```

### ICU Not Found

```
CMake Error: Could not find ICU
```

**Solution**: Install ICU library or specify its location:

```bash
# Linux: Install via package manager (see above)

# macOS with Homebrew
brew install icu4c
export PKG_CONFIG_PATH="/usr/local/opt/icu4c/lib/pkgconfig"

# Windows with vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

### C++23 Compiler Errors

```
error: requested C++23 but compiler only supports C++20
```

**Solution**: Update your compiler:

```bash
# Ubuntu/Debian
sudo apt install g++-11  # or later

# Fedora
sudo dnf install gcc-c++

# macOS
brew install llvm
```

### Missing Qt Multimedia Module

```
CMake Error: Could not find Qt6Multimedia
```

**Solution**: Install Qt Multimedia component:

```bash
# Linux
sudo apt install qt6-multimedia-dev  # Debian/Ubuntu
sudo dnf install qt6-qtmultimedia-devel  # Fedora

# Or reinstall Qt with Qt Multimedia selected
```

### Runtime Errors: Missing Shared Libraries

```
error while loading shared libraries: libQt6Core.so.6
```

**Solution**: Set `LD_LIBRARY_PATH`:

```bash
export LD_LIBRARY_PATH=/path/to/qt6/lib:$LD_LIBRARY_PATH

# Or install Qt system-wide via package manager
```

---

## Build Options

### Custom Installation Prefix

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/obsession
sudo cmake --install .
```

### Disable Optimizations (for debugging)

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-O0 -g"
```

### Enable Compiler Warnings

```bash
cmake .. -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"
```

### Build with Specific Compiler

```bash
# Use Clang instead of GCC
cmake .. -DCMAKE_CXX_COMPILER=clang++

# Use specific GCC version
cmake .. -DCMAKE_CXX_COMPILER=g++-12
```

---

## Development Build

For active development, enable compile commands export for IDE integration:

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# This creates compile_commands.json for IDEs like VS Code, CLion, etc.
```

---

## Clean Build

To start fresh:

```bash
# Remove build directory
rm -rf build

# Or use CMake's clean target
cmake --build build --target clean
```

---

## Platform-Specific Notes

### Linux

- Qt 6 packages vary by distribution; check your package manager
- ICU is usually available as `libicu-dev` or `icu-devel`
- AppImage creation is possible using `linuxdeployqt`

### macOS

- Qt 6 via Homebrew is the easiest installation method
- Universal binaries (Apple Silicon + Intel) require building for each architecture
- Code signing may be required for distribution

### Windows

- MinGW-based Qt builds are simpler than MSVC for beginners
- ICU via vcpkg is recommended for consistent builds
- Use `windeployqt` to bundle Qt DLLs for distribution
- Installer creation possible with NSIS or Inno Setup

---

## Cross-Compilation

### Building for Windows on Linux (MinGW)

```bash
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64.cmake \
    -DCMAKE_BUILD_TYPE=Release
```

(Requires MinGW cross-compilation toolchain)

---

## Continuous Integration

Example GitHub Actions workflow snippet:

```yaml
- name: Install dependencies (Ubuntu)
  run: |
    sudo apt update
    sudo apt install qt6-base-dev qt6-multimedia-dev libicu-dev cmake

- name: Build
  run: |
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . -j$(nproc)
```

---

## Additional Resources

- **Qt Documentation**: https://doc.qt.io/qt-6/
- **CMake Documentation**: https://cmake.org/documentation/
- **ICU Documentation**: https://unicode-org.github.io/icu/
- **Obsession GitHub**: https://github.com/tjohnman/Obsession
- **Hotline Wiki**: https://hlwiki.com/

---

## Getting Help

If you encounter build issues:

1. Check this document's **Troubleshooting** section
2. Verify all prerequisites are installed with correct versions
3. Create a GitHub issue with:
   - Your operating system and version
   - Qt version (`qmake6 --version`)
   - CMake version (`cmake --version`)
   - Compiler version (`g++ --version` or `clang++ --version`)
   - Full error output from CMake/build

---

## License

Obsession is open-source software. See LICENSE.txt for details.
