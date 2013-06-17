Obsession
=========

A Hotline client written in Qt featuring Shift-JIS support and other stuff.

The Obsession Hotline client aims to be a modern alternative for Windows users. It supports Shift-JIS encoding for Japanese users or for browsing Japanese servers. It is written using Qt and although porting it to other operating systems should be trivial, right now it only works on Windows, as there are other much better alternatives for Mac OS X in development: http://hotline.wikia.com/wiki/Clients.

Downloading and installing
----------
First get the [directory structure](http://www.sumamimasen.com/obsession/ObsessionAlpha-a107.zip). This contains all the dll files needed. It's a big package, but you won't have to download it more than once.
Make sure to also grab the [executable file](http://www.sumamimasen.com/obsession/Obsession.exe). Place this in the directory you downloaded above and you are good to go. Note that this may be not up to date with the latest code from the repo. I upload regular builds though.

Getting updates
---------------
The client will connect to the Internet and check for updates on launch. If there is a new update you will be prompted and sent here if desired. Normally you want to get the [executable file](http://www.sumamimasen.com/obsession/Obsession.exe) and copy it over the older version. Should a version require a redownload of the directory structure a special notice will be posted here.
*Note* that you do need to get the file structure if you are updating from anything previous to 107.2. There have been some DLL changes.


What works
----------

Right now the client is capable of reading news (both old-style and threaded), downloading files with a completely functional download queue (pause, resume, etc.) and uploading, trackers and bookmarks and of course chatting (including crude private messages). The first stages of sound notification are implemented and the chat font can be changed from the preferences dialog, among other things.

What doesn't work
-----------------

Posting news is not implemented. There's no account managing either, so the client is not good for server administration right now.

Note this is an alpha build, it's pretty stable but may lack some features.
Use this program at your own risk. Should you find any bugs, please activate the debugging log (in the preferences dialog) and reproduce the bug (so that it gets registered in the log). Then post a bug report.

