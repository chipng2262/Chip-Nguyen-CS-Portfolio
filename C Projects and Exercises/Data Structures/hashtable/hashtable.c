/* 
 * hashtable.c implements the hash table data structure.
 *
 * A *hashtable* is a set of (key,item) pairs.  It acts just like a set, 
 * but is far more efficient for large collections.
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 18, 2023
 */

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "hash.h"
#include "../set/set.h"

/**************** global types ****************/
// 
typedef struct hashtable {
    set_t ** table;
    long count;
} hashtable_t;

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable. */
hashtable_t*
hashtable_new(const int num_slots) {
    hashtable_t* new_table = malloc(sizeof(hashtable_t));
    if (new_table != NULL) {
        new_table->table = calloc(num_slots, sizeof(set_t*));
        new_table->count = num_slots;

        for (int i = 0; i < num_slots; i++) {
            new_table->table[i] = set_new(); // create a new set for each slot
            if (new_table->table[i] == NULL) { //deallocate memory if failed to make set
                for (int j = 0; j < i; j++) {
                    set_delete(new_table->table[j], NULL);
                }
                free(new_table->table);
                free(new_table);
                return NULL;
            }
        }
        return new_table;
    }
    else return NULL;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable. */
bool 
hashtable_insert(hashtable_t* ht, const char* key, void* item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }
    char* key_copy = malloc((strlen(key) + 1) * sizeof(char));
    if (key_copy == NULL) {
        return false;
    }
    strcpy(key_copy, key);
    int hash_index = hash_jenkins(key_copy, ht->count);
    bool is_inserted = set_insert(ht->table[hash_index], key_copy, item);
    free(key_copy);
    return is_inserted; // return true if insertion is successful
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key. */
void* 
hashtable_find(hashtable_t* ht, const char* key) {
    if (ht == NULL || key == NULL) {
        return NULL;
    }
    int hash_index = hash_jenkins(key, ht->count);
    return set_find(ht->table[hash_index], key); // if key is not found, set_find returns NULL
}

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item. */
void
hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) {
    if (fp == NULL) {
        return;
    }
    
    // print "(null)" if null table
    if (ht == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    // loop through all sets and print
    for (int i = 0; i < ht->count; i++) {
        set_print(ht->table[i], fp, (*itemprint));
        fprintf(fp, "\n");
    }
}

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order. */
void 
hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;
    }
    for (int i = 0; i < ht->count; i++) {
        set_iterate(ht->table[i], arg, itemfunc);
    }
}

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item. */
void
hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item)) {
    if (ht == NULL) {
        return;
    }
    if (itemdelete != NULL) {
        for (int i = 0; i < ht->count; i++) {
            set_delete(ht->table[i], itemdelete);
        }
        free(ht->table);
        free(ht);
    }
}

#endif // __HASHTABLE_H