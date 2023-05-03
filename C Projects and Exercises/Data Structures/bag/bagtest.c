/* 
 * bagtest.c - test program for CS50 bag module
 *
 * usage: read names from stdin, each on one line
 *
 * This program is a "unit test" for the bag module.
 * It does a decent job of testing the module, but is
 * not a complete test; it should test more corner cases.
 *
 * CS50, April 2019, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"
#include "file.h"

static void nameprint(FILE* fp, void* item);
static void namedelete(void* item);
static void itemcount(void* arg, void* item);

/* **************************************** */
int main() 
{
  bag_t* bag1 = NULL;           // one bag
  bag_t* bag2 = NULL;           // another bag
  char* name = NULL;            // a name in the bag
  int namecount = 0;            // number of names put in the bag
  int bagcount = 0;             // number of names found in a bag

  printf("Create first bag...\n");
  bag1 = bag_new();
  if (bag1 == NULL) {
    fprintf(stderr, "bag_new failed for bag1\n");
    return 1;
  }

  printf("\nTest with null bag, good item...\n");
  bag_insert(NULL, "Dartmouth");
  printf("test with null item...\n");
  bag_insert(bag1, NULL); 
  printf("test with null bag, null item...\n");
  bag_insert(NULL, NULL);

  printf("\nCount (should be zero): ");
  bagcount = 0;
  bag_iterate(bag1, &bagcount, itemcount);
  printf("%d\n", bagcount);

  printf("\nTesting bag_insert...\n");
  // read lines from stdin
  namecount = 0;
  while (!feof(stdin)) {
    name = file_readLine(stdin);
    if (name != NULL) {
      bag_insert(bag1, name);
      namecount++;
    }
  }

  printf("\nCount (should be %d): ", namecount);
  bagcount = 0;
  bag_iterate(bag1, &bagcount, itemcount);
  printf("%d\n", bagcount);

  printf("\nThe bag:\n");
  bag_print(bag1, stdout, nameprint);
  printf("\n");

  printf("\nMove items to a new bag...\n");
  bag2 = bag_new();
  if (bag2 == NULL) {
    fprintf(stderr, "bag_new failed for bag2\n");
    return 2;
  }

  while ( (name = bag_extract(bag1)) != NULL) {
    bag_insert(bag2, name);
  }

  printf("\nThe old bag:\n");
  printf("Count (should be zero): ");
  bagcount = 0;
  bag_iterate(bag1, &bagcount, itemcount);
  printf("%d\n", bagcount);  
  bag_print(bag1, stdout, nameprint);
  printf("\n");

  printf("\nThe new bag:\n");
  printf("Count (should be %d): ", namecount);
  bagcount = 0;
  bag_iterate(bag2, &bagcount, itemcount);
  printf("%d\n", bagcount);  
  bag_print(bag2, stdout, nameprint);
  printf("\n");

  printf("\ndelete the bags...\n");
  bag_delete(bag1, namedelete);
  bag_delete(bag2, namedelete);

  return 0;
}


/* count the non-null items in the bag.
 * note here we don't care what kind of item is in bag.
 */
static void itemcount(void* arg, void* item)
{
  int* nitems = arg;

  if (nitems != NULL && item != NULL)
    (*nitems)++;
}

// print a name, in quotes.
void nameprint(FILE* fp, void* item)
{
  char* name = item; 
  if (name == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    fprintf(fp, "\"%s\"", name); 
  }
}

// delete a name 
void namedelete(void* item)
{
  if (item != NULL) {
    free(item);   
  }
}
