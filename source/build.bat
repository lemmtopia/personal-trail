@echo off

set CCFLAGS=-g
set LDFLAGS=-luser32 -lgdi32

clang++ %CCFLAGS% code\win32_trail.cpp -o bin\game.exe %LDFLAGS%
clang++ -shared -o bin\game.dll code\personal_trail.cpp -luser32
