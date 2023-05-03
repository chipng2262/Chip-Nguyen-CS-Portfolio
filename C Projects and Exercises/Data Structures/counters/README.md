# CS50 Lab 3
## CS50 Spring 2023

### counters

A counter set is a set of counters, each distinguished by an integer key. It's a set - each key can only occur once in the set - and it tracks a counter for each key. It starts empty. Each time counters_add is called on a given key, the corresponding counter is incremented. The current counter value can be retrieved by asking for the relevant key.

### Usage

The *counters* module, defined in `counters.h` and implemented in `counters.c` with the following functions:

```c
counters_t* counters_new(void);
int counters_add(counters_t* ctrs, const int key);
int counters_get(counters_t* ctrs, const int key);
bool counters_set(counters_t* ctrs, const int key, const int count);
void counters_print(counters_t* ctrs, FILE* fp);
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count));
void counters_delete(counters_t* ctrs);
```

### Implementation

We implement this counter as a linked list. Each node in the list is a `struct countersnode`, a type defined internally to the module.

### Files

* `Makefile` - compilation procedure
* `counters.h` - the interface
* `counters.c` - the implementation
* `counterstest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make counters.o`.

### Testing

The `counterstest.c` program reads lines from stdin and stuffs them into a bag, then pulls them back out. It tests a few error and edge cases.
It doesn't have a test input file like test.names in the bag module. The inputs are given inside main() of `counterstest.c`.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.