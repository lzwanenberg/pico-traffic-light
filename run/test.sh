#!/bin/bash

param_count=$#
tests_exec="./build/tests/tests"

if [[ $* == *--watch* ]]; then
  python3 ./run/python/test-watcher.py
else
  "$tests_exec" "${@:1}"
fi
