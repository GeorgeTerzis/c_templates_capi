#!/bin/bash

INFILE="./test2.c"

clang  -Xclang -ast-dump -fsyntax-only -xc -std=c2x "$INFILE"
