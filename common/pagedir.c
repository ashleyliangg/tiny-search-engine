/* 
 * pagedir.c: c file for pagedir.h
 *
 * see pagedir.h for more information.
 *
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/memory.h"
#include "../libcs50/webpage.h"
#include "index.h"
#include "pagedir.h"
#include "../libcs50/file.h"


/**************** is_directory() ****************/
/* see pagedir.h for description */
bool is_directory(char *dir_name)
{
    FILE *fp;
    char *check_file = "/.crawler";
    //allocating memory for the dir_copy + /.crawler
    char *dir_copy = count_malloc(strlen(dir_name) + strlen(check_file) + 1);
    strcpy(dir_copy, dir_name);
    strcat(dir_copy, "/.crawler");
    if ((fp = fopen(dir_copy, "w")) == NULL) {
        free(dir_copy);
        return false;
    }
    else {
        fclose(fp);
        free(dir_copy);
        return true;
    }
}

/**************** page_saver() ****************/
/* see pagedir.h for description */
void page_saver(char *page_dir, webpage_t *webpage, int page_id)
{
    FILE *fp;
    //malloc for dirname + '/' + ID + '\0', enough memory for the whole filename
    char *filename = assertp(count_malloc(strlen(page_dir) + 20), "error allocating mem for filename\n");  
    sprintf(filename, "%s/%d", page_dir, page_id);
    //double check if file is writable
    if ((fp = fopen(filename, "w")) != NULL) {
        fprintf(fp, "%s\n", webpage_getURL(webpage));
        fprintf(fp, "%d\n", webpage_getDepth(webpage));
        fprintf(fp, "%s", webpage_getHTML(webpage));
        fclose(fp);
    }
    else {
        fprintf(stderr, "failed to save %s\n", webpage_getURL(webpage));
    }
    free(filename);
}

/**************** is_crawlerdir() ****************/
/* see pagedir.h for description */
bool is_crawlerdir(char *dir_name)
{
    FILE *fp;
    char *check_file = "/.crawler";
    //buffer array for the dir_copy + /.crawler
    char dir_copy[strlen(dir_name) + strlen(check_file) + 1];
    strcpy(dir_copy, dir_name);
    strcat(dir_copy, check_file);
    
    //check if ./crawler file exists
    if ((fp = fopen(dir_copy, "r")) == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

/**************** file_to_web() ****************/
/* see pagedir.h for description */
webpage_t *file_to_web(char *filename)
{
    //check if file is readable
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }
    //get URL
    char *URL = freadlinep(fp);
    //get depth
    char *char_depth = freadlinep(fp);
    //get HTML
    char *HTML = freadfilep(fp);

    //create webpage, assume that format is correct (URL is internal, depth is integer, HTML is valid)
    webpage_t *wp = webpage_new(URL, atoi(char_depth), HTML);
    free(char_depth);
    fclose(fp);
    return wp;
}