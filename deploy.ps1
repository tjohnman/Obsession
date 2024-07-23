
# Adjust these paths as necessary.

$mingwPath = "C:\i686-4.8.2-release-posix-dwarf-rt_v3-rev3\mingw32\bin"
$qtPath = "C:\Qt\4.8.7\bin"

###

$ErrorActionPreference = "Stop"
$env:Path = "$mingwPath;$qtPath;$env:Path"

if (!(Test-Path -Path "deploy")) {
    New-Item -ItemType Directory -Path "deploy"
}

Set-Location -Path "deploy"

qmake -config release ..\Obsession.pro
make

Copy-Item release\obsession.exe .\Obsession.exe

Remove-Item -Recurse -Force -Path "release"
Remove-Item -Recurse -Force -Path "debug"
Get-ChildItem -Filter "*.h" | Remove-Item -Force
Get-ChildItem -Filter "Makefile*" | Remove-Item -Force
Get-ChildItem -Filter "object_script*" | Remove-Item -Force

Copy-Item (Join-Path $qtPath "libgcc_s_dw2-1.dll") .
Copy-Item (Join-Path $qtPath "libstdc++-6.dll") .
Copy-Item (Join-Path $qtPath "libwinpthread-1.dll") .
Copy-Item (Join-Path $qtPath "QtCore4.dll") .
Copy-Item (Join-Path $qtPath "QtGui4.dll") .
Copy-Item (Join-Path $qtPath "QtNetwork4.dll") .

Copy-Item -Recurse ..\resources\sounds .

Set-Location -Path ..
Compress-Archive -Path "deploy\*" -DestinationPath "obsession-alpha-qt4-windows.zip"
Remove-Item -Recurse -Force -Path "deploy"