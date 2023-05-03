/* 
 * testtest.c - test program for CS50 set module
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
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "set.c"
#include "file.h"

static void itemprint(FILE* fp, const char *key, void *item);
static void itemdelete(void* item);
static void itemcount(void* arg, const char* key, void* item);

/* **************************************** */
int main() 
{
    set_t* set; 
    int namecount = 0; // number of elements put in set
    int setcount = 0; // number of elements found in set

    // test set_new
    printf("Create set with set_new.\n");
    set = set_new();
    if (set == NULL) {
        fprintf(stderr, "Failed to create new set");
        return 1;
    }
    else {
        printf("Set 1 created successfully.\n");
    }

    // test set_insert with invalid cases
    printf("\nTest insert with null set, good key and item: ");
    set_insert(NULL, "CN", "Chip Nguyen");
    set_print(set, stdout, itemprint);
    printf("\nTest insert with valid set, null key and good item: ");
    set_insert(set, NULL, "Chip Nguyen"); 
    set_print(set, stdout, itemprint);
    printf("\nTest insert with null set, null key, and null item: ");
    set_insert(NULL, NULL, NULL);
    set_print(set, stdout, itemprint);

    // test set_iterate
    printf("\nNumber of elements in set 1 (should be zero): ");
    setcount = 0;
    set_iterate(set, &setcount, itemcount);
    printf("%d\n", setcount);

    // test set_insert with valid cases
    printf("\nAdd 4 elements to the set.");
    namecount = 0;
    while (!feof(stdin)) {
      char* key = file_readLine(stdin);
      char* name = file_readLine(stdin);
      if(!set_insert(set, key, name)) {
        free(name);
      }
      free(key);
      namecount++;
    }

    printf("\nCount after adding (should be %d): ", namecount);
    setcount = 0;
    set_iterate(set, &setcount, itemcount);
    printf("%d\n", setcount);

    // test set_print
    printf("\nTest set print. The set is:\n");
    set_print(set, stdout, itemprint);
    printf("\n");

    // add a key that already exists
    printf("\nAdd a key that already exists. Set shouldn't change: \n");
    set_insert(set, "P", "Pink");
    set_print(set, stdout, itemprint);

    printf("\n\nTest set print for a null set:");
    set_print(NULL, stdout, itemprint);
    printf("\n");

    // test set_find
    printf("\nTest set_find with a key that doesn't exist: ");
    printf("Output of find: %s", (char*) set_find(set, "Chip"));
    printf("\nTest set_find with a null set: ");
    printf("Output of find: %s", (char*) set_find(NULL, "Peter"));
    printf("\nTest set find with a key Peter that exists: ");
    printf("Output of find: %s", (char*) set_find(set, "Peter"));
    printf("\n");

    // test set_delete
    printf("\nDelete the set.\n");
    set_delete(set, itemdelete);
    set = NULL;

    return 0;
}

/* itemprint prints an item */
void itemprint(FILE* fp, const char* key, void* item)
{
  const char* key_name = key;
  char* item_name = item;
  if (key_name == NULL || item_name == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    // print (key, value)
    fprintf(fp, "(%s, %s)", key_name, item_name);
  }
}

/* itemdelete */
void itemdelete(void* item)
{
  if (item != NULL) {
    free(item);
  }
}

/* itemcount counts the non-null items in the set. */
static void 
itemcount(void* arg, const char* key, void* item)
{
    int* nitems = arg;
    if (nitems != NULL && key != NULL && item != NULL) {
        (*nitems)++;
    }
}