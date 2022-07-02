
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static void itemdelete(void *item);
static void idx_saver(void *arg, const char *word, void *item);
static void save_counts(void *arg, const int docID, int count);

/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new(int num_slots)
{
    return (index_t*) hashtable_new(num_slots);
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* idx, const char *word, counters_t* webcount)
{
    return hashtable_insert((hashtable_t*) idx, word, webcount);
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t *index_find(index_t *idx, const char *word)
{
    return (counters_t*) hashtable_find((hashtable_t*) idx, word);
} 

/**************** index_iterate() ****************/
/* see index.h for description */
void index_iterate(index_t *idx, void *arg, void (*itemfunc)(void *arg, const char *word, void *webcount) )
{
    hashtable_iterate((hashtable_t*) idx, arg, itemfunc);
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* idx)
{
    hashtable_delete((hashtable_t*) idx, itemdelete);
}

/**************** itemdelete() ****************/
/* calls counter_delete on each item in index */
static void itemdelete(void *item)
{
    counters_t* c = item;
    counters_delete(c);
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(char* idx_file)
{
    //open file
    FILE *fp = fopen(idx_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "unable to open file");
        exit(1);
    }
    //get num lines in file for best hashtable
    int num_line = lines_in_file(fp);
    //create new index, num hashslots are total num * 1.3: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-8.html
    index_t *index = index_new(num_line * 1.3);

    //read file and put info into index
    char *word;
    for (int line = 0; line < num_line; line++) {
        if ((word = freadwordp(fp)) != NULL) {
            counters_t* word_count = counters_new();
            int docID;
            int count;
            while (fscanf(fp, "%d %d", &docID, &count) == 2) {
                counters_set(word_count, docID, count);
            }
            index_insert(index, word, word_count);
            free(word);
        }
    }
    fclose(fp);
    return index;
}

/**************** index_save() ****************/
/* see index.h for description */
bool index_save(index_t* idx, char* idx_file)
{
    //check if index null
    if (idx == NULL) {
        fprintf(stderr, "null index");
        return false;
    }

    //check if file readable
    FILE *fp = fopen(idx_file, "w");
    if (fp == NULL) {
        fprintf(stderr, "cannot write to file");
        return false;
    }

    //iterate over index to print to file
    index_iterate(idx, fp, idx_saver);
    fclose(fp);
    return true;
}

/**************** idx_saver() ****************/
/* item_iterator function for index_save */
static void idx_saver(void *arg, const char *word, void *item)
{
	FILE *fp = arg;
    counters_t *c = item;
	fprintf(fp, "%s", word);
    //call counters_iterate to print the contents in the counters
	counters_iterate(c, fp, save_counts);
	fprintf(fp, "\n");
}

/**************** save_counts() ****************/
/* item_iterator function for idx_saver */
static void save_counts(void *arg, const int docID, int count)
{
	FILE *fp = arg;
	fprintf(fp, " %d %d", docID, count);
}