#!/bin/sh

cmake . -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
