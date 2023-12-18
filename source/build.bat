@echo off

set CCFLAGS=-Wall
set LDFLAGS=-luser32 -lgdi32

clang++ %CCFLAGS% code\win32_trail.cpp -o game.exe %LDFLAGS%
