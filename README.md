# c_templates_capi

A proof-of-concept tool that generates C macro-based generics by parsing C source files with libclang. Built primarily as a learning project to explore what's possible with the libclang API — not something you'd want in production, but it works (kinda). It is also something I wrote a long time ago I am aware that you can go about it in a better way(use C++) and maybe structure the macros in a better way and use concat macros to help parametrise the names of the functions and types instead of using this tool.

## What it does

C doesn't have generics. The usual workarounds are `void*` (no type safety), macros written by hand (tedious and error-prone), or code generation. This tool takes a third approach: you annotate your C structs and functions with special naming conventions, and the tool generates a re-includable header that uses `##` token-pasting macros to stamp out type-safe instantiations.

The output is a header you can include multiple times with different type definitions:

## Why
I wanted an excuse to dig into libclang. I tried the C++ API first but the abstractions felt heavy, so I dropped down to the C API which turned out to be much more manageable.
The goal was to see if I could make C generics less painful to write by hand. I didn't really succeed, but I learned a lot about AST traversal along the way and it gave an intrest for compilers and code generation tools in general.

```c
#define a_ti int
#define b_ti float
#include "generated.h"

#define a_ti char
#define b_ti double
#include "generated.h"
```

Each include produces an independent set of types and functions with mangled names like `my_entity_int_float`.

## How it works

1. The input C file is parsed with libclang into an AST
2. The AST visitor walks declarations and classifies them by naming convention into **template inputs**, **template entities**, and **template auxiliaries**
3. Type information (pointers, function pointers, blocks, arrays, enums, records) is recursively analysed to track which template parameters each declaration depends on
4. A header is emitted with:
   - `#error` guards ensuring all type parameters are defined before inclusion
   - `CONCAT_N` macros for name mangling up to 10 type parameters
   - An auxiliary section (guarded by `#ifndef`) for shared helpers that don't need re-instantiation
   - An entity section with `CONCAT`-based aliases for structs and functions
   - Full `#undef` cleanup at the end so the header is safely re-includable

## Naming conventions

The tool classifies declarations by suffix:

| Suffix | Meaning |
|--------|---------|
| `_ti`  | Template input — a type parameter placeholder |
| `_te`  | Template entity — the generic struct or function being instantiated |
| `_ta`  | Template auxiliary — a helper shared across instantiations |

Anything not matching these patterns is ignored.

## Limitations

This is a proof of concept. Known rough edges:

- Circular dependencies between template entities can cause strange ordering issues in the output
- Type parameters must be single-word identifiers (pointer types need to be `typedef`'d first)
- `enum` support is not fully implemented
- Functions whose call-expressions appear before their definitions in the translation unit may have incomplete nested type information
- The input filename and prefix are not yet configurable at runtime (work in progress)

## Building

Requires libclang. Adjust the include/library paths for your system:

compiling with g++ works but I use clang so I would suggest using that, but keep in mind I have seed gcc fail compiling when clang passed just fine.

```sh
clang++ -std=c++20 main.cpp -lclang -o c_templates_capi
```

## Usage

```sh
./main.out <input_file>
```

Output is written to stdout so you can redirect it:

```sh
./main.out ./src/my_types.c > generated.h
```
