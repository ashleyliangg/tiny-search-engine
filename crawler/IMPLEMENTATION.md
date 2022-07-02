# Implementation of crawler.c
### Ashley Liang, CS50 Spring 2022

## Overview of crawler 
`Crawler` largely follows the pseudocode in DESIGN.md.
`Crawler` is responsible for 'crawling' the web from a given _seedURL_, retrieving webpages until a given _maxDepth_. It parses the webpage, caching the content it finds in a given directory _pageDirectory_. If the current depth of that website is less than the _maxDepth_, the program extracts any emedded href URLs and retrieves those pages to continue to crawl at those URLs. 

`Crawler` must takes 3 arguments:
- _seedURL_ is the initial URL that crawler will parse through. It must be an internal URL (defined in DESIGN.md)
- _pageDirectory_ is the directory that the files of the webpage contents will be put in. It must be an existing, writable directory. If the directory is outside the current directory, the user must provide the pathway to the directory
- _maxDepth_ is the maximum depth that the crawler will go to extract/save the page. It must be an integer between 0 and 10.

## Crawler functions and Pseudocode
### Functions
`Crawler` calls the following functions in crawler.c:
```
static void crawler(char *seed_URL, int max_depth, char *page_dir);
static bool page_fetcher(webpage_t *wp, char *page_dir, int page_id);
static void page_scanner(webpage_t *wp, int curr_depth, hashtable_t *url_table, bag_t *webpage_bag, char *value);
inline static void logr(const char *word, const int depth, const char *url);
```
`Crawler` also calls functions within the `pagedir.h`, `memory.h`, `webpage.h`

### Pseudocode
```
int main(int argc, char *argv[])
```
- Validate that there are 3 arguments given. If that's not, exit non-zero
- Validate that the seed URL is an internal URL by calling `IsInternalURL` from webpage.h. If not, exit non-zero
- Validate that the _pageDirectory_ is a writable directory by calling `is_directory`. If not, exit non-zero
- Validate that the _maxDepth_ is an integer between 0-10. If not, exit non-zero
- Call `crawler` function
- returns 0

```
static void crawler(char *seed_URL, int max_depth, char *page_dir)
```
- Create a new bag to hold the webpages
- Create a hashtable to hold the URLs
- Initialize the current depth of the website to 0
- Allocate memory for the seed URL
- Create a new webpage struct with the seed URL
- Insert the webpage into the bag and URL into the hashtable
- While the bag isn't empty, extract a webpage and call `page fetcher`. If `page_fetcher` wasn't successful, continue to the next webpage. If the website's depth is less than the _maxDepth_, call `page_scanner` and increment the current depth. Finally, delete the webpage
- Delete the bag
- Delete the hashtable

```
static bool page_fetcher(webpage_t *wp, char *page_dir, int page_id)
```
- Call `webpage_fetch`
- If `webpage_fetch` was successful, call `logr` to log the process, then call `page_saver` to save the webpage contents. Finally, increment _page id_
- If it wasn't successful, print to standard error and delete the webpage

```
static void page_saver(char *page_dir, webpage_t *webpage, int page_id)
```
- Allocate memory for the filename
- Create the filename as "dirname/id_name
- If the file is writable, write the webpage URL, depth, and HTML to the file
- If the file isn't writable, print an error mesage
- free the filename

```
static void page_scanner(webpage_t *wp, int curr_depth, hashtable_t *url_table, bag_t *webpage_bag, char *value)
```
- call `logr` to print scanning process
- while there are URLs in the webpage (while `webpage_getNextURL` isn't null), get the URL. If the URL is not an internal URL, log that and free the URL. Otherwise, if it's not in the hashtable, add it to the hashtable and add a new webpage with that URL to the bag. If it is in the hashtable, log that there is a duplicate and free the URL.

```
inline static void logr(const char *word, const int depth, const char *url)
```
- prints a message to standard out in this format: [current depth]  [relevant word in process (eg Found or Scanning)]: [URL of website]

paraphrase what each func do
talk about pagedir directory
explain functions
have to create own directory
if it runs into a URL that isn't valid, just prints an error message to log and continues

## Data Structures
`crawler` uses a _bag_ and _hashtable_ struct (and the hashtable calls functions of the _set_ module). The _bag_ of webpages is used to store the webpages that the program should explore. The _hashtable_ of URLs is used to store the URLs/webpages that the program has already looked at.


## Security and Privacy Properties
All the functions in crawler.c other than the main function are static functions because they do not need to be seen from outside users.

## Error handling
This program terminates and exits non-zero if any of the arguments given in the command line (seedURL, maxDepth, and pageDirectory) are invalid. The program also terminates and exits non-zero if there are any errors involving memory allocation, using the function assertp in memory.h. Otherwise, if the program runs into errors fetching the HTML or saving the webpage information to a file, the program prints out an error message and continues on to the next webpage. If the program finds non-internal URLs (other than the seed URL, which has to be internal) or find duplicate URLs, it will print that information to stdout and continue.

## Resource management
This program uses `count_malloc` and `count_calloc` from `memory.h` to allocate memory. Each time the program allocates memory, the program frees it once it is no longer needed by calling a delete function or caling `free()`.

## Content Storage
The contents of the webpages crawled are stored in files with unique IDs in the _pageDirectory_ given.