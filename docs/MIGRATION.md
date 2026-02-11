# Qt 4 to Qt 6 Migration Guide

## Overview

Obsession has been modernized from Qt 4.8.7 to Qt 6.10 with the following major upgrades:

- **Build System**: QMake → CMake
- **C++ Standard**: C++11 → C++23
- **Sound API**: QSound → QSoundEffect
- **Text Encoding**: QTextCodec → QStringConverter + ICU
- **Signal/Slot**: Old-style macros → Function pointers (in progress)
- **New Feature**: Dark theme support with Light/Dark/Auto modes

---

## API Migrations Completed

### 1. QSound → QSoundEffect

**What Changed**: Qt 6 removed `QSound` in favor of `QSoundEffect` from Qt Multimedia.

**Files Modified**:
- `src/mainwindow.h` - Header includes and member variables
- `src/mainwindow.cpp` - Sound instantiation  
- `src/widgetchat.h` - Sound member variable

**Old Code**:
```cpp
#include <QSound>

QSound * chatSound;
chatSound = new QSound("./sounds/chat.wav");
chatSound->play();
```

**New Code**:
```cpp
#include <QSoundEffect>

QSoundEffect * chatSound;
chatSound = new QSoundEffect(this);
chatSound->setSource(QUrl::fromLocalFile("./sounds/chat.wav"));
chatSound->play();
```

**Key Differences**:
- `QSoundEffect` requires `Qt6::Multimedia` module
- Sound sources are set via `QUrl` instead of QString paths
- Resource paths use `qrc:` scheme instead of `:`
- Parent object should be specified for memory management

---

### 2. QTextCodec → QStringConverter + ICU

**What Changed**: Qt 6 moved `QTextCodec` to Qt5Compat and provides `QStringConverter` with limited built-in encodings. **ICU library** is required for extended encoding support (Shift-JIS, Apple Roman, etc.).

**Files Modified**:
- `src/TextHelper.h` - Core encoding helper class (complete rewrite)
- `src/transactionparameter.cpp` - Parameter decoding
- `src/mainwindow.cpp` - Header include
- `src/connectioncontroller.cpp` - Header include
- `src/dialogtrackers.cpp` - Header include
- `src/dialogpreferences.cpp` - Header include

**Old Code**:
```cpp
#include <QTextCodec>

QTextCodec *codec = QTextCodec::codecForName("Shift_JIS");
QString decoded = codec->toUnicode(data, length);
QByteArray encoded = codec->fromUnicode(str);
```

**New Code**:
```cpp
#include <QStringConverter>

// QStringDecoder/Encoder with ICU support
auto decoder = QStringDecoder("Shift_JIS");
if (!decoder.isValid()) {
    // Fallback handling
    decoder = QStringDecoder("UTF-8");
}
QString decoded = decoder.decode(QByteArrayView(data, length));

auto encoder = QStringEncoder("Shift_JIS");
QByteArray encoded = encoder.encode(str);
```

**Key Changes**:
- `QTextCodec::codecForName()` → `QStringDecoder(codecName)`
- `codec->toUnicode()` → `decoder.decode(QByteArrayView)`
- `codec->fromUnicode()` → `encoder.encode()`
- **ICU Required**: Qt 6's built-in encodings are limited (UTF-8/16/32, Latin-1). Shift-JIS and Apple Roman need ICU.
- Check validity with `decoder.isValid()` before use
- `QByteArrayView` is used for zero-copy data viewing

**Supported Encodings** (with ICU):
- Apple Roman (Mac OS Roman) - Hotline default
- Shift-JIS - Japanese servers
- ISO-8859-1 (Latin-1) - Western European  
- UTF-8 - Modern standard
- UTF-16, UTF-32 - Unicode variants

---

### 3. Build System: QMake → CMake

**What Changed**: Modern CMake-based build system replacing QMake `.pro` files.

**Old Build**:
```bash
qmake Obsession.pro
make
```

**New Build**:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

**Key Benefits**:
- Better cross-platform support
- Modern dependency management
- Out-of-source builds (clean source directory)
- Better IDE integration
- Easier continuous integration setup

**CMakeLists.txt Highlights**:
```cmake
cmake_minimum_required(VERSION 3.21)
project(Obsession VERSION 109.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core Gui Widgets Network Multimedia
)

find_package(ICU REQUIRED COMPONENTS uc i18n)

qt_standard_project_setup()
qt_add_executable(Obsession ...)
target_link_libraries(Obsession PRIVATE Qt6::... ICU::uc ICU::i18n)
```

---

### 4. C++11 → C++23

**What Changed**: Modern C++ standard for better language features and performance.

**New Features Available**:
- `std::expected<T, E>` for error handling
- `std::print` for formatted output
- `if consteval` for compile-time logic
- Improved constexpr capabilities
- `std::stacktrace` for debugging
- Deducing `this` for less boilerplate

**Compiler Requirements**:
- **GCC 11+** (full C++23 support in GCC 13+)
- **Clang 14+** (full support in Clang 16+)
- **MSVC 2022 17.2+**

---

## New Features

### Dark Theme Support

**Feature**: Modern dark color scheme with Light/Dark/Auto modes.

**Components**:
1. **Dark Theme Stylesheet**: `resources/dark-theme.qss`
   - Comprehensive QSS covering all Qt widgets
   - Dark backgrounds (#1e1e1e, #252526, #2d2d2d)
   - Light text (#e0e0e0)
   - Blue accent color (#007acc) for selection/focus

2. **ThemeManager**: `src/ThemeManager.h`
   - Static utility class for theme management
   - `AppTheme` enum: Light, Dark, Auto
   - System theme detection (Qt 6.5+ `colorScheme()`)
   - QSettings integration for theme persistence

3. **Theme Preference**:
   - Saved in QSettings ("mir", "Contra", key: "appTheme")
   - Auto mode follows system theme (Windows/macOS/Linux)
   - Light mode uses default Qt styling
   - Dark mode loads custom QSS

**Usage**:
```cpp
#include "ThemeManager.h"

// Apply dark theme
ThemeManager::applyTheme(AppTheme::Dark);

// Auto-detect system theme
ThemeManager::applySystemTheme();

// Save preference
ThemeManager::saveThemePreference(AppTheme::Auto);

// Load saved preference
AppTheme theme = ThemeManager::loadThemePreference();
```

**Preferences Dialog Integration**:
Theme selector combo box added to `dialogpreferences.ui` with options:
- Light
- Dark
- Auto (Follow System)

---

## Remaining Work

### Signal/Slot Migration (179 occurrences)

**Status**: Not yet migrated. Still using old-style SIGNAL/SLOT macros.

**Old Style** (string-based, runtime-checked):
```cpp
connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
connect(socket, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
```

**New Style** (function pointers, compile-time checked):
```cpp
connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onDataReceived);
```

**Benefits of Migration**:
- Compile-time type checking
- Better IDE auto-completion
- Slightly better performance  
- Catches typos at compile time instead of runtime

**Files to Migrate**:
- `mainwindow.cpp` (~70 connections)
- `dialogpreferences.cpp`
- `dialogtrackers.cpp`
- `widgetnews.cpp`
- `downloadmanager.cpp`
- `uploadmanager.cpp`
- And ~10 more dialog files

**Migration Strategy**:
1. Work file-by-file to isolate errors
2. Compile and test after each file
3. Use compiler errors to catch signal/slot mismatches
4. Lambda functions can be used for complex connections

---

## Breaking Changes & Compatibility

### Removed Qt 4 APIs

- **QSound** - Use `QSoundEffect`
- **QTextCodec** - Use `QStringConverter` (or Qt5Compat temporarily)
- **QString::fromAscii() / toAscii()** - Use `fromLatin1()` / `toLatin1()`
- **QDesktopWidget** - Use `QScreen`
- **QHttp** - Use `QNetworkAccessManager` (already migrated)

### Qt 6 Behavioral Changes

1. **QString is UTF-16 only**: No implicit conversions from char* (good practice)
2. **QList = QVector**: No more distinction
3. **Event pointer types**: Some events changed from raw pointers
4. **Qt::AA_ attributes**: Many moved or renamed

### Build System Changes

- **No more `.pro` files**: QMake not supported
- **Resources**: Still uses `.qrc` files, but CMake handles them with `qt_add_resources`
- **UI files**: Auto-processed by CMake with `qt_wrap_ui` (via `qt_standard_project_setup()`)
- **MOC**: Automatic via CMake's `AUTOMOC`

---

## Testing Checklist

After completing migration:

- [ ] **Compilation**: All source files compile without errors
- [ ] **Linking**: Executable links with Qt6 and ICU libraries
- [ ] **Runtime**: Application launches without crashes
- [ ] **Encodings**:
  - [ ] Apple Roman (Mac OS Roman) text decodes correctly
  - [ ] Shift-JIS (Japanese) text decodes correctly
  - [ ] ISO-8859-1 (Latin-1) text decodes correctly
  - [ ] UTF-8 text decodes correctly
  - [ ] Encoding selection in preferences works
- [ ] **Sounds**:
  - [ ] Chat sound (`chat.wav`) plays on new messages
  - [ ] PM sound (`pm.wav`) plays on private messages
  - [ ] Sound enable/disable toggle works
- [ ] **Dark Theme**:
  - [ ] Light theme uses default Qt styling
  - [ ] Dark theme loads custom stylesheet correctly
  - [ ] Auto mode detects and follows system theme
  - [ ] Theme preference is saved and restored
  - [ ] All 26 dialogs/widgets look correct in dark mode
  - [ ] Text is readable against dark backgrounds
  - [ ] Icons are visible on dark backgrounds
- [ ] **Functionality**:
  - [ ] Connect to Hotline server
  - [ ] Public chat send/receive
  - [ ] Private messaging
  - [ ] File browser navigation
  - [ ] File upload/download
  - [ ] User list display
  - [ ] News reading (linear and threaded)
  - [ ] Server agreement display
  - [ ] Tracker connection
  - [ ] Bookmarks management
  - [ ] Preferences save/load
  - [ ] Tab management (create/close tabs)

---

## Known Issues

1. **SIGNAL/SLOT Not Migrated**: Still using old-style macros (works but not ideal)
2. **ICU Dependency**: Required for Shift-JIS and Apple Roman (not optional)
3. **Qt Module Includes**: Some files still have old-style includes (works but deprecated)

---

## Performance Improvements

Qt 6 + C++23 + modern tooling provides:

- **Faster Compilation**: Better Qt MOC, modern CMake parallel builds
- **Better Runtime Performance**: Qt 6 internal optimizations
- **Modern C++**: Move semantics, constexpr, better optimizations
- **Better Memory Management**: Smart pointers, RAII patterns

---

## Development Workflow

### Recommended IDE Setup

1. **Qt Creator** (best Qt integration)
   - Open `CMakeLists.txt` as project
   - Qt Creator handles CMake configuration automatically
   - Built-in UI designer for `.ui` files

2. **VS Code** with extensions:
   - C/C++ (Microsoft)
   - CMake Tools
   - Qt tools
   - Import `compile_commands.json` for IntelliSense

3. **CLion** (JetBrains)
   - Native CMake support
   - Excellent Qt integration

### Building for Development

```bash
# Debug build with compile commands export
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build with warnings enabled
cmake .. -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic"

# Incremental build (fast)
cmake --build . -j$(nproc)
```

### Code Style

Follow existing code style:
- 4-space indentation
- Opening braces on same line
- camelCase for functions/variables
- PascalCase for classes
- Prefix `m_` for member variables (optional, not consistent in codebase)

---

## Rollback Information

If Qt 6 migration causes issues, you can:

1. **Use Qt5Compat**: Temporarily add `Qt6::Core5Compat` for `QTextCodec`
2. **Revert to QMake**: Original `Obsession.pro` still exists
3. **Git History**: All changes tracked in version control

However, Qt 4 is no longer maintained (last release: 2015), so staying modern is recommended.

---

## Resources

- **Qt 6 Porting Guide**: https://doc.qt.io/qt-6/portingguide.html
- **Qt 6 What's New**: https://doc.qt.io/qt-6/whatsnew60.html
- **CMake Qt Documentation**: https://cmake.org/cmake/help/latest/manual/cmake-qt.7.html
- **ICU Project**: https://unicode-org.github.io/icu/
- **C++23 Reference**: https://en.cppreference.com/w/cpp/23

---

## Credits

Original Obsession by [@tjohnman](https://github.com/tjohnman)

Qt 6 modernization includes contributions from the community and automated migration tools.
