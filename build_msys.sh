#!/bin/sh


cmake . -DCMAKE_GENERATOR="MinGW Makefiles"
mingw32-make.exe
