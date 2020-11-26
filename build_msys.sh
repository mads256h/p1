#!/bin/sh


CC=clang cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR="MinGW Makefiles"
CC=clang cmake --build . --config Release
