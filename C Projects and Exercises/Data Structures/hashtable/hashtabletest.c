/* 
 * hashtableset.c - test program for CS50 hashtable module
 *
 * usage: read names from stdin and output from functions
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 16, 2023
 *
 * Template from bagtest.c - CS50, April 2019, 2021
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

static void itemprint(FILE* fp, const char *key, void *item);
static void itemdelete(void* item);
static void itemcount(void* arg, const char* key, void* item);

/* **************************************** */
int
main() 
{
    // create new table
    hashtable_t *ht = hashtable_new(5);
    if (ht == NULL) {
        fprintf(stderr, "Unable to make table.\n");
        return 1;
    }
    else {
        printf("Table with 5 slots successfully created.\n");
    }

    // create some key - name pairs
    char *key1 = "CN";
    char *name1 = (char*) malloc((strlen("Chip") + 1) * sizeof(char));
    strcpy(name1, "Chip");

    char *key2 = "CP";
    char *name2 = (char*) malloc((strlen("Charles") + 1) * sizeof(char));
    strcpy(name2, "Charles");

    char *key3 = "AN";
    char *name3 = (char*) malloc((strlen("Anna") + 1) * sizeof(char));
    strcpy(name3, "Anna");

    // test hashtable_insert
    printf("\nAdd 3 valid key-value pairs and some invalid ones. A duplicate key-item pair is also added\n");
    hashtable_insert(ht, key1, name1);
    hashtable_insert(ht, key1, name1);
    hashtable_insert(ht, key2, name2);
    hashtable_insert(ht, key3, name3);
    hashtable_insert(ht, NULL, name1);
    hashtable_insert(NULL, key1, name1);
    printf("Hash table after adding (Should have 3 non-empty sets, duplicate pair not added): \n");

    // test hashtable_print
    hashtable_print(ht, stdout, itemprint);
    printf("\n");

    // test hashtable_iterate
    int tablecount = 0;
    hashtable_iterate(ht, &tablecount, itemcount);
    printf("Test hashtable_iterate: should give count of table = 3: %d\n", tablecount);

    // test hashtable_find
    printf("Result from a valid find of key = AN: %s\n", (char*) hashtable_find(ht, "AN"));
    printf("Result from an invalid find of key = PQ, which doesn't exist: %s\n", (char*) hashtable_find(ht, "PQ"));
    printf("Result from an invalid find from a NULL hashtable: %s\n", (char*) hashtable_find(NULL, "AN"));

    //test hashtable_delete
    hashtable_delete(ht, itemdelete);
    printf("\nTable deleted.\n");
}

static void 
itemprint(FILE* fp, const char *key, void *item)
{
    const char* key_name = key;
    char* item_name = item;
    if (key_name == NULL || item_name == NULL) {
        fprintf(fp, "(null)");
    }
    else {
    // print (key, value)
    fprintf(fp, "(%s: %s)", key_name, item_name);
    }
}

static void 
itemdelete(void* item) {
    if (item != NULL) {
        free(item);
    }
}

static void
itemcount(void* arg, const char* key, void* item)
{
    int* nitems = arg;
    if (nitems != NULL && key != NULL && item != NULL) {
        (*nitems)++;
    }
}