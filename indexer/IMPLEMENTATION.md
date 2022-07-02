# Implementation of indexer.c
### Ashley Liang, CS50 Spring 2022


## Overview of indexer 
`Indexer` follows the basic pseudocode in DESIGN.md
`Indexer` reads the document files produced by the TSE crawler, builds an _index_ mapping words to webpage docIDs, and writes that _index_ to a _file_. Its companion, `indextest`, loads an _index file_ produced by the indexer and saves it to another _file_.

`Indexer` must takes 2 arguments:
- *page_directory* is the pathname to the crawler-made directory
- *index_filename* is the pathname to the file that the indexer write the inverted index to

## Relevant Files and Function Explanation
In addition to the files mentioned below, indexer uses APIs from `webpage.h`, `memory.h`, and `file.h`.
### indexer.c
`indexer.c` has the following functions:
```
int main(int argc, char *argv[]);
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file);
static index_t *index_build(char *pageDirectory)
static void index_page(index_t **index, webpage_t *wp, int docID)
```
```
int main(int argc, char *argv[]);
```
- Initialize the *page_dir* and *index_file* variables
- Call `validate_args` to validate arguments
- Call `index_build` to create an index for *page_dir*
- Initialize `index_save` to save the index to *index_file*
- Delete the index

```
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file);
```
- Check if there are 2 arguments given, print an error message and exit non-zero if not
- Assign *page_dir* to the first argument. If *page_dir* isn't a crawler-made directory (by calling `is_crawlerdir`), print an error message and exit non-zero
- Assign *index_file* to the second argument. If *index_file* isn't a writable file, print an error message and exit non-zero

```
static index_t *index_build(char *pageDirectory);
```
- Create an index with 500 slots
- Initialize the docID as 1
- Create an array (with buffer) for the filename and print *page_dir/docID* to the array
- Declare webpage variable
- Call `file_to_web`. While that isn't null, call `index_page` to parse the webpage contents. Then call `webpage_delete`. Increment the docID and update the filename
- return the index

```
static void index_page(index_t **index, webpage_t *wp, int docID)
```
- Declare a counters variable
- Initialize the minimum character count considered to 3
- While `webpage_getNextWord` isn't NULL and if the word is at least the minimum character count, normalize the word. If the word/counters pair is in the index, add the webpage to the counters. Otherwise, create a new counters, add to the counters, and insert the counters into the index. Free the word at the end.
-----------------------------------------------------

### indextest.c
`indextest.c` has 1 function, the main function.
```
int main(int argc, char *argv[]); 
```
- Assign variables to each of the arguments
- Verify that there are only 2 command-line arguments (other than the ./indextest)
- Call `index_load` to create an index from an indexfile
- Call `index_save` to save the index to a new file
-----------------------------------------------------

### index.c
`index.c` has the following functions:
```
index_t* index_new(int num_slots);
bool index_insert(index_t* idx, const char *word, counters_t* webcount);
counters_t *index_find(index_t *idx, const char *word);
void index_iterate(index_t *idx, void *arg, void (*itemfunc)(void *arg, const char *word, void *webcount) );
void index_delete(index_t* idx);
static void index_delete(index_t* idx);
index_t* index_load(char* idx_file);
bool index_save(index_t* idx, char* idx_file);
static void idx_saver(void *arg, const char *word, void *item);
static void save_counts(void *arg, const int docID, int count);
```
```
index_t* index_new(int num_slots);
```
- Call `hashtable_new` to create a new index

```
bool index_insert(index_t* idx, const char *word, counters_t* webcount);
```
- Call `hashtable_insert` to insert a word/counters pair into the index

```
counters_t *index_find(index_t *idx, const char *word);
```
- Call `hashtable_find` to find a counters from its key

```
void index_iterate(index_t *idx, void *arg, void (*itemfunc)(void *arg, const char *word, void *webcount) );
```
- Call `hashtable_iterate` to iterate through the index

```
void index_delete(index_t* idx);
```
- Call `hashtable_delete` to delete the index

```
static void index_delete(index_t* idx);
```
- Helper function called by `hashtable_delete` to delete the counters in the index

```
index_t* index_load(char* idx_file);
```
- Check if file is readable
- Call `lines_in_file` to get the number of lines in the file
- Create a new index with the number of lines in the file * 1.3
- For each line in the file, read the first word and create a counters. Scan the rest of the line for the docID/count pair and add that to the counters with `counters_set`. Insert the word/counters into the index and free the word
- Return the index

```
bool index_save(index_t* idx, char* idx_file);
```
- Check if the *idx_file is writable, if not, return false
- Call `index_iterate` to print the contents of the file
- Close the file and return true

```
static void idx_saver(void *arg, const char *word, void *item);
```
- Helper function called by `index_iterate` that prints the word and calls `counter_iterate` to print the contents of the counter

```
static void save_counts(void *arg, const int docID, int count);
```
- Helper function called by `counters_iterate` that prints each docID and count
-----------------------------------------------------

### pagedir.c

`pagedir.c` has the following relevant functions:
```
bool is_crawlerdir(char *dir_name);
webpage_t *file_to_web(char *filename);
```

```
bool is_crawlerdir(char *dir_name);
```
- Check if the directory is a crawler-made directory by seeing if a ./crawler file exists in the directory (if the file is readable)

```
webpage_t *file_to_web(char *filename);
```
- Opens up the file to read
- Read the lines. The first line is the URL, second is the depth, and the rest of the file is the HTML
-----------------------------------------------------

### word.c
`pagedir.c` has 1 function:
```
char *normalize_word(char *word);
```
- Go through each character in the string and convert it to lowercase

## Data Structures
`indexer` uses an _index_ struct, which is a more specific _hashtable_ module where the key is a word and the item is a _counters_. The _counters_ stores each webpage's document ID that contains the key word and a count of how many times that word appears in that webpage.

## Security and Privacy Properties
All the functions in indexer.c other than the main function are static functions because they do not need to be seen from outside users. All helper functions in index.c are static because outside users also do not need to see them.

## Error handling
This program terminates and exits non-zero if any of the arguments given in the command line (*page_dir* and *index_filename*) are invalid. The program also terminates and exits non-zero if there are any errors involving memory allocation, using the function `assertp` in memory.h. Otherwise, if the program runs into errors fetching the HTML or saving the webpage information to a file, the program prints out an error message and continues on to the next webpage. If the program finds non-internal URLs (other than the seed URL, which has to be internal) or find duplicate URLs, it will print that information to stdout and continue.

## Resource management
This program uses `count_malloc` and `count_calloc` from `memory.h` to allocate memory. Each time the program allocates memory, the program frees it once it is no longer needed by calling a delete function or caling `free()`.

## Content Storage
The contents of the index are stored in the *index_filename* given.

## Testing

*Unit testing*.  A small test program to test each module to make sure it does what it's supposed to do.
- `indextest.c` tests the `index_load` and `index_save` in the index module by seeing if taking an indexfile, converting it to an index, and converting that back to an indexfile results in the same file content.

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output indexfile carefully to be sure the format and output is correct.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the indexer with a `page_directory` that points to a page directory not created by crawler.

3. Test the indexer with a `index_file` that points to a non-writable file.

4. Run indexer on letters at depth 0, 2, 4, 5. Run index and verify that it matches expectations with the cs50 sample output. Run `index_load` to create a copy and verify that it is the same.

5. Repeat with the Wikipedia playground at depths 0, 1, and 2.