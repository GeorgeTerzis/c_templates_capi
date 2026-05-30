#!/bin/bash

clang++ ./src/main.cpp -lclang -foptimize-sibling-calls -O3 -g0 -o main.out
