/* 
 * set.c implements the set data structure
 *
 * A *set* maintains an unordered collection of (key,item) pairs;
 * any given key can only occur in the set once. It starts out empty 
 * and grows as the caller inserts new (key,item) pairs. The caller 
 * can retrieve items by asking for their key, but cannot remove or 
 * update pairs. Items are distinguished by their key.
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 16, 2023
 */

#ifndef __SET_H
#define __SET_H

#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include "mem.h"
#include <string.h>
#include "set.h"

/**************** global types ****************/
// opaque to users of the module
// the nodes in the set
typedef struct setnode {
  void* item;
  char *key;             
  struct setnode *next;
} setnode_t;

// the set as a whole with a head
typedef struct set {
    setnode_t *head;
} set_t;

/**************** functions ****************/
// helper function to set a new node
static setnode_t* setnode_new(const char* key, void* item);
// helper function to determine if a key already exists in the set
static bool has_key(set_t* set, const char* key);

/**************** set_new ****************/
/* Create a new (empty) set. */
set_t*
set_new(void) {
    set_t* set = mem_malloc(sizeof(set_t));

    if (set != NULL) {
        set->head = NULL;
        return set;
    }
    else {
        return NULL;
    }
}

/**************** set_insert ****************/
/* Insert item, identified by a key (string), into the given set. */
bool 
set_insert(set_t* set, const char* key, void* item) {
    // check the validity of arguments
    if (set == NULL || key == NULL || item == NULL) {
        return false;
    }
    
    if (has_key(set, key)) {
        return false;
    }

    setnode_t* new_node = setnode_new(key, item);
    // check if key already exists
    // set new node
    if (new_node != NULL) {
        new_node->next = set->head; 
        set->head = new_node;
        return true;
    }
    else {
        return false;
    }
}

/**************** set_find ****************/
/* Return the item associated with the given key. */
void* 
set_find(set_t* set, const char* key) {
    // check for valid inputs and non-empty set
    if (set == NULL || key == NULL || set->head == NULL) {
        return NULL;
    }

    // loop through set to find the item
    for (setnode_t* node = set->head; node != NULL; node = node->next) {
        const char* current_key = node->key;
        if (strcmp(current_key, key) == 0) {
            return node->item; 
        }
    }
    return NULL; // return null if key is not in the set
}

/**************** set_print ****************/
/* Print the whole set; provide the output file and func to print each item. */
void 
set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) {
    if (fp == NULL) {
        return;
    }
    
    if (set == NULL) {
        fprintf(fp, "(null)");
    }
    else if (itemprint == NULL) {
        fprintf(fp, "{}");
    }
    else {
        fprintf(fp, "{");
        for (setnode_t* node = set->head; node != NULL; node = node->next) {
           (*itemprint)(fp, node->key, node->item);
           if (node->next != NULL) fprintf(fp, ", ");
        }
        fprintf(fp, "}");
    }
}

/**************** set_iterate ****************/
/* Iterate over the set, calling a function on each item. */
void
set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)) {
    if (set != NULL && itemfunc != NULL) {
        for (setnode_t* node = set->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->item);
        }
    }
}

/**************** set_delete ****************/
/* Delete set, calling a delete function on each item. */
void 
set_delete(set_t* set, void (*itemdelete)(void* item))
{
    if (set != NULL){
        for (setnode_t* node = set -> head; node != NULL; ){
            if (itemdelete != NULL) {
                (*itemdelete)(node -> item);
            }
            setnode_t* next = node->next;
            mem_free(node->key);
            mem_free(node); // free memory for each node
            node = next;
        }
        mem_free(set); // free memory for the entire set
    }
}

/**************** setnode_new ****************/
/* Helper function to create a new node. */
static setnode_t*
setnode_new(const char* const key, void* item) {
    // allocate memory to the new node
    setnode_t* node = mem_malloc(sizeof(setnode_t));

    if (node == NULL) {
        return NULL;
    }
    else {
        char* key_copy = mem_malloc(strlen(key) + 1);
        if (key_copy == NULL) {
            mem_free(node);
            return NULL;
        }
        strcpy(key_copy, key);
        node->item = item;
        node->key = key_copy;
        node->next = NULL; 
        return node;
    }
}

/**************** has_key ****************/
/* Helper function to determine if a key already exists in the set.
 * Returns true of the key exists, else returns false.
 */
static bool
has_key(set_t* set, const char* key) {
    for (setnode_t* node = set->head; node != NULL; node = node->next) { // loop through all the nodes
        const char* current_key = node->key; // get current key
        if (strcmp(current_key, key) == 0) { //
            return true;
        }
    }
    return false;
}
#endif