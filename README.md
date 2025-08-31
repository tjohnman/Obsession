Obsession
=========

A Hotline client written in Qt featuring Shift-JIS support and other stuff.

The Obsession Hotline client aims to be a modern alternative for Windows users. It supports Shift-JIS and UTF-8 encoding for Japanese users or for browsing Japanese servers. It is written using Qt and should compile and work out of the box on Windows, macOS and Linux. Check out other clients in the [Hotline Wiki](http://hotline.wikia.com/wiki/Clients).

![](https://github.com/tjohnman/Obsession/blob/qt4/screenshot.png?raw=true)

**Note:** This repository includes Qt 4 and Qt 6 branches. Choose the one that best fits your platform.

Downloading and installing
----------

### Windows

You can get Windows builds [here](https://github.com/tjohnman/Obsession/releases).

### macOS

You can find Obsession on [MacPorts](https://ports.macports.org/port/Obsession/details/).

### Arch Linux

You can find Obsession on the [AUR](https://aur.archlinux.org/packages/obsession-git/).

Building
--------

You might need to download and build/install [Qt 4](https://download.qt.io/archive/qt/4.8/4.8.7/) for your platform, as well as other libraries or frameworks necessary to build Qt projects. On Windows download [MinGW](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download), and use the MinGW variant from the previous link.

- Download this repository and unpack it.
- Create a `build` directory inside the directory where you unpacked the source if it doesn't exist.
- Run `qmake -config release ../Obsession.pro` from the `build` directory. This will generate a Makefile for you.
- You can then run `make` (or `nmake`, etc. depending on your build setup) normally in order to build your binary, which will appear in the `build` directory.

You can also set up modern Qt Creator by creating a kit with Qt 4.8.7 after you have installed it from the link above.

How do I find servers to connect to?
------------------------------------

You can get started by opening the trackers dialog (Connection -> Trackers...) and adding hltracker.com. This will load a list of servers to which you can connect.

Check out https://hlwiki.com/ for more.

This is an alpha
----------------

Posting news is not implemented. Folder uploading and downloading isn't either.

Please note that this is an alpha build. It's pretty stable, but it may lack some features.
Use this program at your own risk. Should you find any bugs, please post a bug report.
