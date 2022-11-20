Obsession
=========

A Hotline client written in Qt featuring Shift-JIS support and other stuff.

The Obsession Hotline client aims to be a modern alternative for Windows and macOS users. It supports Shift-JIS encoding for Japanese users or for browsing Japanese servers. It is written using Qt and should compile and work out of the box on Windows, macOS and Linux. Check out other clients in the [Hotline Wiki](http://hotline.wikia.com/wiki/Clients).

Downloading and installing
----------

You can get release builds [here](https://github.com/tjohnman/Obsession/releases).

Building
--------

You might need to download and install [Qt 4.8.7](https://download.qt.io/archive/qt/4.8/4.8.7/) for your platform.

Then create a `build` directory inside the directory where you unpacked the source if it doesn't exist, and run `qmake ../Obsession.pro` from the `build` directory. This will generate a Makefile for you. You can then run `make` normally in order to build your binary.

What doesn't work
-----------------

Posting news is not implemented. Folder uploading and downloading isn't either.

Note this is an alpha build, it's pretty stable but may lack some features.
Use this program at your own risk. Should you find any bugs, please activate the debugging log (in the preferences dialog) and reproduce the bug (so that it gets registered in the log). Then post a bug report.

