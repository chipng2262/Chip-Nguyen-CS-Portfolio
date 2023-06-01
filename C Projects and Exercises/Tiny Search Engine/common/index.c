/* 
 * index.c implements index.h, which contains helper functions for the indexer
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * May 3, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "pagedir.h"
#include "word.h"
#include "index.h"

typedef hashtable_t index_t;

// helper function prototypes
static void item_print(void* arg, const int id, const int count);
static void item_save(void* arg, const char* id, void* word_counter);
static void item_delete(void* item);

/**************** index_insert ****************/
/* Insert a word into the hash table */
int
index_insert(hashtable_t* ht, const char* result, int id) {
    // find word in the table
    counters_t* counter = hashtable_find(ht, result);

    // if a counter for the word does not exist
    if (counter == NULL) {
        counter = counters_new(); // add a new counter
        if (counter == NULL) {
            fprintf(stderr, "Cannot create new counter");
            exit(1);
        }
        if(!hashtable_insert(ht, result, counter)) { // insert new counter to table
            fprintf(stderr, "Cannot add new counter");
            exit(1);
        }
    }
    // else, increment the counter already existing in the table
    if (!counters_add(counter, id)) {
        fprintf(stderr, "Cannot increment counter");
        exit(1);
    }
    return 0;
}

/**************** index_save ****************/
/* Write indexes in the table to a file */
int
index_save(hashtable_t* ht, char* targetDirectory) {
    // check for valid parameters
    if (ht == NULL || targetDirectory == NULL) {
        fprintf(stderr, "Invalid parameters for index_save().");
        exit(1);
    }

    // open taregt file, iterate through the elements in the hashtable and write to file
    FILE* fp = fopen(targetDirectory, "w");
    if (fp != NULL) {
        hashtable_iterate(ht, fp, item_save);
    }
    else {
        fprintf(stderr, "Unable to open file. Make sure file is valid.");
        exit(1);
    }
    fclose(fp);
    return 0;
}

/**************** index_delete ****************/
/* Delete the index */
void
index_delete(hashtable_t* ht) {
    hashtable_delete(ht, item_delete);
}

/**************** createInvertedIndex ****************/
/* Create an inverted index from an indexer output file */
int 
createInvertedIndex(hashtable_t* ht, FILE*fp) {
    if (ht == NULL || fp == NULL) {
        fprintf(stderr, "Invalid parameters for createInvertedIndex().");
        exit(1);
    }

    // read until end of file
    while (!feof(fp)) {
        char* word = file_readWord(fp);
        if (word == NULL) {
            fprintf(stderr, "Invalid content detected in file.");
            exit(1);
        }

        counters_t* counter = counters_new();
        if (counter == NULL) {
            fprintf(stderr, "Unable to create counter.");
            exit(1);
        }

        // get id and count of word
        int id, count;
        while (fscanf(fp, " %d %d", &id, &count) == 2) {
            counters_set(counter, id, count);
        }

        hashtable_insert(ht, word, counter);
        free(word);
    }
    fclose(fp);
    return 0;
}

/**************** item_save ****************/
/* Print a word's document id and frequency count in that document */
static void 
item_save(void* arg, const char* word, void* word_counter) {
    FILE* fp = arg;
    counters_t* counter = word_counter;
    fprintf(fp, "%s", word); // print the word
    counters_iterate(counter, fp, item_print); // iterate through counter and print accoridng to word_print
    fprintf(fp, "\n");
}

/**************** item_print ****************/
/* Print a word's document id and frequency count in that document */
static void 
item_print(void* arg, const int key, int count)
{
    FILE* fp = arg;
    fprintf(fp, " %d %d", key, count);
}

/**************** item_delete ****************/
/* Delete a counter in the hashtable */
static void 
item_delete(void* item) {
    counters_t* counter = item;
    counters_delete(counter);
}