/* 
 * word.c implements word.h, which contains a function to normalize a word
 *
 * Chip Nguyen, COSC 50 Spring 2023
 * May 2, 2023
 */

#include <ctype.h>
#include <string.h>

/**************** normalizeWord ****************/
/* function to convert word to lowercase */
void
normalizeWord(char *word) {
    if (word == NULL) return;
    // iterate through each charater of the word and convert to lowercase
    for (int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }
}