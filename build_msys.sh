#!/bin/sh


cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR="MinGW Makefiles"
cmake --build . --config Release
