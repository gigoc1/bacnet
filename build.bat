@echo off
echo Build with MinGW32 and MSYS: mingw.sourceforge.net
rem set PATH=C:\Users\KDK\Downloads\mingw64\bin;%PATH%
rem assumes rm, cp, size are already in path
set CC=gcc.exe
set AR=ar.exe
set NM=nm.exe
set OBJCOPY=objcopy.exe
set SIZE=size.exe
set MAKE=mingw32-make.exe

mingw32-make --version
rem doskey make = mingw32-make.exe $*

rem Build for Mingw IP
rem mingw32-make BACNET_PORT=win32 clean
mingw32-make BACNET_PORT=win32 BUILD=debug %1

rem Build for MinGW debug
rem make BACNET_PORT=win32 BUILD=debug clean all

rem Build for MinGW MS/TP
rem mingw32-make BACNET_PORT=win32 BACDL_DEFINE=-DBACDL_MSTP=1 %1
rem make BACNET_PORT=win32 BACDL_DEFINE=-DBACDL_BIP6=1 clean all

rem On Linux, install mingw32 and use this:
rem make BACNET_PORT=win32 CC=i586-mingw32msvc-gcc AR=i586-mingw32msvc-ar clean all
