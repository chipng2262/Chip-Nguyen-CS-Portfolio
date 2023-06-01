/* 
 * indexer.c implements an indexer for the tiny search engine. 
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * May 2, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"

#define MAX_LENGTH 500

/**************** functions ****************/
int indexBuild(char* pageDirectory, hashtable_t* ht);
void indexPage(index_t* index, webpage_t* page, int docID);

int main(int argc, char* argv[]) {
    // check number of arguments
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments!\n");
        exit(1);
    }

    // check valid page directory (produced by crawler)
    char* pageDirectory = argv[1];

    FILE *file_1 = fopen(pageDirectory, "r");
    if (!file_1) {
        fprintf(stderr, "Invalid/Non-existent page directory.\n");
        exit(1);
    }
    fclose(file_1);

    if (!(pagedir_validate(pageDirectory))) {
        fprintf(stderr, "Not a valid crawler-produced directory!\n");
        exit(1);
    }

    index_t* index = hashtable_new(500);
    if (index == NULL) {
        fprintf(stderr, "Unable to create new index\n");
        exit(1);
    }

    if (indexBuild(pageDirectory, index) != 0) {
        fprintf(stderr, "indexBuild failed.\n");
        index_delete(index);
        exit(1);
    }

    char* targetDirectory = argv[2];
    // create a new file to write index
    FILE *file_2 = fopen(targetDirectory, "w");

    if (!file_2) {
        fprintf(stderr, "Failed to create/open target directory.\n");
        exit(1);
    }

    fclose(file_2);

    // write the index to the file
    if (index_save(index, targetDirectory) != 0) {
        fprintf(stderr, "Failed to write index to target directory.\n");
        exit(1);
    }
    index_delete(index);
    return 0;
}

int
indexBuild(char* pageDirectory, hashtable_t* ht) {
    int id = 1;
    char* path = malloc((strlen(pageDirectory)) + 5);
    sprintf(path, "%s/%d", pageDirectory, id);

    FILE* fp;

    // open file
    while ((fp = fopen(path, "r")) != NULL) {
        // use page load to load a new page
        webpage_t* webpage = pagedir_load(path, id);

        if (webpage != NULL) {
            // index this page
            indexPage(ht, webpage, id);
            webpage_delete(webpage);
        }

        // increment id, and construct address to next document
        id++;
        sprintf(path, "%s/%d", pageDirectory, id);
        fclose(fp);
    }
    free(path);
    return 0;
}

void
indexPage(index_t* index, webpage_t* page, int docID) {
    char* result;
    int pos = 0;
    
    while ((result = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(result) >= 3) {
        normalizeWord(result);
        index_insert(index, result, docID);
        }
        free(result);
    }
}