#!/bin/sh


cmake . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR="MinGW Makefiles"
cmake --build . --config Debug
