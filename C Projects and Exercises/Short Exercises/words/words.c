/*
 * words.c - Program that breaks its input into a series of words, one per line. 
 * It may take input from stdin, or from files whose names are listed as arguments.
 * 
 * Chip Nguyen, COSC50 Spring 2023
 * April 11, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// function prototypes
int strcmp(const char* part, const char* string); // string comparator
void parseFile(FILE *file); // function to parse texts in a file

/* ***************************
 * main function to handle inputs and break them into series of words  
 */
int
main(const int argc, char *argv[])
{
  FILE *file;

  // if no file name is given, take inputs from stdin
  if (argc == 1) {
    file = stdin;
    parseFile(file);
  }

  // if there are 1 or more file names given 
  else {
    for (int i = 1; i < argc; i++) { // loop through all file names given
      if (strcmp("-", argv[i]) != 0) { // hand file names that are not "-"
        file = fopen(argv[i], "r");
        if (file == NULL) { // check if file exists, is readable, and is not empty
          fprintf(stderr, "File %s does not exist or is unreadable. \n", argv[i]);
          exit(1);
        }
      }
      else {
        file = stdin;
      }
      parseFile(file);
      fclose(file);
    }
  }
  return 0;
}

/* ***************************                                                                     
 * parseFile function to parse the texts
 */
void
parseFile(FILE *file) {
  char word; // variable storing each word in the file
  bool newLine = false; // boolean variable to determine whether to insert a new line
  while ((word = fgetc(file)) != EOF) {

    // handle non-letter characters
    if (!isalpha(word) && newLine) {
      printf("\n");
      newLine = false; // set newLine to false to prevent printing double line breaks
    }

    // handle letter characters
    else if (isalpha(word)){
      printf("%c", word);
      newLine = true;
    }
  }
}