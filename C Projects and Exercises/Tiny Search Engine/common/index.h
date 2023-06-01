/* 
 * index.h - header file for index.c
 * 
 * Chip Nguyen, CS 50 Spring 2023
 * May 3, 2023
 */

#ifndef __INDEX_H__
#define __INDEX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "pagedir.h"
#include "word.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** index_insert ****************/
/** Insert an index into the hash table
 * 
 * Caller provides a valid hashtable, a pointer to a word, and the document's id
 * Function returns:
 *  0 if index is sucessfully inserted to the table and all inputs are valid
 *  else exit non-zero
*/
int index_insert(hashtable_t* ht, const char* result, int id);

/**************** index_save ****************/
/** Write indexes in the table to a file 
 * 
 * Call provides a valid hashtable and a targets directory to write elements of hashtable to
 * Function returns:
 *  0 if writing is successful and all inputs are valid
 *  else exit non-zero
*/
int index_save(hashtable_t* ht, char* targetDirectory);

/**************** index_delete ****************/
/** Delete an item in a hashtable
 * Caller provides a valid hashtable.
*/
void index_delete(hashtable_t* ht);

/**************** createInvertedIndex ****************/
/** Create an inverted index using content of an index output file
 * 
 * Caller provides a valid hashtable/index and a file to read from.
 * Function returns:
 *  0 if inverted index is successfully created and all inputs are valid
 *  exit non-zero otherwise
*/
int createInvertedIndex(hashtable_t* ht, FILE*fp);
#endif