# Implementation of querier.c
### Ashley Liang, CS50 Spring 2022


## Overview of querier 
`querier` follows the basic pseudocode in DESIGN.md
`querier` reads the *index_filename* produced by the `indexer` and the page files in the *page_directory* produced by the `crawler` to produce a ranked list of webpages answering the user's written search query.

`querier` must takes 2 arguments:
- *page_directory* is the pathname to the crawler-made directory
- *index_filename* is the pathname to the index file that the indexer made

## Relevant Files and Function Explanation
In addition to the files mentioned below, querier uses APIs from `webpage.h`, `memory.h`, `file.h`, `index.h`, `pagedir.h`, `word.h`, and `counters.h`.

### querier.c
`querier.c` has the following functions:
```
int main(int argc, char *argv[]);
int fileno(FILE *stream);
static void prompt(void);
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file);
static char **read_input(char *line, int *num_words);
static bool validate_query(char **word_array, int num_words);
static bool is_operator(char *word);
static counters_t *process_query(char **word_array, int num_words, index_t *idx);
static void counters_intersect(counters_t* temp, counters_t* other);
static void intersect_helper(void *arg, const int key, const int count);
static inline int min(const int a, const int b) {
  return (a < b ? a : b);
}
static void counters_union(counters_t* result, counters_t* other);
static void union_helper(void *arg, const int key, const int count);
static void result_rank(counters_t *result, char *page_directory);
static void counters_key_count(void *arg, const int key, const int count);
static void array_add_doc(void *arg, const int key, const int count);

```
```
int main(int argc, char *argv[]);
```
- initialize the *page_dir* and *index_file* variables
- call `validate_args` to validate arguments
- call `index_load` to create an index from *index_filename*
- call `prompt` to prompt "Query?"
- while `freadlinep(stdin)` isn't null:
    - initialize number of words to 0
    - call `read_input` to get the char ** word array
    - call `validate_query`. if it returns false, prompt and continue
    - iterate through the word array to print a clean, normalized query
    - call `process_query` to get the final counters that takes in the and/or operators
    - call `result_rank` to rank and print out the final list
    - delete the counters
    - free the line
    - reprompt for the next line
- delete the index
- return 0

```
static void prompt(void);
```
- if the stdin is a tty, it prints prompt "Query?"

```
static void validate_args(const int argc, char *argv[], char **page_dir, char **index_file);
```
- Check if there are 2 arguments given, print an error message and exit non-zero if not
- Assign *page_dir* to the first argument. If *page_dir* isn't a crawler-made directory (by calling `is_crawlerdir`), print an error message and exit non-zero
- Assign *index_filename* to the second argument. If *index_filename* isn't a readable file, print an error message and exit non-zero

```
char **read_input(char *line, int* num_words);
```
- get the length of the line by calling `strlen`
- if the line length is even, max_words = line_length / 2; if it's odd, then max_words = (line_length / 2) + 1
- allocate memory for the word array by multiplying sizeof(char*) with max_words
- initalize pointers to the beginning and end of a word to the beginning of the line, word array_index to 0, and start of a word to false
- while the begin and end pointers are not '\0'
    - check if the begin and end pointers are non-alpha, non-space characters. if they are, print an error statement and return NULL
    - if start_word is false, increment begin until it reaches a letter (`isalpha`), then set start_word to true and end to begin.
    - if start_word is true, increment end until it reaches a space (`issapce`), then set that character to '\0', increment end, normalize the word (by calling `normalize_word` on the begin pointer), add that word to the word array, increment the index and the num_words added, set begin = equal, and start_word = false
- if begin is still a letter after exiting the while loop, normalize it and add it to the array list, incrementing the index and num_words to get the last word in the query
-return the word array

```
bool validate_query(char** word_array, int num_words);
```
- check if num_words is 0 or word_array is NULL. if so, return false
- check if beginning of end of query is an operator. if so, print an error statement and return false
- if num_words is more than 3, loop through the array. if a word at array[i] is an operator, check if array[i+1] is an operator. if so, print na error statement and return false

```
bool is_operator(char *word);
```
- returns true if the word is "and" or "or", otherwise false

```
counters_t *process_query(char **word_array, int num_words, index_t *idx);
```
- initalize a counters _temp_ to NULL
- initialize a counters _result_ to `counters_new`
- iterate through the array with a while-loop:
    - if the word is "and" (with `stsrcmp`), continue
    - if the word is "or", call `counters_union` between _result_ and _temp_. call `counters_delete` on _temp_ and set _temp_ to NULL
    - else, call `index_find` on the word to get the counters for that word. if _temp_ is NULL, call `counters_new` on it and `counters_union` between _temp_ and *word_count*. otherwise, call `counters_intersect` between _temp_ and *word_count*. if the word is the last word in the array (i == num_words - 1), call `counters_union` between _result_ and _temp_ and delete the _temp_ counters.
- free the word_array
- return the _result_ counters

```
void counters_intersect(counters_t* temp, counters_t* other);
```
- creates a *counter_pair* with the two parameters
- calls `counters_iterate` on the temp, passing in the counter pair and the `intersect_helper` function

```
void intersect_helper(void *arg, const int key, const int count);
```
- set arg to a *counter_pair*
- call `counters_set` to set the key to the minimum count between the two counters in the counter pair

```
void counters_union(counters_t* result, counters_t* other);
```
- creates a *counter_pair* with the two parameters
- calls `counters_iterate` on the other, passing in the counter pair and the `union_helper` function

```
void union_helper(void *arg, const int key, const int count);
```
- creates a *counter_pair* with the two parameters
- new_count is the sum of the two counter's counts of the key parameter
- set the counters to the new count

```
void result_rank(counters_t *result, char *page_directory);
```
- get the number of total documents to create an appropriately sized array:
    - call `counters_iterate` and pass in a *num_docs* variable and helper function to get the number of docs
- if the number of docs is 0, print "No documents match."
- else:
    - allocate memory for a doc array with the num_docs
    - create an array_doc with the doc array and an initial 0 for number inserted into the array
    - call `counters_iterate` on the result and pass in the array_doc and helper function to insertion sort the result docs into the array
    - print out the ranked array:
            - iterate through the array, opening the file of its corresponding ID and getting the URL. print the score, doc, and URL
            - free the URL and close the file
    - free the docs in the doc array as well as the docs

```
void counters_key_count(void *arg, const int key, const int count);
```
- set int* *num_docs* to arg
- if the key > 0 and count > 0, increment *num_docs

```
void array_add_doc(void *arg, const int key, const int count)
```
- set *array_doc* to arg
- if count is more than 0:
    - create a new *doc* for the key and count, allocate memory for it
    - compare the new doc's score to the scores of the docs in the array, starting from the doc in the last index. if the new doc's score is higher, move the doc in the array one position to the right. finally, insert the doc into the correct index in the sorted array and increment the array_doc's num_inserted

## Data Structures
`querier` besides creating an *index* from the `index.h` module, has several local data structures. 
- *counter_pair* that holds a pair of counters: a 'result' counters and an 'other' counters
- *doc* that holds the docID and score for a document
- *array_doc* that holds an array of *docs* and the number of *docs* inserted into the array

## Security and Privacy Properties
All the functions in querier.c other than the main function are static functions because they do not need to be seen from outside users. 

## Error handling
This program terminates and exits non-zero if any of the arguments given in the command line (*page_directory* and *index_filename*) are invalid. The program also terminates and exits non-zero if there are any errors involving memory allocation, using the function `assertp` in memory.h. Otherwise, if the program runs into errors with the query (invalid query), the program prints an error statement and prompts for a new query.

## Resource management
This program uses `count_malloc_assert` and `count_calloc_assert` from `memory.h` to allocate memory. Each time the program allocates memory, the program frees it once it is no longer needed by calling a delete function or caling `count_free()`.

## Testing

*Unit testing*.  A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the querier and test it as a whole.
In each case, examine the output carefully to be sure the format and output is correct.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the querier with a `page_directory` that points to a page directory not created by crawler.

3. Test the querier with a `index_file` that points to a non-readable file.

4. Run querier with fuzzquery on letters depth 2, toscrape depth 1, and wiki depth 1. Verify the output with the output produced by the querier in /thayerfs/courses/22spring/cosc050/workspace/ or by checking with the correct index.

5. Test valgrind with fuzzquery on letters depth 2