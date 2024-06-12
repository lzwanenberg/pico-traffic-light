# Raspberry Pico Traffic Light

This repository contains code for a basic traffic light system designed for the Raspberry Pico W board. The system includes two signal heads: one for pedestrians and one for vehicular traffic. A push button allows pedestrians to request a green signal.

- [Prerequisites](#preqrequisites)
- [Installation](#installation)
- [Other commands](#other-commands)
- [Demo](#other-commands)

## Prerequisites

- Unix-like environment
- [CMake](https://cmake.org/)

## Installation

```.sh
./run/init.sh # initialize build directory
./run/build.sh # build the project
./run/start.sh # start the application
```

## Other commands

```.sh
./run/test.sh # run the test suite
./run/test.sh Factorial # run tests matching *Factorial* filter
./run/clean.sh # remove build directory
./run/copy.sh # copy .uf2 file to Pico
```

## Demo

https://github.com/lzwanenberg/pico-traffic-light/assets/5617056/7a1c173c-b91b-457a-b4fb-cbfd0717aa88
