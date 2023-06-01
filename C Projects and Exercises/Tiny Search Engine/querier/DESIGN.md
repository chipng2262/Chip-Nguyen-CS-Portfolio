# CS50 TSE Querier
## Design Spec - Chip Nguyen, Spring 2023

The TSE **Querier** is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### User interface

The querier's only interface with the user is on the command-line; it must always have three arguments.

 ```
 ./querier pageDirectory indexFilename
 ```
   * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
   * where `indexFilename` is the pathname of a file produced by the Indexer.

### Inputs and outputs

Input: the only inputs are command-line parameters.

Output:

```
Query? possible IDENTIFIES or espA
Query: possible identifies or espa 
score  2 doc  7: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Computer_science.html 
score  1 doc  2: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Linked_list.html 
score  1 doc  3: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Hash_table.html 
score  1 doc  5: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Unix.html 
score  1 doc  6: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/C_(programming_language).html 
score  1 doc  4: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Dartmouth_College.html 
```

### Functional decomposition into modules

We anticipate the following functions in querier.c. These functions are divided into groups:

- Main function and query function:
```c
int main(int argc, char *argv[]);
static void makeQuery(index_t *index, char *pageDirectory);
```

- Set functions: These are used to merge counters in accordnacw with 'and' and 'or' operators.
```c
static void mergeUnion(counters_t **thisSet, counters_t **otherSet);
static void mergeUnionHelper(void *arg, const int key, int count);
static void mergeIntersection(counters_t **thisSet, counters_t *otherSet);
static void mergeIntersectionHelper(void *arg, const int key, int count);
static void mergeSet(counters_t **andSet, counters_t **orSet);
```

- Query processing functions: These are used to validate, tokenize, and process the query from the user.
```c
static bool validateQuery(char *query);
static char** tokenizeQuery(char *query, int word_count);
static bool checkOperators(char *words[], int wordCount);
static counters_t *processOperators(char *word[], index_t *index, int word_count);
static int countWords(char *query);
```

- Ranking functions: These are used to implement the ranking mechanism for the querier's output.
```c
static void printByRank(counters_t *score, char *pageDirectory);
static void docComparator(void *arg, int key, int count);
static void findTopDocument(counters_t *score, document_t *topDoc);
```

- Functions from other modules: `pagedir`, `index`,`word`, `webpage`, and `hashtable` will also be used.

### Pseudo code for logic/algorithmic flow

1. Execute from a command line with valid parameters `./querier pageDirectory indexFilename`
2. Validate command-line arguments to make sure that:
	* `pageDirectory` is the path to a directory produced by the `crawler`.
	* `indexFilename` is the path to a readable file that is produced by the `indexer`. 
3. Create an inverted index from data in input files.
4. Get input query from user. Validate, normalize, and tokenize the query. Print error messages if an invalid query is given.
5. Process operators and create union/intersection sets accordingly with reference to the inverted index:
  * *and/intersection* set: sequences of words connected by 'and'.
  * *or/union* set: sequences of words connected by 'or'.
6. Rank the documents, obtain their urls, and print out the ranked documents. Print out "No documents match" if no documents match. 
8. Exit with zero status when EOF is reached on stdin.

### Dataflow through modules
1. `main` verifies inputs, load data from input files to an inverted index, and pass the inverted index to `makeQuery`
2. `makeQuery` asks for a query from the user at the beginning of the search and after everytime a query has been processed. It makes call to these functions. 
  2.1. `validateQuery` to make sure the query is valid.  
  2.2. `countWords` to count the number of words entered.  
  2.3. `tokenizeQuery`, which calls `normalizeWord` to normalize a word, to create an array of words from the query.  
  2.4. `checkOperators` to make sure that operators are in valid positions only.  
  2.5. `processOperators` to create unions/intersections based on operators in the query and return a resulting counters.  
  2.6. `printByRank`, which calls `findTopDocument`, to sort documents stored in the counters above by order of decreasing score. Print out the document's ID and score in that orders if there are matching documents.  

### Major data structures
We use a hashtable to present an index, which contains pairs of (docID, count) that represent a word's document and its frequency in that document. Each pair is a counter where docID is the key and count is the value. 

Data structures used: counters, hashtable, set. 

We also define a local type document_t, which is an array storing (id, score) pairs. Each pair represents a document's ID and the score of a word in that document. This document_t type is used for sorting documents in order of decreasing scores.

### Testing plan

**Fuzzy testing**: A form of black-box testing that generates hundreds of random inputs of different lengths to the program. The file `fuzzquery.c`implements this random test generation, which is then piped into the querier in stdin.

**Manual testing**: Bad cases with invalid inputs will be fed into the querier to see if it correctly handles the errors. Casess include invalid number of command line argumnenets, invalid/unreadable paths to pageDirectory and indexFilename, invalid queries with invalid syntax and wrong operator positions. 

**Memory testing**: Use `valgrind` to identify memory errors.
