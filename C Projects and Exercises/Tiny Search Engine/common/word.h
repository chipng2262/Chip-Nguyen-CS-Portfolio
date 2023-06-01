/* 
 * word.h - header file for CS50 TSE 'word' module
 *
 * Chip Nguyen, CS 50 Spring 2023
 * May 2, 2023
 */

#ifndef __WORD_H__
#define __WORD_H__

#include <ctype.h>
#include <string.h>
#include <stdio.h>

/**************** normalizeWord ****************/
/**
 * 
 * Caller provides a non-NULL character pointer
 * Function returns if pointer points to null, else converts the word to all-lowercase
 */
void normalizeWord(char *word);
#endif