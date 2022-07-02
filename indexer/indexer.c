/*
 * indexer.c - reads the document files produced by the TSE crawler, builds an index 
 * mapping words to webpage docIDs, and writes that index to a file.
 * 
 * input: an internal seedURL (defined in README.md), an existing directory, and a maxDepth to crawl
 * output: files of webpage contents in the existing directory
 * 
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/memory.h"
#include "../common/index.h"

/******* local function prototypes *******/
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file);
static index_t *index_build(char *pageDirectory);
static void index_page(index_t **index, webpage_t *wp, int docID);

/*********** main ***********/
int main(int argc, char *argv[]) 
{
    //index parameters from command line
    char *page_dir = NULL;
    char *index_file = NULL;
    
    //validating arguments, exits if error
    validate_args(argc, argv, &page_dir, &index_file);

    //build index in memory by loading data from the page directory
    index_t *word_index = index_build(page_dir);

    //save index to external file
    if (!index_save(word_index, index_file)) {
        fprintf(stderr, "unable to save index\n");
    }

    //delete indexer
    index_delete(word_index);

    return 0;
}

/*********** validate_args *********/
/* Parse the command-line args, filling in parameters
 */
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file)
{
    char *program = argv[0];
    //validate num of parameters (2 arguments)
    if (argc != 3) {
        fprintf(stderr, "%s must have 2 arguments: page_directory and index_filename\n", program);
        exit(1);
    }

    //check if directory is produced by crawler
    *page_dir = argv[1];
    if (!is_crawlerdir(*page_dir)) {
        fprintf(stderr, "%s: directory %s must be made by crawler\n", program, *page_dir);
        exit(2);
    }

    //check if indexfile is the pathname of writable file
    *index_file = argv[2];
    FILE *fp;
    if ((fp = fopen(*index_file, "w")) == NULL) {
        fprintf(stderr, "%s: index_file %s must be writable\n", program, *index_file);
        exit(3);
    }
    else {
        fclose(fp);
    }
}

/**************** index_build ****************/
/* Creates an index from files within a crawler-made directory
 * based on the words in each file and mapping it to its 
 * corresponding docID and count
 */
static index_t *index_build(char *page_directory)
{
    const int hashtable_value = 500;
    //create new index
    index_t *index = assertp(index_new(hashtable_value), "error creating index\n");
    //start scanning for files, first one will always be /1
    int docID = 1;
    //allocate space for 'pagedirectory/docID'
    char filename[strlen(page_directory) + 20];
    sprintf(filename, "%s/%d", page_directory, docID);

    webpage_t *wp;
    //check if file exists
    while ((wp = file_to_web(filename)) != NULL) {
        //parses the webpage words and maps it to an index
        index_page(&index, wp, docID);
        webpage_delete(wp);

        //increment docID so can open next file
        docID++;
        sprintf(filename, "%s/%d", page_directory, docID);
    }
    
    return index;
}

/**************** index_page ****************/
/* scans all the words in the webpage and adds maps the word and 
 * docID/count to the index
 */
static void index_page(index_t **index, webpage_t *wp, int docID)
{
    //index with word as the key, word_count counterset as item
    //counterset holds 
    int pos = 0;
    char *word;
    counters_t* word_count;
    int min_char_count = 3;

    //for each word in the webpage
    while ((word = webpage_getNextWord(wp, &pos)) != NULL) {
        //ignore words fewer than 3 characters (min_char_count)
        if (strlen(word) >= min_char_count) {
            //normalize the word
            normalize_word(word);
            //if word/counterset pair for that word exists, just add docID to the counterset
            if ((word_count = index_find(*index, word)) != NULL) {
                counters_add(word_count, docID);
            }
            //if word/counterset pair does not yet exist, create a new counterset, add the docID to it 
            //and insert into index
            else {
                counters_t* new_count = assertp(counters_new(), "error making counters");
                counters_add(new_count, docID);
                index_insert(*index, word, new_count);
            }
        }
        free(word);
    }
}