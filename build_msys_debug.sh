#!/bin/sh


CC=clang cmake . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR="MinGW Makefiles"
CC=clang cmake --build . --config Debug
