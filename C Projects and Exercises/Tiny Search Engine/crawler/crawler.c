/* 
 * crawler.c implements a crawler for the tiny search engine. 
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 26, 2023
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

// helper function prototypes
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* ***************************
 * main function
 */
int
main(const int argc, char* argv[]) {
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
}

/* ***************************
 * function to parse arguments and ensure their validity
 */
static void 
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments. Make sure there are only 3.\n");
        exit(1);
    }

    // check url
    *seedURL = argv[1];
    char* normalized_url = normalizeURL(*seedURL);
    if (!isInternalURL(normalized_url)) {
        fprintf(stderr, "URL is not internal.\n");
        free(normalized_url);
        exit(1);
    }
    // *seedURL = normalized_url;
    free(normalized_url);

    // check pageDirectory
    if (pagedir_init(argv[2])) {
        *pageDirectory = argv[2];
    }
    else {
        fprintf(stderr, "Invalid page directory\n");
        exit(1);
    }

    int depth;
    if (sscanf(argv[3], "%d", &depth) != 1) {
        fprintf(stderr, "Please input a number for depth instead.\n");
    }
    if (depth < 0 || depth > 10) {
        fprintf(stderr, "Depth out of range.\n");
        exit(1);
    }
    else {
        *maxDepth = depth;
    }
}

/* ***************************
 * function to crawl the page
 */
static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    int id = 1;
    // initialize the hashtable and add the seedURL
    hashtable_t *visited_table = hashtable_new(100);
    char *seed = malloc(strlen(seedURL) + 1);
    strcpy(seed, seedURL);
    hashtable_insert(visited_table, seed, "seen");

    // initialize the bag and add a webpage representing the seedURL at depth 0
    webpage_t *page = webpage_new(seed, 0, NULL);
    bag_t *bag = bag_new();
    bag_insert(bag, page);

    webpage_t *current_page;
    while ((current_page = bag_extract(bag)) != NULL) {
        if (webpage_fetch(current_page)) {
            pagedir_save(current_page, pageDirectory, id);
            if (webpage_getDepth(current_page) < maxDepth) {
                pageScan(current_page, bag, visited_table);
            }
            id ++;
        }
        webpage_delete(current_page);
    }
    bag_delete(bag, webpage_delete);
    hashtable_delete(visited_table, NULL);
}

/* ***************************
 * function to scan all urls in the page
 */
static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    int pos = 0;
    char *result;
    // while there is another URL in the page
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
        char* normalized_url = normalizeURL(result);
        free(result);
        if (isInternalURL(normalized_url)) {
            if (hashtable_insert(pagesSeen, normalized_url, "seen")) {
                // create a webpage_t and insert the webpage into the bag
                webpage_t *new_page = webpage_new(normalized_url, webpage_getDepth(page) + 1, NULL);
                if (new_page != NULL) {
                    bag_insert(pagesToCrawl, new_page);
                }
                else {
                  free(normalized_url);  
                }
            }  
            else {
                free(normalized_url);
            } 
        } 
        else {
            free(normalized_url);
        }
    }
}