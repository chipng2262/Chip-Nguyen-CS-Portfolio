/*
 * querier.c implements a querier for the tiny search engine
 *
 * Chip Nguyen, CS50 Spring 2023
 * May 9, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../common/pagedir.h"

/**************** local type ****************/
// document_t stores a document's ID and its score
// used later for sorting the documents by scores
typedef struct {
    int id;
    int score;
} document_t;

/**************** functions prototype ****************/
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

/**************** main ****************/
/* main function for querier */
int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Error: invalid number of arguments.\n");
        exit(1);
    }

    char *pageDirectory = argv[1];
    char *indexFilename = argv[2];

    if (!pagedir_validate(pageDirectory))
    {
        fprintf(stderr, "Error: invalid page directory.\n");
        exit(1);
    }

    FILE *fp = fopen(indexFilename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error: invalid index file name.\n");
        exit(1);
    }

    int size = file_numLines(fp);

    // create an empty index
    index_t *index = hashtable_new(size);

    // load an inverted index
    createInvertedIndex(index, fp);
    makeQuery(index, pageDirectory);

    // free memory
    index_delete(index);
    return 0;
}

/**************** makeQuery ****************/
/* function to ask for users' query and process it */ 
static void makeQuery(index_t *index, char *pageDirectory)
{
    char *query;
    char **words;

    printf("Query? ");

    while ((query = file_readLine(stdin)) != NULL)
    {
        // check if query is valid
        if ((validateQuery(query)) == true)
        {
            int word_count = countWords(query);

            // check if users have entered any words
            if (word_count == 0) {
                if (query != NULL) free(query);
                printf("Query? ");
                continue;
            }

            words = tokenizeQuery(query, word_count);

            // print out the words
            printf("\nQuery: ");
            for (int i = 0; i < word_count; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");

            // check if all operators are valid
            if (checkOperators(words, word_count) == true) {
                counters_t *counter = processOperators(words, index, word_count);

                // print the rank of documents
                printByRank(counter, pageDirectory);
                counters_delete(counter);
                printf("\nQuery? ");

                // free all the words in the array
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
            }

            if (query != NULL) {
                free(query);
            }
        }
    }
}

// FUNCTIONS TO HANDLE QUERY ---------------------------------------------------------------------
/**************** validateQuery ****************/
/* validate to make sure query contains valid characters */ 
static bool 
validateQuery(char *query) {
    // loop over all characters in query until an invalid character is found
    for (char *character = query; *character != '\0'; character++) {
        if (!(isalpha(*character) || isspace(*character))) {
            printf("\nError: bad character in query.\n");
            return false;
        }
    }
    return true;
}

/**************** tokenizeQuery ****************/
/* create an array to store the words */ 
static char **tokenizeQuery(char *query, int size)
{
    // create a new array to store words in the query
    char **words = calloc(size, sizeof(char *));

    int wordCount = 0;

    // variable to track if currently inside a word
    bool inWord = false;

    // allocate memory for a copy of the query string
    char *copy = malloc(strlen(query) + 1);
    memcpy(copy, query, strlen(query) + 1);

    char *wordStart = NULL;
    char *currentChar = copy;

    while (*currentChar != '\0') {
        if (isspace(*currentChar)) {
            if (inWord) {
                // if whitespace encountered after a word, tokenize and normalize it
                *currentChar = '\0';
                normalizeWord(wordStart); 

                // allocate memory for the word and copy it to the words array
                words[wordCount] = malloc(strlen(wordStart) + 1);
                strcpy(words[wordCount], wordStart);
                wordCount++;
                inWord = false;
            }
        } 
        else if (!inWord) {
            // if non-whitespace character encountered and not inside a word, mark start of new word
            wordStart = currentChar;
            inWord = true;
        }
        currentChar++;
    }

    // process the last word
    if (inWord) {
        normalizeWord(wordStart);
        words[wordCount] = malloc(strlen(wordStart) + 1);
        strcpy(words[wordCount], wordStart);
        wordCount++;
    }
    free(copy);
    return words;
}

/**************** checkOperators ****************/
/* check if operators are where they are not supposed to be */ 
static bool
checkOperators(char *words[], int wordCount)
{
    char* first_word = words[0];
    char* last_word = words[wordCount - 1];

    // check if query begins with 'and' or 'or'
    if (strcmp(first_word, "and") == 0 || strcmp(first_word, "or") == 0) {
        printf("Error: 'and' or 'or' cannot be first.\n");
        return false;
    }

    // check if query ends with 'and' or 'or'
    if (strcmp(last_word, "and") == 0 || strcmp(last_word, "or") == 0) {
        printf("Error: 'and' and 'or' cannot be last.\n");
        return false;
    }

    if (wordCount > 1) {
        // loop through each word to identify adjacent 'and' and 'or'
        for (int i = 0; i < wordCount - 1; i++)
        {
            // get current word and next word
            char* current_word = words[i];
            char* next_word = words[i+1];

            // check if an 'and' is followed by an 'and' or an 'or'
            if ((strcmp(current_word, "and")) == 0) {
                if ((strcmp(next_word, "and") == 0)) {
                    printf("Error: 'and' and 'and' can't be adjacent.\n");
                    return false;
                }
                if ((strcmp(next_word, "or") == 0)) {
                    printf("Error: 'and' and 'or' can't be adjacent.\n");
                    return false;
                }
            }

            // check if an 'or' is followed by an 'and' or an 'or'
            if ((strcmp(current_word, "or")) == 0) {
                if ((strcmp(next_word, "or") == 0)) {
                    printf("Error: 'or' and 'or' can't be adjacent.\n");
                    return false;
                }
                if ((strcmp(next_word, "and") == 0)) {
                    printf("Error: 'or' and 'and' can't be adjacent.\n");
                    return false;
                }
            }
        }
    }
    return true;
}

/**************** processOperators ****************/
/* function to create union/intersection sets according to operator given in the query */ 
static 
counters_t *processOperators(char *word[], index_t *index, int word_count)
{
    // create counters for the 'and' and 'or' operators
    counters_t *andSet = NULL; 
    counters_t *orSet = NULL;

    // 
    bool skipWord = false;

    for (int i = 0; i < word_count; i ++)
    {
        char* currWord = word[i];

        // if current word is 'and', continue to next word
        if (strcmp(currWord, "and") == 0) continue; // continue to next word

        // if current word is 'or', merge 'and' and 'or' sets
        if (strcmp(currWord, "or") == 0)
        {
            mergeSet(&andSet, &orSet);
            skipWord = false;
            continue;
        }

        if (skipWord == true) {
            continue;
        }

        // if a word that is not 'and' or 'or' is found, find that word in the index
        counters_t *result = hashtable_find(index, currWord);

        // if this word is not found anywhere
        if (result == NULL) {
            skipWord = true;
            if (andSet != NULL) {
                counters_delete(andSet);
                andSet = NULL;
            }
        }

        else {
            // found counters for this word in index
            if (andSet == NULL)
            {
                // this is the first word of andSet
                andSet = counters_new();
                mergeUnion(&andSet, &result); // copies match to andSequence
            }
            else {
                mergeIntersection(&andSet, result);
            }
        }
    }
    mergeSet(&andSet, &orSet);
    return orSet;
}

/**************** countWords ****************/
/* count the number of words in the query. used to determine size of array */ 
static int 
countWords(char *query)
{
    int count = 0;
    // skip leading white spaces
    while (isspace(*query)) {
        query++;
    }

    // count number of words
    while (*query != '\0') {
        if (isalpha(*query)) {
            count++;

            // skip the rest of the characters in the current word
            while (isalpha(*query)) {
                query++;
            }
        }
        else {
            // skip characters
            query++;
        }

        // skip trailing white spaces
        while (isspace(*query)) {
            query++;
        }
    }
    return count;
}

// FUNCTIONS TO HANDLE "OR" OPERATIONS -------------------------------------------------------------

/**************** mergeUnion ****************/
/* iterate through a set of counters and merge it with another */ 
static
void mergeUnion(counters_t **thisSet, counters_t **otherSet)
{
    counters_iterate(*otherSet, *thisSet, mergeUnionHelper);
}

/**************** mergeUnionHelper ****************/
/* helper function to add up the count of 2 counters */ 
static
void mergeUnionHelper(void *arg, const int key, int count)
{
    counters_t *counterA = arg;
    int word_count = counters_get(counterA, key);
    counters_set(counterA, key, count + word_count);
}

// FUNCTIONS TO HANDLE "AND" OPERATIONS --------------------------------------------------------------

/**************** mergeIntersection ****************/
/* iterate through a set of counters and merge it with another */ 
static void mergeIntersection(counters_t **thisSet, counters_t *otherSet)
{
    counters_t **array = malloc(sizeof(counters_t *) * 2);

    // assign this and other set, iterate through the first set of counters and merge the other to it.
    array[0] = *thisSet;
    array[1] = otherSet;
    counters_iterate(array[0], array, mergeIntersectionHelper);
    free(array);
}

/**************** mergeIntersectionHelper ****************/
/* helper function for mergeIntersection */ 
static void mergeIntersectionHelper(void *arg, const int key, int count)
{
    counters_t **array = arg;

    // array[0]: this counter, array[1]: other counter
    // get count of other counter and compare with that of other
    int counter_count = counters_get(array[1], key);
    int smaller_count = count > counter_count ? counter_count : count;

    // update this counter with the smaller_count
    counters_set(array[0], key, smaller_count);
}

/**************** mergeSet ****************/
/* function to merge an "and" and an "or sequence together" */ 
static void 
mergeSet(counters_t **andSet, counters_t **orSet)
{
    if (*orSet == NULL) {
        *orSet = counters_new();
    }
    if (*andSet != NULL)
    {
        // merge the andSequence into the orSequence
        mergeUnion(orSet, andSet); 
        counters_delete(*andSet);
        *andSet = NULL;
    }
}

// FUNCTIONS TO HANDLE RANKINGS-------------------------------------------------------------
/**************** printByRank ****************/
/* print document information by ranks */ 
static void printByRank(counters_t *score, char *pageDirectory)
{
    document_t topDoc = {0, 0};
    int count = 0;  // counter for the number of documents processed

    while (topDoc.score >= 0)
    {
        topDoc.score = 0; // reset best score for each iteration

        // find the document with the highest score and store in top doc
        findTopDocument(score, &topDoc);

        if (topDoc.score == 0) {
            // print and break out if no document is found
            if (count == 0) {
                printf("No documents found.\n");
                break;
            }
            // break out if the current highest score is 0, meaning that all docs have been ranked
            else break;
        }

        //set the counter value of the top document to be 0 to avoid it being processed again
        counters_set(score, topDoc.id, 0);

        // obtain the url of the top document
        char *url = pagedir_getLink(pageDirectory, topDoc.id);
        printf("score %2d doc %2d: %s \n", topDoc.score, topDoc.id, url);
        free(url);
        count++;
    }
}

/**************** findTopDocument ****************/
/* function to find the document with the highest score */ 
static void findTopDocument(counters_t *score, document_t *topDoc)
{
    counters_iterate(score, topDoc, docComparator);
}

/**************** docComparator ****************/
/* helper function for fincTopDocument */ 
static void docComparator(void *arg, int key, int count)
{
    document_t *topDoc = arg;
    // update topDoc if current count is higher than previous best score
    if (count > topDoc->score)
    {
        topDoc->id = key;
        topDoc->score = count;
    }
}