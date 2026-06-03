# c_templates_capi

A proof-of-concept tool that generates C macro-based generics using libclang. Built as an early learning project introducing me to compiler tooling and design as well as being the reason for [trt](https://github.com/GeorgeTerzis/trt), my ongoing LLVM-based programming language project
## What it does

C doesn't have generics. The usual workarounds are `void*`, macros written by hand. Here you annotate your C structs and functions with special naming conventions, and the tool generates a re-includable header that uses `##` token-pasting macros to stamp out type-safe instantiations.

The output is a header you can include multiple times with different type definitions:

## Why
I wanted an excuse to dig into libclang.The goal was to see if I could make C generics less painful to write by hand. I didn't really succeed.

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

Known rough edges:

- Circular dependencies between template entities can cause strange ordering issues in the output
- Type parameters must be single-word identifiers (pointer types need to be `typedef`'d first)
- `enum` support is not fully implemented
- Functions whose call-expressions appear before their definitions in the translation unit may have incomplete nested type information
- The input filename and prefix are not yet configurable at runtime (work in progress)

## Building

Requires libclang. Adjust the include/library paths for your system:

compiling with g++ works but I use clang so I would suggest using that, but keep in mind I have seed gcc fail compiling when clang passed just fine.

```sh
clang++ ./src/main.cpp -lclang -o main.out
```

## Usage

```sh
./main.out <input_file>
```

Output is written to stdout so you can redirect it:

```sh
./main.out ./src/my_types.c > generated.h
```
