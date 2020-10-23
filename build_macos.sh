#!/bin/sh

LDFLAGS="-L/usr/local/lib" CFLAGS="-I/usr/local/include" cmake . -DCMAKE_BUILD_TYPE=Release
LDFLAGS="-L/usr/local/lib" CFLAGS="-I/usr/local/include" cmake --build . --config Release
