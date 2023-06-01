/* 
 * pagedir.c implements pagedir.h
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * April 26, 2023
 */

#ifndef __PAGEDIR_H__
#define __PAGEDIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/set.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagedir_init ****************/
/* function to verify if the pageDirectory is valid or accessible */
bool
pagedir_init(char* pageDirectory)
{
    if (pageDirectory == NULL) {
        return false;
    }
    else {
        // construct the pathname for the .crawler file in the directory
        const char* file_name = "/.crawler";
        char* path = malloc(strlen(pageDirectory) + strlen(file_name) + 2);
        memcpy(path, pageDirectory, strlen(pageDirectory) + 1);
        strcat(path, file_name);

        // check if successfully access file
        FILE* fp = fopen(path, "w");
        if (fp == NULL) {
            free(path);
            return false;
        }
        else {
        fclose(fp);
        free(path);
        return true;
        }
    }
}

/**************** pagedir_save ****************/
/* function to write a page's URL, depth, and HTML content to a file */
void
pagedir_save(webpage_t* page, const char* pageDirectory, const int docID)
{   
    // convert docID to string and store in file_id
    char* file_id = malloc(sizeof(char) * 12);
    sprintf(file_id, "%d", docID); 

    // create a path name in the form "pageDirectory/docID"
    char* path = malloc(strlen(pageDirectory) + strlen(file_id) + 2); 
    memcpy(path, pageDirectory, strlen(pageDirectory) + 1);
    strcat(path, "/");
    strcat(path, file_id);

    // print to file
    if (webpage_getDepth(page) >= 0 && webpage_getHTML(page) != NULL) {
        FILE* fp;
        if ((fp = fopen(path, "w")) != NULL) {
            fprintf(fp, "%s\n", webpage_getURL(page));
            fprintf(fp, "%d\n", webpage_getDepth(page));
            fprintf(fp, "%s", webpage_getHTML(page));
        }
        fclose(fp);
    }
    free(path);
    free(file_id);
}

/**************** pagedir_validate ****************/
/* function to verify whether a directory is a Crawler-produced directory */
bool
pagedir_validate(char* pageDirectory) {
    if (pageDirectory == NULL) return false;

    const char* file_name = "/.crawler";
    char* path = malloc(strlen(pageDirectory) + strlen(file_name) + 2);
    memcpy(path, pageDirectory, strlen(pageDirectory) + 1);
    strcat(path, file_name);
    
    // check if a crawler file exists and is accessible
    bool isCrawlerDir = access(path, F_OK) == 0;
    free(path);
    return isCrawlerDir;
}

/**************** pagedir_load ****************/
/* load a page from a file in the directory */
webpage_t* 
pagedir_load(char* pageDirectory) {
    FILE* fp = fopen(pageDirectory, "r");
    char* url = malloc(1000);
    char* html;
    int depth;

    if (fp == NULL) return NULL;

    // check if fscanf has sucessfully read and assigned 2 values
    if (fscanf(fp, "%s\n%d\n", url, &depth) != 2) {
        fclose(fp);
        return NULL;
    }
    
    // read html content
    html = file_readUntil(fp, NULL);

    // create new webpage
    webpage_t *page = webpage_new(url, depth, html);

    if (page == NULL) {
        fclose(fp);
        return NULL;
    }
    else {
        fclose(fp);
        return page;
    }
}

/**************** pagedir_getLink ****************/
/* get url link of the file pageDirectory/id */ 
char* 
pagedir_getLink(char *pageDirectory, int id)
{
    // convert docID to string and store in file_id
    char* file_id = malloc(sizeof(char) * 12);
    sprintf(file_id, "%d", id);

    // create a path name in the form "pageDirectory/docID"
    char* path = malloc(strlen(pageDirectory) + strlen(file_id) + 2); 
    memcpy(path, pageDirectory, strlen(pageDirectory) + 1);
    strcat(path, "/");
    strcat(path, file_id);
    free(file_id);

    FILE* fp = fopen(path, "r");

    if (fp != NULL) {
        webpage_t* page = pagedir_load(path);
        char* url = webpage_getURL(page);
        char* link = malloc(500);        

        strcpy(link, url);
        
        webpage_delete(page);
        fclose(fp);
        free(path);
        return link;
    }
    else {
        free(path);
        return NULL;
    }
}
#endif