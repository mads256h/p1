#!/bin/sh

LDFLAGS="-L/usr/local/lib" CFLAGS="-I/usr/local/include" cmake .
LDFLAGS="-L/usr/local/lib" CFLAGS="-I/usr/local/include" make
