# CS50 Lab 3
## CS50 Spring 2021

### bag

A `bag` is an unordered collection of _items_.
The `bag` starts empty, grows as the caller adds one _item_ at a time, and shrinks as the caller extracts one _item_ at a time.
It could be empty, or could contain hundreds of _items_.
_Items_ are indistinguishable, so the _extract_ function is free to return any _item_ from the `bag`.

### Usage

The *bag* module, defined in `bag.h` and implemented in `bag.c`, implements a bag of `void*`, and exports the following functions:

```c
bag_t* bag_new(void);
void bag_insert(bag_t* bag, void* item);
void* bag_extract(bag_t* bag);
void bag_print(bag_t* bag, FILE* fp, 
	       void (*itemprint)(FILE* fp, void* item));
void bag_iterate(bag_t* bag, void* arg, void (*itemfunc)(void* arg, void* item) );
void bag_delete(bag_t* bag, void (*itemdelete)(void* item) );
```

### Implementation

We implement this bag as a linked list.
The *bag* itself is represented as a `struct bag` containing a pointer to the head of the list; the head pointer is NULL when the bag is empty.

Each node in the list is a `struct bagnode`, a type defined internally to the module.
Each bagnode includes a pointer to the `void* item` and a pointer to the next bagnode on the list.

To insert a new item in the bag we create a new bagnode to hold the `item`, and insert it at the head of the list.

To extract an item from the bag we pop off the head of the list and return its item, deleting its bagnode.
Of course, if the list is empty, we return NULL instead.

The `bag_print` method prints a little syntax around the list, and between items, but mostly calls the `itemprint` function on each item by scanning the linked list.

The `bag_iterate` method calls the `itemfunc` function on each item by scanning the linked list.

The `bag_delete` method calls the `itemdelete` function on each item by scanning the linked list, freeing bagnodes as it proceeds.
It concludes by freeing the `struct bag`.

### Assumptions

No assumptions beyond those that are clear from the spec.

The `item` inserted cannot be NULL, and thus a NULL return from `bag_extract` must indicate either error or empty bag, not simply a NULL `item` coming out of the bag.

Because of the semantics of a *bag*, we have great freedom in our implementation.
Our linked-list approach actually implements a stack: `bag_insert` pushes a new item on the stack, and `bag_extract` pops the top item off the stack.
Other implementations are permitted, but this LIFO list approach is fast and easy to implement.

### Files

* `Makefile` - compilation procedure
* `bag.h` - the interface
* `bag.c` - the implementation
* `bagtest.c` - unit test driver
* `test.names` - test data
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make bag.o`.

### Testing

The `bagtest.c` program reads lines from stdin and stuffs them into a bag, then pulls them back out.
It tests a few error and edge cases.
This test is somewhat minimal.
A lot more could be done!

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

Try testing with `MEMTEST` by editing Makefile to turn on that flag and then `make test`.

To test with valgrind, `make valgrind`.
