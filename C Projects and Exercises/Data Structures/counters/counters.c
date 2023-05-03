/* 
 * counters.c implements the counter data structure.
 *
 * A "counter set" is a set of counters, each distinguished by an integer key.
 * It's a set - each key can only occur once in the set - but instead of
 * storing (key,item) pairs, it tracks a counter for each key.  It starts
 * empty. Each time `counters_add` is called on a given key, that key's
 * counter is incremented. The current counter value can be retrieved by
 * asking for the relevant key.
 * 
 * Chip Nguyen, COSC 50 Spring 2023
 * April 18, 2023
 */

#ifndef __COUNTERS_H
#define __COUNTERS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "counters.h"

// a node in the counter
typedef struct countersnode{
    int key;
    int count;
    struct countersnode *next;  //link to next node
} countersnode_t;

/**************** global types ****************/
typedef struct counters {
    struct countersnode *head;
} counters_t;

/**************** local functions ****************/
static countersnode_t *countersnode_new(int key);

/**************** FUNCTION ****************/
/* Create a new (empty) counter structure. */
counters_t* 
counters_new(void) {
    counters_t* counter = malloc(sizeof(counters_t));
    if (counter == NULL) {
        return NULL;
    }
    else {
        counter->head = NULL;
        return counter;
    }
}

/**************** counters_add ****************/
/* Increment the counter indicated by key. */
int
counters_add(counters_t* ctrs, const int key) {
    if (ctrs == NULL || key < 0) {
        return 0;
    }
    // loop through all nodes to check if they input key already exists
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
        if (node->key == key) {
            node->count ++;
            return node->count;
        }
    }

    countersnode_t* new_node = countersnode_new(key);
    if (new_node == NULL) {
        return 0;
    }
    else {
        new_node->next = ctrs->head;
        ctrs->head = new_node;
        return new_node->count;
    }
}

/**************** counters_get ****************/
/* Return current value of counter associated with the given key. */
int
counters_get(counters_t* ctrs, const int key)
{
    if (ctrs == NULL || key < 0) {
        return 0;
    }
    else {
        for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
            if (node->key == key) {
                return node->count;
            }
        }
        return 0;
    }
}

/**************** counters_set ****************/
/* Set the current value of counter associated with the given key. */
bool
counters_set(counters_t* ctrs, const int key, const int count) {
    if (ctrs == NULL || key < 0 || count < 0) {
        return false;
    }

    // loop through all nodes to check if they input key already exists
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
        if (node->key == key) {
            node->count = count;
            return true;
        }
    }
    countersnode_t* new_node = countersnode_new(key);
    if (new_node == NULL) return false; // out of memory for new node
    else {
        new_node->count = count;
        new_node->next = ctrs->head;
        ctrs->head = new_node;
        return true;
    }
}

/**************** counters_print ****************/
/* Print all counters; provide the output file. */
void counters_print(counters_t* ctrs, FILE* fp) {
    if (fp == NULL) return;
    if (ctrs == NULL) {
        fprintf(fp, "(null)");
        return;
    }
    
    fprintf(fp, "{");
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
        fprintf(fp, "%d=%d", node->key, node->count);
        if (node->next != NULL) fprintf(fp, ", ");
    }
    fprintf(fp, "}");
}

/**************** counters_iterate ****************/
/* Iterate over all counters in the set. */
void
counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count)) {
    if (ctrs == NULL || itemfunc == NULL) return;
    for (countersnode_t* node = ctrs->head; node != NULL; node = node->next) {
        (*itemfunc)(arg, node->key, node ->count);
    }
}

/**************** counters_delete ****************/
/* Delete the whole counterset. */
void counters_delete(counters_t* ctrs) {
    if (ctrs == NULL) return;
    for (countersnode_t* node = ctrs->head; node != NULL;) {
        countersnode_t* next = node->next;
        free(node);
        node = next;
    }
    free(ctrs);
}

/**************** countersnode_new ****************/
/* Create a new node. */
static countersnode_t *countersnode_new(int key) {
    countersnode_t *node = malloc(sizeof(countersnode_t));
    if (node != NULL) {
        node->key = key;
        node->count = 1;
        node->next = NULL;
        return node;
    }
    else return NULL;
}
#endif