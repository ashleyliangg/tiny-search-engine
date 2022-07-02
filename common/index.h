/* 
 * index.h - header file for index module
 *
 * An *index* acts just like a hashtable but is more specific
 * the key is a word and the item is a counters set
 *
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
index_t* index_new(int num_slots);

/**************** index ****************/
/* Insert item, identified by word (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid pointer for counters.
 * We return:
 *   false if word exists in index, any parameter is NULL, or error;
 *   true if new counters was inserted.
 * Notes:
 *   The word string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the word string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its word string after this call.  
 */
bool index_insert(index_t* idx, const char *word, counters_t* webcount);

/**************** index_find ****************/
/* Return the item associated with the given word.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word.
 * We return:
 *   pointer to the counterset corresponding to the given word, if found;
 *   NULL if index is NULL, word is NULL, or word is not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
void *index_find(index_t *idx, const char *word);

/**************** index_iterate ****************/
/* Iterate over all items in the index; in undefined order.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (word, counters) pair.
 * We do:
 *   nothing, if index==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, word, counters).
 * Notes:
 *   the order in which counters are handled is undefined.
 *   the index and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the counters.
 */
void index_iterate(index_t *idx, void *arg, void (*itemfunc)(void *arg, const char *word, counters_t *webcount) );

/**************** index_delete ****************/
/* Delete the given index.
 *
 * Caller provides:
 *   valid pointer to index.
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless call counters_delete on each counters in the index
 *   free all the key (words), and the index itself.
 * Notes:
 *   We free the strings that represent key for each counters, because 
 *   this module allocated that memory in index_insert.
 */
void index_delete(index_t* idx);

/**************** index_load ****************/
/* Return an index from a given indexfile.
 *
 * Caller provides:
 *   valid indexfile name.
 * We do:
 *   if file doens't exist/isn't readable, print error message and exit non-zero.
 *   otherwise, read the file and convert the contents into an index
 * Notes:
 *   we assume that the valid index file has the correct formatting
 */
index_t* index_load(char* idx_file);

/**************** index_save ****************/
/* Saves an index to a provided index filename.
 *
 * Caller provides:
 *   a pointer to an index and indexfile name.
 * We do:
 *   if file doens't exist/isn't readable, print error message and exit non-zero.
 *   if pointer is NULL, print error message and exit non-zero
 *   otherwise, iterate through the index and print it to the index_file
 */
bool index_save(index_t* idx, char* idx_file);