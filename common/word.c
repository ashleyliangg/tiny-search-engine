/* 
 * word.c: c file for word.h
 *
 * see word.h for more information.
 *
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/******normalize_word********/
/* see word.h for description */
char *normalize_word(char *word)
{
    if (word == NULL) {
        return NULL;
    }
    //interate through each character in the word
    for (char *c = word; *c != '\0'; c++) {
        *c = tolower(*c);
    }
    return word;
}