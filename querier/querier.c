/*
 * querier.c - reads the pagedirectory made by Crawler and indexer made by Indexer to
 * produce ranked list of webpages answering the user's search query
 * 
 * input: a crawler-made page directory and a readable index-made indexer
 * output: ranked list of webpages to stdout
 * 
 * Ashley Liang, CS50 Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/memory.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/file.h"

/******** local data types *******/
typedef struct counter_pair {       //to hold a pair of counters
	counters_t *result;
	counters_t *other;
} counter_pair_t;

typedef struct doc {        //to hold a document's ID and score
    int docID;
    int score;
} doc_t;

typedef struct array_doc {      //to hold an array of docs
    doc_t **docs;
    int num_inserted;
} array_doc_t;


/******* local function prototypes *******/
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


/*********** main ***********/
int main(int argc, char *argv[]) 
{
    //create variable for the parameters from command line
    char *page_dir = NULL;
    char *index_file = NULL;
    
    //validating arguments, exits if error
    validate_args(argc, argv, &page_dir, &index_file);

    //create index from the index_file
    index_t *index = index_load(index_file);

    char *line;
    prompt();
    //reads from stdin until EOF
    while ((line = freadlinep(stdin)) != NULL) {
        //keep track of number of words being inserted
        int num_words = 0;

        //create an array of words, increment num_words
        char **word_array = read_input(line, &num_words);

        //validate query, prompt new query if any errors
        if (!validate_query(word_array, num_words))  {
            prompt();
            continue;
        }

        //prints out clean, normalized query
        printf("Query:");
        for (int i = 0; i < num_words; i++) {
            printf(" %s", word_array[i]);
        }
        printf("\n");

        //get final counters of docID and score
        counters_t *final_count = process_query(word_array, num_words, index);

        //rank documents by score and print the result
        result_rank(final_count, page_dir);

        counters_delete(final_count);
        count_free(line);

        //print prompt for next line
        prompt();
    }

    index_delete(index);

    return 0;
}
/*********** prompt **********/
/* print a prompt iff stdin is a tty (terminal) */
static void prompt(void)
{
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/*********** validate_args **********/
/* Parse the command-line args, filling in parameters */
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

    //check if indexfile is the pathname of readable file
    *index_file = argv[2];
    FILE *fp;
    if ((fp = fopen(*index_file, "r")) == NULL) {
        fprintf(stderr, "%s: index_file %s must be readable\n", program, *index_file);
        exit(3);
    }
    else {
        fclose(fp);
    }
}

/*********** read_input **********/
/* splits a line of words into an array of words */
static char **read_input(char *line, int* num_words)
{
    //determine the max number of words that can appear in the line
    int line_length = strlen(line);
    int max_words;
    if (line_length % 2 == 0) {
        max_words = line_length / 2;
    } else {
        max_words = (line_length / 2) + 1;
    }

    //allocate memory for the array using the max number of words possible in the array
    char **word_list = count_malloc_assert(sizeof(char*) * max_words, "error creating string array");

    //pointers to the begining and end of a word
    char *begin = line;
    char *end = line;
    int word_index = 0;
    //bool to check if a word has started or not
    bool start_word = false;
    while (*begin != '\0' && *end != '\0') {
        //check if character is non-alphabet, non-space character
        if (!(isalpha(*begin) || isspace(*begin))) {
            fprintf(stderr, "Error: bad character '%c' in query\n", *begin);
            return NULL;
        }
        if (!(isalpha(*end) || isspace(*end))) {
            fprintf(stderr, "Error: bad character '%c' in query\n", *end);
            return NULL;
        }
        //increment begin until reaches letter, then starts new word
        if (!start_word) {
            if (isalpha(*begin)) {
                end = begin;
                start_word = true;
            }
            else {
                begin++;
            }
        }
        //else if word already started
        else {
            //increment until end points to a space
            if (isspace(*end)) {
                *end = '\0';
                end++;
                //normalize word before put into word_list
                normalize_word(begin);
                word_list[word_index++] = begin;
                *num_words = *num_words + 1;
                begin = end;

                //end of word, so no longer starting word
                start_word = false;
            }
            else {
                end++;
            }
        }
        
    }
    //make sure the last word gets added to the array
    if (isalpha(*begin)) {
        normalize_word(begin);
        word_list[word_index++] = begin;
        *num_words = *num_words + 1;
    }

    return word_list;
}

/*********** validate_query **********/
/* checks to see if query is a valid query */
static bool validate_query(char** word_array, int num_words)
{
    //check if no input or bad character (word_array is NULL if bad character)
    if (num_words == 0 || word_array == NULL) {
        return false;
    }

    //check if beginning of query is operator
    if (is_operator(word_array[0])) {
        fprintf(stderr, "Error: '%s' cannot be first\n", word_array[0]);
        return false;
    }

    //check if end of query is operator
    if (is_operator(word_array[num_words - 1])) {
        fprintf(stderr, "Error: '%s' cannot be last\n", word_array[num_words - 1]);
        return false;
    }
    

    //loop through query if there are more than 3 words bc already checked begin/end
    //so will only run into problems with 4+ words if multiple operators
    if (num_words > 3) {
        for (int i = 1; i < num_words - 2; i++) {
            //if is operator, check if next word is operator
            if (is_operator(word_array[i])) {
                if (is_operator(word_array[i+1])) {
                    fprintf(stderr, "Error: cannot have consecutive operators\n");
                    return false;
                }
            }
        }
    }
    
    return true;
}

/*********** is_operator **********/
/* checks if the word is an operator */
static bool is_operator(char *word) 
{
    if (strcmp(word, "and") == 0 || strcmp(word, "or") == 0) {
        return true;
    }
    return false;
}

/*********** process_query **********/
/* creates a counters of the doc IDs and score by combining the
 * results of each word's index depending on the operators in the
 * query
 */
static counters_t *process_query(char **word_array, int num_words, index_t *idx)
{
    counters_t *temp = NULL;
    counters_t *result = counters_new();

    for (int i = 0; i < num_words; i++) {
        //if and, continue
        if (strcmp(word_array[i], "and") == 0) {
            continue;
        }
        //if or, add the temp to the result and reset temp
        if (strcmp(word_array[i], "or") == 0) {
            counters_union(result, temp);
            counters_delete(temp);
            temp = NULL;
        }
        //if word, then add to temp
        else {
            //index_find(word)
            counters_t *word_count = index_find(idx, word_array[i]);

            //update temp (if temp is empty, union, else, intersection)
            if (temp == NULL) {
                temp = counters_new();
                counters_union(temp, word_count);
            }
            else {
                counters_intersect(temp, word_count);
            }
            //merge temp with result if it's the last word in the query
            if (i == num_words - 1) {
                counters_union(result, temp);
                counters_delete(temp);
            }
        }
    }

    //free array
    free(word_array);
    return result;
}

/*********** counters_intersect **********/
/* function that deals with intersecting two counters */
static void counters_intersect(counters_t* temp, counters_t* other)
{
	counter_pair_t count_pair = {temp, other}; 
	counters_iterate(temp, &count_pair, intersect_helper);
}

/*********** intersect_helper **********/
/* itemfunc function for counters_intersect's counters_iterate
 * that finds the minimum of the key's count between two counters
 */
static void intersect_helper(void *arg, const int key, const int count)
{
	counter_pair_t *count_pair = arg; 

    //set the count to the minimum between the result and the other's count
	counters_set(count_pair->result, key, min(count, counters_get(count_pair->other, key)));
}

/*********** counters_union **********/
/* function that deals with unioning two counters */
static void counters_union(counters_t* result, counters_t* other)
{
    counter_pair_t count_pair = {result, other};

    //iterate through other to add other's counts to new
    counters_iterate(other, &count_pair, union_helper);
}

/*********** union_helper **********/
/* itemfunc function for counters_union's counters_iterate
 * that sets the count to each key's total count
 */
static void union_helper(void *arg, const int key, const int count)
{
    counter_pair_t *count_pair = arg;

    //new count is the sum of the two counts
    int new_count = counters_get(count_pair->result, key) + counters_get(count_pair->other, key);
    counters_set(count_pair->result, key, new_count);
}

/*********** result_rank **********/
/* creates a sorted array of documents (results) and
 * prints out the results 
 */
static void result_rank(counters_t *result, char *page_directory)
{
    //iterate through the counters to get the number of documents
    int num_docs = 0;
    counters_iterate(result, &num_docs, counters_key_count);
    if (num_docs == 0) {
        printf("No documents match.\n");
    }
    else {
         //create an array of doc pointers
        doc_t **doc_array = count_calloc_assert(num_docs, sizeof(doc_t*), "error creating doc array");
        //initally 0 inserted
        int num_inserted = 0;
        array_doc_t array_struct = {doc_array, num_inserted};
        //insert/sort into array if the score is more than 0
        counters_iterate(result, &array_struct, array_add_doc);

        //print the documents in order (sorted array)
        printf("Matches %d documents (ranked):\n", num_docs);
        for (int i = 0; i < num_docs; i++) {
            int id = array_struct.docs[i]->docID;
            int score = array_struct.docs[i]->score;

            FILE *fp;
            //buffer for filename
            char filename[strlen(page_directory) + 10];
            sprintf(filename, "%s/%d", page_directory, id);
            if ((fp = fopen(filename, "r")) != NULL) {
                char *url = freadlinep(fp);
                printf("score %*d doc %*d: %s\n", 4, score, 4, id, url);
                free(url);
                fclose(fp);
            }
        }

        //delete the array of docs
        for (int i = 0; i < num_docs; i++) {
            count_free(array_struct.docs[i]);
        }
        count_free(doc_array);
    }
   
}

/*********** counters_key_count ***********/
/* counts how many docIDs (with a score over 0) there are */
static void counters_key_count(void *arg, const int key, const int count)
{
    int *num_docs = arg;

    //increment number of docs if the count is over 0
    if (key > 0 && count > 0) {
        (*num_docs)++;
    }
}

/*********** array_add_doc ***********/
/* creates a doc for each key/count pair (docID/score pair)
 * and inserts it into a sorted array
 */
static void array_add_doc(void *arg, const int key, const int count)
{
    // static int index = 0;
    array_doc_t *array_doc = arg;

    if (count > 0) {
        //create new doc struct for the key/count
        doc_t *new_doc = count_malloc_assert(sizeof(doc_t), "error allocating memory for doc");
        new_doc->docID = key;
        new_doc->score = count;

        int score_cmp, i;
        score_cmp = new_doc->score;
        //getting the index of the last item in the array
        i = array_doc->num_inserted - 1;
        //move all docs with a smaller score 1 index to the right
        while (i >= 0 && score_cmp > array_doc->docs[i]->score) {
            array_doc->docs[i + 1] = array_doc->docs[i];
            i = i - 1;
        }
        //insert into right place so it stays sorted
        array_doc->docs[i + 1] = new_doc;
        array_doc->num_inserted++;
    }
}