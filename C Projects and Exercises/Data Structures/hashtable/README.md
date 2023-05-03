# CS50 Lab 3
## CS50 Spring 2023

### hash table

A hashtable is a set of (key,item) pairs. It acts just like a set, but is far more efficient for large collections.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c` with the following functions:

```c
hashtable_t* hashtable_new(const int num_slots);
bool hashtable_insert(hashtable_t* ht, const char* key, void* item);
void* hashtable_find(hashtable_t* ht, const char* key);
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) );
```

### Implementation

We implement this hash table as an array of pointers to sets.

### Files

* `Makefile` - compilation procedure
* `hash.h` - interface of jenkins hashing function
* `hash.c` - implementation of jenkins hashing function
* `hashtable.h` - the interface of hashtable
* `hashtable.c` - the implementation of hashtable
* `hashtabletest.c` - unit test driver
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make hashtable.o`.

### Testing

The `hashtabletest.c` program reads lines from stdin and stuffs them into a hash table, then pulls them back out. It tests a few error and edge cases.
It doesn't have a test input file like test.names in the bag module. The inputs are given inside main() of `hashtabletest.c`.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.