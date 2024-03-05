#!/bin/bash

mkdir build

# Debug
mkdir build/release
cd build/release
cmake -DTARGET=release -DPICO_BOARD=pico_w ../..
cd ../..

# Tests
mkdir build/tests
cd build/tests
cmake -DTARGET=tests ../..
cd ../..
