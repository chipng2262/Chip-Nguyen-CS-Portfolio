/* 
 * countertest.c - test program for CS50 counter module
 *
 * usage: read names from stdin and output from functions
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 17 2023
 *
 * Template from bagtest.c - CS50, April 2019, 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "file.h"

// function protype
static void itemcount(void* arg, const int key, const int count);

/* **************************************** */
int main() 
{
  // create new counter
  printf("Create new counter with counters_new.\n");
  counters_t* counter = counters_new();
  if (counter == NULL) {
    fprintf(stderr, "Failed to create new counter.");
    return 1;
  }
  else {
    printf("Counter created successfully.\n");
  }

  // test counter_add and counters_print
  printf("\nAdd an invalid (NULL) counter.\n");
  counters_add(NULL, 2);
  printf("Add a valid counter but negative key.\n");
  counters_add(counter, -10);
  printf("Add two valid counter with valid keys = 2 and 10.\n");
  counters_add(counter, 2);
  counters_add(counter, 10);
  printf("Current counter should be {10=1, 2=1}: ");
  counters_print(counter,stdout);
  printf("\n\nAdd a counter with valid key = 2 again.\n");
  counters_add(counter, 2);
  printf("Current counter should be {10=1, 2=2}: ");
  counters_print(counter,stdout);
  printf("\n");

  // test counter_set
  printf("\nSet counter with a negative key.\n");
  counters_set(counter, -2, 6);
  printf("Set counter with a negative count.\n");
  counters_set(counter, 2, -6);
  printf("Set a non-existing key = 8 to 9.\n");
  counters_set(counter, 8, 9);
  printf("Set the existing key = 10 to 11.\n");
  counters_set(counter, 10, 11);
  printf("Current counter should be {8=9, 10=11, 2=2}: ");
  counters_print(counter,stdout);
  printf("\n");

  // test counter_get 
  printf("\nCall counter_get on a non-existent key (should return zero): ");
  printf("%d\n", counters_get(counter, 13));
  printf("Call using the existing key 8 (should return 9): ");
  printf("%d\n", counters_get(counter, 8));

  // test counter_iterate
  int number_elements = 0;
  printf("\nNumber of elements in set counter (should be 3): ");
  counters_iterate(counter, &number_elements, itemcount);
  printf("%d\n", number_elements);

  // test counter_delete
  counters_delete(counter);
  printf("\nCounter deleted.\n");
}

/* itemcount counts the non-null items in the set. */
void 
itemcount(void* arg, const int key, const int count)
{
    int* nitems = arg;
    if (nitems != NULL) {
        (*nitems)++;
    }
}