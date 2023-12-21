@echo off

set CCFLAGS=-g
set LDFLAGS=-luser32 -lgdi32

clang++ %CCFLAGS% code\win32_trail.cpp -o bin\game.exe %LDFLAGS%
