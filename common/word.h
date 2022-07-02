/* 
 * word.h - header file for word module
 *
 * the word module includes functions related to strings/words
 *
 * Ashley Liang. CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/******normalize_word********/
/* converts a string to all lowercase
 *
 * Caller provides:
 *   a word (char *)
 * We do:
 *   if the word is NULL, returns NULL
 *   if it isn't NULL, converts the word to all lowercase and return the word
 */
char *normalize_word(char *word);