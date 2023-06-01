# CS50 TSE Indexer
## Implementation Spec - Chip Nguyen

This document details the implementation specifications for the indexer - a part of the Tiny Search Engine.

## Data structures 
The key data structure is the index, mapping from word to (docID, #occurrences) pairs. The index is a hashtable keyed by word and storing counters as items. The counters is keyed by docID and stores a count of the number of occurrences of that word in the document with that ID.

## Control flow
The Indexer is implemented in one file `indexer.c`, with 3 functions.

```c
int main(int argc, char* argv[]);
int indexBuild(char* pageDirectory, hashtable_t* ht);
void indexPage(index_t* index, webpage_t* page, int docID);
```

### main
The `main` function first verifies that all inputs/arguments are valid. It then calls `indexBuild` to build an index from a given directory. Finally, it calls `index_save` from `index.h` to print content of the index to a target directory.

### indexBuild
`indexBuild` takes a pageDirectory and a hash table. It builds an in-memory index from webpage files it finds in the pageDirectory. Pseudocode: 

```
  initialize id = 1 and construct a path to pageDirectory/id
  open file path. while file is not null
    call pagedir_load to load a webpage from the path. if successful:
      pass this webpage to indexPage
      delete the webpage
    increment the id
    construct a new path pageDirectory/id with the new id
    close file path
  free paths
  return 0
```

### indexPage
`indexPage` takes an index (hashtable), a webpage, and an ID. It scans a webpage document to add its words to the index. Pseudocode:

```
 steps through each word of the webpage,
  skips trivial words (less than length 3),
  normalizes the word (converts to lower case),
    add the word to the index by calling index_insert
```

## Other modules
We created helper modules for our tiny search engine. These modules are stored in the `common` directory and are compiled together to create a re-usable library `common.a`.

### pagedir
We create a module `pagedir.c` to handle page directories. This module contains the following functions:

```c
bool pagedir_init(char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(char* pageDirectory);
webpage_t* pagedir_load(char* pageDirectory, int id);
```

- `pagedir_init` to verify if the pageDirectory is valid or accessible. Pseudocode:

```
  construct the pathname for the .crawler file in that directory
  open the file for writing; on error, return false.
  close the file and return true.
```

- `pagedir_save` to write a page's URL, depth, and HTML content to a file. Pseudocode:

```
	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file
```

- `pagedir_validate` to verify whether a directory is a Crawler-produced directory. Pseudocode:

```
	construct the pathname for the .crawler file in the directory
	check if the file .crawler located at this path exists and is accessible
	return the accessibility of the file (true if file exists and accessible, otherwise false)
```

- `pagedir_load` to load a page from a file in the directory. Pseudocode:

```
	read the first 2 lines in the file and store them to variables url and depth respectively
	read the html content of the file
	create a new webpage struct with the ur, depth, and html content
	verifies if webpage is successfully created
```

### words
The `word` module in the `common` directory contains a function to turn a word to all-lowercase. Pseudocode:

```
  if input word is null, return
  iterate through each character of the word
    convert that character to lowercase
```

### index
The `index` module in the `common` directory implements an abstract index_t type that represents an index in memory. It also contains functions to deal with this index. The functions prototypes:

```c
int index_insert(hashtable_t* ht, const char* result, int id);
int index_save(hashtable_t* ht, char* targetDirectory);
void index_delete(hashtable_t* ht);
int createInvertedIndex(hashtable_t* ht, FILE*fp);
```

- `index_insert` is used to insert a word into the hashtable. Pseudocode:

```
  find the counter representing the word is in the hashtable
    if find returns null (word not been added to table before):
      create a new counter (count is initially 0)
      insert the counter to the table
    increment the counter
  return 0 if successful
```

- `index_save` writes indexes in the hashtable to a file. Pseudocode:

```
  check if target file exists and is writable
  iterate through the elements in the hashtable and write each elements to target file in the form (word [id occurence] [id occurence] ...)
  
```

- `index_delete`deletes the index.

- `createInvertedIndex` reads an indexer-produced file and load file content to an inverted index/hashtable. Pseudocode:

```
  loop through all lines of file. while is not the end of file:
    read the word at the beginning of the line
    create a new counter
    read the [id occurence] pair on the line
    set the counter with id and count
    insert the counter to the hashtable, with the word as the key
```

## Error handling and recovery
Errors are extensively handled internally within all the functions. Each function carefully checks if all inputs are correct and all sub-tasks are successfully carried out. If any invalid input or unsuccessful task is detected, the function exits non-zero with an error message in stderr. Valgrind is used to detect memory issues and gdb is used to degub the program.

## Testing plan
Here is an implementation-specific testing plan.

### Unit testing
A program `indextest` which is implemented by `indextest.c` will serve as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file. This test run is written in testing.sh.

### Integration/system testing
We wrote a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments:
  - First, call the `indexer` with a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
  - Second, call the `indexer` with a sequence of invocations with valid arguments. Verify correct behavior by studying the outputs, and by sampling the files created in the respective pageDirectories.
  - Third, use`indextest` to test the outputs from the second step. 
  - Valgrind was used to ensure that there's no memory issue in both `indexer` and `indextest`.