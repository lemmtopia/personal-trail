@echo off

set CCFLAGS=-g -Wall
set LDFLAGS=-luser32 -lgdi32 -ld2d1

clang++ %CCFLAGS% code\win32_trail.cpp -o bin\game.exe %LDFLAGS%
