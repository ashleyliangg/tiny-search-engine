/*
 * pagedir.h - headerfile for pagedir module
 * 
 * various functions needed by multiple parts of TSE related to pages and directories
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
#include "../libcs50/file.h"
#include "word.h"
/**************** is_directory ****************/
/* Determine whether the pathname provided as pageDirectory is a directory/writable
 *
 * Caller provides:
 *   directory name
 * We do:
 *   if isn't valid, writable directory, returns false
 *   if is valid, writable directory, closes file and returns true
 */
bool is_directory(char *dir_name);

/**************** page_saver ****************/
/* Saves the contents of a webpage (URL, depth, HTML) into a file with a unique file name
 *
 * Caller provides:
 *   valid page directory
 *   valid webapge
 *   valid HTML
 * 
 * We do:
 *   create a file with the directory name and a unique ID
 *   if the file is writable, write the contents of the webpage into the file
 *   the URL is on the first line, the depth is on the second line, and the HTML
 *   (unchanged from the webpage) is on the rest of the lines
 */
void page_saver(char *page_dir, webpage_t *webpage, int page_id);

/**************** is_crawlerdir ****************/
/* Determine whether the pathname provided as pageDirectory is a directory/writable
 *
 * Caller provides:
 *   directory name
 * We do:
 *   if the directory isn't made by crawler, return false
 *   if it is made by crawler, return true
 */
bool is_crawlerdir(char *dir_name);

/**************** file_to_web ****************/
/* converts file contents into a webpage
 *
 * Caller provides:
 *   a file with the correct crawler-made file layout (can assume formatting is corect)
 * We do:
 *   convert the file contents into a webpage
 *   return the webpage
 */
webpage_t *file_to_web(char *filename);