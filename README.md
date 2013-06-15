Obsession
=========

A Hotline client written in Qt featuring Shift-JIS support and other stuff.

Current official build is [a107](http://www.sumamimasen.com/obsession/ObsessionAlpha-a107.zip)

The Obsession Hotline client aims to be a modern alternative for Windows users. It supports Shift-JIS encoding for Japanese users or for browsing Japanese servers. It is written using Qt and although porting it to other operating systems should be trivial, right now it only works on Windows, as there are other much better alternatives for Mac OS X in development: http://hotline.wikia.com/wiki/Clients.

What works
----------

Right now the client is capable of reading news (both old-style and threaded), downloading files with a completely functional download queue (pause, resume, etc.), trackers and bookmarks and of course chatting (including crude private messages). The first stages of sound notification are implemented and the chat font can be changed from the preferences dialog, among other things.

What doesn't work
-----------------

SHIFT-JIS support broke when the code switched to Qt5. It is one of the priorities. UTF-8 works fine as an alternative though.

Uploading and posting news is not implemented. There's no account managing either, so the client is not good for server administration right now.

Note this is an alpha build, it's pretty stable but may lack some features.
Use this program at your own risk. Should you find any bugs, please activate the debugging log (in the preferences dialog) and reproduce the bug (so that it gets registered in the log). The post a bug report.

