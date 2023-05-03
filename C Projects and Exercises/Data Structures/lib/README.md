# CS50 Lab 3
## CS50 Spring 2021

### lib

This directory contains a little library of useful functions.

The `Makefile` compiles each of the source (`.c`) files, creating object (`.o`) files that can be linked into your programs.
To see how they are used, examine `../bag/Makefile`.

#### mem

The *mem* (memory) module provides some convenient replacements for `malloc` and `free` that help track a count of the number of allocations and frees.
Useful for debugging.

#### file

The *file* module includes variants of the familiar `readLinep()` function, which makes it easy to read a line of input, of unbounded size.

