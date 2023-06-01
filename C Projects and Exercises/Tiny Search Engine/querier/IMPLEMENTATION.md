# CS50 TSE Querier
## Implementation Spec - Chip Nguyen, Spring 2023

In this document we focus on the implementation-specific decisions in developing the querier for the Tiny Search Engine.

## Data structures and types
We used the two main data structures:
- hashtable: We used the hashtable to store an inverted index representing a word and its ocurrence in different documents.
- counters: We used counters to store documents that matched a query. An item in the counters is in a pair (docID, score), which stores the ID of the document a word is in and its score in that document.

We also defined a struct document_t, which represents (1) a word's document's ID (the ID of the document that has the words) and (2) the score of the word in that document. This struct is useful for ranking documents, as will be explained below. 

```c
typedef struct {
    int id;
    int score;
} document_t;
```

## Function prototypes
The querier module is implemented in one file `querier.c`, with the following functions divided into groups

```c
// main and query functions
int main(int argc, char *argv[]);
static void makeQuery(index_t *index, char *pageDirectory);

// query processing functions
static bool validateQuery(char *query);
static char** tokenizeQuery(char *query, int word_count);
static bool checkOperators(char *words[], int wordCount);
static counters_t *processOperators(char *word[], index_t *index, int word_count);
static int countWords(char *query);

// set functions
static void mergeUnion(counters_t **thisSet, counters_t **otherSet);
static void mergeUnionHelper(void *arg, const int key, int count);
static void mergeIntersection(counters_t **thisSet, counters_t *otherSet);
static void mergeIntersectionHelper(void *arg, const int key, int count);
static void mergeSet(counters_t **andSet, counters_t **orSet);

// ranking functions
static void printByRank(counters_t *score, char *pageDirectory);
static void docComparator(void *arg, int key, int count);
static void findTopDocument(counters_t *score, document_t *topDoc);
```

## Control flow and pseudocodes
  
### 1. main
The `main` function:
  - validates command line arguments. print error and exit non-zero if invalid command is given
  - create an inverted index by calling `createInvertedIndex` from the `index` module
  - make query using the index by calling the `makeQuery` function
  - exit zero if all tasks are successfully carried out

### 2. makeQuery
The `makeQuery` function prompts a query from an user and processes that query. Pseudocode: 

```
print "Query? " to user's screen
read user's input from stdin until end of input:
  validate query by calling `validateQuery`. if query is valid:
    count words in query by calling `countWords`
    if wordcount == 0 (user didn't input anything):
      prompt "Query? " again,
      continue,
    else tokenize query by calling `tokenizeQuery`
    print out query words
    check if all operators are valid by calling `checkOperators`. if valid:
      create intersection/union sets according to operators by calling `processOperators'
      print ranked documents by calling `printByRank`
      prompt new query;
    free memory
```

### 3. validateQuery
`validateQuery` makes sure query contains only valid characters. Pseudocode:

```
loop over each character in the query:
  if a non-space, non-alphabetical character is found:
    print error message
    return false
  else return true
```

### 4. tokenizeQuery
`validateQuery` creates an array to store the words of a valid query. Pseudocode:

```
create new array to store words
initialize inWord variable to keep track of whether current character is in a word
initialize variable to keep track of beginning of word
loop through each character of query:
  if whitespace encountered after a word:
    normalize, tokenize it, and add to array
    set inWord to false,
  else (character is a letter):
    mark current char as start of word
    set inWord to true
  increment current char
  process the last word
```

### 5. checkOperators
`checkOperators` check if positions of operators in query are valid. Pseudocode:

```
if first word and last word are 'or' or 'and':
  print error message and return false
loop through each word
  if current word is an operator:
    compare current word with next word. if next word is also an operator:
      print error message and return false
return true
```

### 6. processOperators
`processOperators` create intersection/union sets according to operators in the query. Pseudocode:

```
initialize andSet and orSet to null;
initialize skipWord to false, which tells us whether to skip a word.
loop through each word in word array:
  if current word is 'and', continue
  if current word is 'or':
    merge 'andSet' and 'orSet'
    set skipWord to false so the next word is not skipped
    continue to get next word.
  if skipWord is true, keep processing
  if current word is not an operator, find its counter in the index
  if counters not found, set skipWord to true to stop making andSet. delete andSet
  if counters found:
    if andSet is null, create new andSet and mergeUnion counters into andSet
    if andSet not null, mergeIntersection counters into andSet
merge andSet and orSet
return orSet, which is the combination of andSet and orSet.
```

### 7. countWords
`countWords` returns the number of words in the query. This is useful for creating an array of words during tokenization. Pseudocode:

```
intialize count
skip leading whitespaces
while end of query is not reached:
  if a character is reached meaning a word is found:
    increment count
    skip all other characters in that word
  else skip characters
  skip trailing whitespaces
return count
```

### 8. mergeUnion, mergeIntersection, mergeSet
These functions merge 2 counters by creating an union or intersection depending on the operators. 
- `mergeUnion` calls helper function `mergeUnionHelper`, and `mergeIntersection` calls `mergeIntersectionHelper`. 
- `mergeSet` merges an andSet and an orSet together by calling `mergeUnion`.

These functions were inspired by the iterator knowledge unit in class.

### 9. printByRank
`printByRank` first ranks document_t based on score then prints out their ids and scores. These are the outputs of the query. Pseudocode:

```
initialize a top document variable topDoc with id = 0 and score = 0
initialize a count variable to count how many documents have been processed
while topDoc.score >= 0:
  reset topDoc.score to 0 (for each iteration after)
  find document with highest score and load its id and score into topDoc
  if topDoc.score = 0 and no documents have been successfully processed:
    print no documents found and break out of while loop
  else (meaning that all docs have been ranked) break out of loop
  set value of top document to 0 in the index to avoid it being processed again
  get link to webpage represented by the file that has ID ass topDoc.id
  print topDoc's score, id, and url
  increment count
```

### 10. findTopDocument
`findTopDocument` is an iterator that loops through all counters in the index to identify the counter with highest score. The data of this counter is then loaded to topDoc for processing in `printByRank`.

## Other modules
Functions from other modules in the `common` library were used to aid the implemention of the querier:

- From `word`: `createInvertedIndex` was used to load information from input files into an inverted index later used for querying.
- From `index`: `normalizeWord` was used to bring query characters to all lower-case
- From `pagedir`: `pagedir_validate` to make sure pageDirectory is a valid crawler-produced directory

- A new function `pagedir_getLink` was added to `pagedir` to retrieve the url of a webpage stored in a document. Function protype and pseudocode:

```c
char* pagedir_getLink(char *pageDirectory, int id);
```
```
construct the path to the file in the form pageDirectory/id
open the file for readingusing the concantenated path
if open is successful:
  load a webpage from data in the file
  get the link of that webpage
  return the link
else return NULL
```

## Error handling and recovery
All command-line inputs are extensively checked and tested before any other process begins. If an invalid command-line input is encountered, the program prints an error message to stderr and exits non-zero. 

Memory errors are identified and traced using Valgrind. gdb was also used for debugging and identifying isses.

Some errors are caught and handled internally using series of if-else statements and validation/verification functions as detailed above. These errors all produce error messages so the user knows what went wrong. 

## Testing plan
Here is an implementation-specific testing plan.

## Unit testing
During development, many functions were tested individually using techniques such as print statements, small driver files, etc. to ensure they're behaving as expected.

## Integration/system testing
We write a script testing.sh that invokes the querier several times, with a variety of command-line arguments. First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made. Second, a run with valgrind over a moderate-sized test case to check for memory issues. Verify correct behavior by studying the output. 

### Fuzz testing
The querier is tested using the `fuzzquery.c` program, which is a form of black-box testing that generates a random set of tests that are fed to the program. 
