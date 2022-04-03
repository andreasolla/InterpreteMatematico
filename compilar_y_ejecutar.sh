#!/usr/bin/env bash
mkdir build
cp concurrentSum.go /build
cd build
cmake ..
make
./ComInt

