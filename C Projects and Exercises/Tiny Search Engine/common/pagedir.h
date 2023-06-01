/* 
 * pagedir.h - header file for pagedir.c
 * 
 * Chip Nguyen, CS 50 Spring 2023
 * April 25, 2023
 */

#ifndef __PAGEDIR_H__
#define __PAGEDIR_H__

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/**
 * 
 * Caller provides a non-NULL char pointer that holds the name of a directory
 * Function returns:
 *  true if the directory is valid and accessible
 *  false otherwise
 */
bool pagedir_init(char* pageDirectory);

/**************** pagedir_save ****************/
/**
 * Write a page's URL, depth, and HTML content to a file
 * Caller provides:
 * a valid webpage_t pointer, a valid directory, and a valid ID
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/**
 * Check if a directory is produced by the crawler
 * Caller provides:
 * a valid pointer to a directory name
 * Function returns:
 *  false if pageDirectory is null or invalid or directory is not crawler-produced
 *  true otherwise
 */
bool pagedir_validate(char* pageDirectory);

/**************** pagedir_load ****************/
/**
 * Load a page from a file in the directory 
 * Caller provides:
 * a valid directory name and an id
 * Function returns:
 *  a webpage struct if loading is successful
 *  null if invalid inputs or any failure in the process
 * */
webpage_t* pagedir_load(char* pageDirectory, int id);

/**************** pagedir_getLink ****************/
/**
 * Get url link of the file pageDirectory/id 
 * Caller provides:
 * a valid directory name and an id
 * Function returns:
 *  a link if successfuls
 *  null if there's an issue
 * */
char* pagedir_getLink(char *pageDirectory, int id);
#endif