/*
 * indextest.c - tests the index module functions by reading an indexfile 
 * and creating a new index from it
 * 
 * input: an old index filename and a filename to write the new index to
 * output: a file with the index contents, should be identical to old index file
 * 
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/index.h"

/*********** main ***********/
int main(int argc, char *argv[]) 
{
    char *program = argv[0];
    char *old_index = argv[1];
    char *new_index = argv[2];
    //validate command-line arguments
    if (argc != 3) {
        fprintf(stderr, "%s must have 2 arguments: oldIndexFilename and newIndexFilename\n", program);
        return 1;
    }

    //load index from old_index file to inverted-index data structure
    //index_load check if the file is readable already
    index_t *index = index_load(old_index);

    //write index to file, index_save checks if file is writable
    if (!index_save(index, new_index)) {
        fprintf(stderr, "index did not save correctly");
        return 2;
    }

    return 0;
}