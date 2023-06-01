/* 
 * indextest implements an indexer for the tiny search engine. 
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * May 2, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/index.h"
#include "../common/pagedir.h"

int main(int argc, char* argv[])
{
    // check for correct number of argument
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments");
        exit(1);
    }

    // open oldIndexFilename to load file content to inverted index
    char* oldIndexFilename = argv[1];
    FILE* file_1 = fopen(oldIndexFilename, "r");

    if (file_1 == NULL) {
        fprintf(stderr, "Invalid oldIndexFilename.\n");
        exit(1);
    }
    
    hashtable_t* ht = hashtable_new(file_numLines(file_1) * 10);
    if (ht == NULL) {
        fprintf(stderr, "Unable to make hashtable.\n");
        fclose(file_1);
        exit(1);
    }

    // create an inverted index
    if ((createInvertedIndex(ht, file_1)) != 0) {
        fprintf(stderr, "Failed to create inverted index.\n");
        fclose(file_1);
        exit(1);
    }

    // open newIndexFilename to write inverted index data 
    char* newIndexFilename = argv[2];
    FILE* file_2 = fopen(newIndexFilename, "w");

    if (file_2 == NULL) {
        fprintf(stderr, "Invalid newIndexFilename.");
        exit(1);
    }

    // write from hashtable to new file
    if ((index_save(ht, newIndexFilename)) != 0) {
        fclose(file_2);
        index_delete(ht);
        fprintf(stderr, "Writing to newIndexFilename failed.");
        exit(1);
    }
    fclose(file_2);
    index_delete(ht);
    return 0;
}