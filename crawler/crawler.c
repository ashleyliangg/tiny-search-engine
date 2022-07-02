/*
 * crawler.c - a program that crawls the web from a given seed to a 
 * given maxDepth and caches the content of the pages it finds, one page per file, 
 * in a given directory.
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
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/memory.h"

//helper function declarations - static functions because should stay local to this file
static void crawler(char *seed_URL, int max_depth, char *page_dir);
static bool page_fetcher(webpage_t *wp, char *page_dir);
static void page_scanner(webpage_t *wp, int depth, hashtable_t *url_table, bag_t *webpage_bag, char *value);
inline static void logr(const char *word, const int depth, const char *url);

int main(int argc, char *argv[]) 
{
    int max_depth;
    char excess;
    int min_depth_range = 0;
    int max_depth_range = 10;

    //validate num of parameters (3 arguments)
    if (argc != 4) {
        fprintf(stderr, "must have 3 arguments: seedURL, pageDirectory, maxDepth\n");
        exit(1);
    }

    //validate seedURL (param 1)
    if (!IsInternalURL(argv[1])) {
        fprintf(stderr, "seedURL must be an \"internal\" URL, as defined in README.md\n");
        exit(2);
    }
    
    //validate directory name (param 2)
    if (!is_directory(argv[2])) {
        fprintf(stderr, "pageDirectory must be a writable directory\n");
        exit(3);
    }

    //validate int value and maxDepth range (param 3)
    if (sscanf(argv[3], "%d%c", &max_depth, &excess) != 1) {
        fprintf(stderr, "maxDepth must be an integer\n");
        exit(4);
    }
    if (max_depth < min_depth_range || max_depth > max_depth_range) {
        fprintf(stderr, "maxDepth must be in the range [%d-%d]\n", min_depth_range, max_depth_range);
        exit(5);
    }

    //call crawler
    crawler(argv[1], max_depth, argv[2]);

    return 0;
}

/* Overall function that creates the webpage bag and URL hashtable data structures.
 * Extracts webpages from the bag of webpages to be seen and calls page_fetcher (which
 * calls page_saver) and page_scanner.
 */
static void crawler(char *seed_URL, int max_depth, char *page_dir)
{
    //setting the initial depth of the webpage crawl to 0 at seed URL
    const int init_depth = 0;
    //num of potential items inserted into hashtable
    const int pot_items = 500;
    //arbitratry hashtable value so we can hold URL as key
    char *value = "";

    //bag of webpages
    bag_t* webpage_bag = assertp(bag_new(), "error making bag for website\n");
    //hashtables of URLs seen
    //rule of thumb regarding hashtable sizing: https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-8.html
    hashtable_t *url_table = assertp(hashtable_new(pot_items * 1.3), "error making hashtable\n");

    //webpage for seedURL, marked with initial depth. insert webpage into bag and URL into hashtable
    char *seed_web_url = assertp(count_malloc(strlen(seed_URL) + 1), "error allocating mem for url\n");
    strcpy(seed_web_url, seed_URL);
    webpage_t *seed_web = assertp(webpage_new(seed_web_url, init_depth, NULL), "error creating webpage\n");
    bag_insert(webpage_bag, seed_web);
    hashtable_insert(url_table, seed_web_url, value);
   
    //while bag isn't empty
    webpage_t *wp;
    while ((wp = bag_extract(webpage_bag)) != NULL) {
        //fetch html, check if fetched successfully
        //page_fetcher calls page_saver if fetches HTML correctly
        if (!page_fetcher(wp, page_dir)) {
            continue;
        }

        //if webpage depth < maxDepth, explore webpage for links    
        if (webpage_getDepth(wp) < max_depth) {
            page_scanner(wp, webpage_getDepth(wp), url_table, webpage_bag, value);
        }
        //delete the webpage when done
        webpage_delete(wp);
    }
    //delete the bag of webpages
    bag_delete(webpage_bag, webpage_delete);
    //delete hashtable: null because didn't allocate memory for ""
    hashtable_delete(url_table, NULL);
}

/* Function that fetches the HTML from a webpage. If the page successfully
 * fetches the HTML, the function calls page_saver() from pagedir.h that saves the page
 */
static bool page_fetcher(webpage_t *wp, char *page_dir)
{
    static int page_id = 1;
    bool html_success = webpage_fetch(wp);
    //check if successful in getting HTML
    if (html_success) {
        logr("Fetched", webpage_getDepth(wp), webpage_getURL(wp));
        //save the webpage as a file
        page_saver(page_dir, wp, page_id);
        page_id++;
    }
    else {
        //otherwise, print an error message
        fprintf(stderr, "Failed to fetch HTML from %s\n", webpage_getURL(wp));
        webpage_delete(wp);
    }
    return html_success;
}

/* Function that scans the current webpage for any URLs, inserts them into the 
 * hashtable, and creates a new webpage for it to insert into thewebpage bag
 */
static void page_scanner(webpage_t *wp, int depth, hashtable_t *url_table, bag_t *webpage_bag, char *value)
{
    logr("Scanning", depth, webpage_getURL(wp));
    int pos = 0;
    char *result;
    //fetches the URLs from the current webpage
    while ((result = webpage_getNextURL(wp, &pos)) != NULL) {
        logr("Found", depth, result);
        //normalize and check if URL internal (normalize as side effect)
        if (!IsInternalURL(result)) {
            logr("IgnExtrn", depth, result);
            free(result);
        }
        else {
            //if insert into hashtable of URLs (hasn't been seen yet), make webpage and insert into bag
            if (hashtable_insert(url_table, result, value)) {
                webpage_t *new_web = webpage_new(result, depth + 1, NULL);
                bag_insert(webpage_bag, new_web);
                logr("Added", depth, result);
            }
            else {
                //otherwise note that there was a duplicate
                logr("IgnDupl", depth, result);
                free(result);
            }
        }
    }
}

/* Function that provides a consistent format for logging the process
 * (eg fetching, scanning, adding)
 */
inline static void logr(const char *word, const int depth, const char *url)
{
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}