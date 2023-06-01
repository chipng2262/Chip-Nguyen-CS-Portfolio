# CS50 Spring 2023
# Tiny Search Engine

### common library

This directory provides modules that are helpful for the implementation of the Tiny Search Engine. It contains:

### pagedir
The module `pagedir.c` handles tasks related to extracting and loading information directories. This is used for the crawler, indexer, and querier. Function prototypes:

```c
bool pagedir_init(char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(char* pageDirectory);
webpage_t* pagedir_load(char* pageDirectory, int id);
char* pagedir_getLink(char *pageDirectory, int id);
```

### index
The `index` module implements an abstract index_t type that represents an index in memory. It also contains functions to deal with this index. Function prototypes:

```c
int index_insert(hashtable_t* ht, const char* result, int id);
int index_save(hashtable_t* ht, char* targetDirectory);
void index_delete(hashtable_t* ht);
int createInvertedIndex(hashtable_t* ht, FILE*fp);
```

### word
Provides a function to normalize a word, which is useful for the indexer and querier. 

```c
void normalizeWord(char *word)
```

