#!/bin/bash

cd build/release
make "$@"
cd ../..

cd build/tests
make "$@"
cd ../..
