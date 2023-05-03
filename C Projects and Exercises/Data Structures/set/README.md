# CS50 Lab 3
## CS50 Spring 2023

### set

A set maintains an unordered collection of (key,item) pairs; any given key can only occur in the set once. It starts out empty and grows as the caller inserts new (key,item) pairs. The caller can retrieve items by asking for their key, but cannot remove or update pairs. Items are distinguished by their key.



### Usage

The *set* module, defined in `set.h` and implemented in `set.c` with the following functions:

```c
set_t* set_new(void);
bool set_insert(set_t* set, const char* key, void* item);
void* set_find(set_t* set, const char* key);
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) );
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void set_delete(set_t* set, void (*itemdelete)(void* item) );
```

### Implementation

We implement this set as a linked list. Each node in the list is a `struct setnode_t`, a type defined internally to the module.

### Files

* `Makefile` - compilation procedure
* `set.h` - the interface
* `set.c` - the implementation
* `settest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`
* `test.names` - input file for testing

### Compilation

To compile, simply `make test.o`.

### Testing

The `test.c` program reads lines from stdin and stuffs them into set, then pulls them back out. It tests a few error and edge cases. The file test.names provides input for testing.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.