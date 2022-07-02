# TSE Indexer Design Spec
## Ashley Liang, CS50

`querier` reads the *index_filename* produced by the `indexer` and the page files in the *page_directory* produced by the `crawler` to produce a ranked list of webpages answering the user's written query.

## Overview of the Design Spec
* User interface
* Inputs and Outputs
* Functional decomposition into modules
* Pseudo code for logic/algorithmic flow
* Dataflow through modules
* Major data structures
* Testing plan

### User interface

The querier's interface with the user is on the command-line; it must always have two arguments.

```
./querier page_directory index_filename
```
where *page_directory* is the pathname of a directory produced by the Crawler, and
*index_filename* is the pathname of a file produced by the Indexer.

For example:
``` bash
$ ./querier wiki0 wiki0index
```

The `querier` then prompts continuous user input for the query (one search query, optionally separated by AND or OR, per line) until the user provides an EOF (usually with ^D).

### Inputs and outputs

Input: The inputs for `querier` are command-line parameters and the query search inputs; see the User Interface above.

Output: `querier` prints out a ranked list of webpages (from highest to lowest score) that matches the search query.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which calls `validate_args`, creates an index from the `index_filename`, and processes each line of stdin until EOF by calling the other functions. At the end, *main* returns 0.
 2. *prompt*, which prints prompt "Query?" if stdin is a tty
 3. *validate_args*, which checks that 2 arguments are given on the command-line. It then checks if `page_directory` is a directory produced by crawler and if `index_filename` is the pathname of a readable file. If any of those above conditions are not satisfied, the program prints out an error message and exits non-zero.
 4. *read_input*, which parses the query line and creates an array that stores the words in the query, returning that array
 4. *validate_query*, which checks if the query begins or ends with an operator (AND or OR) or has consecutive operators. If the query does, the program prints an error statement and prompts for a new query.
 5. *is_query*, which is a small function that checks if the word is an operator (AND or OR)
 6. *process_query*, which builds a final counterset of docIDs and that ID's score depending on the operators (AND vs OR) in that query. It returns the final counterset.
 7. *result_rank*, which ranks the counterset that *process_query* returns and prints out the results.
 
 And some helper modules that provide data structures:

 1. *index* from indexer that holds the docID and count for each word as *counters*

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. execute from a command line as shown in the first part of User Interface
2. parse the command line, validate parameters
3. creates a new *index*
4. while line = reading from stdin isn't NULL:
	5. parse the line:
		6. create an array of strings. the size of the array is set to the max number of words possible in the line (if even: line_length / 2, if odd: line_length / 2 + 1).
		7. create a pointer to the start of the word and end of the word and initialize it to the beginning of the line
		8. iterate through the line to insert the words into the array
			9. check if the pointers are letters or spaces. if it's not, return NULL.
			10. if the end pointer reaches a space, set that character to the end of the string and add that word to the array
			11. return string array
	12. if the number of words in the string array is 0 or the string array is NULL, continue.
	13. print the string array as a clean query in the format "Query: "
	14. validate the query:
		15. check if the first word of the array is an operator, return false if the case
		16. check if the last word of the array is an operator, return false if the case
		17. check if there are consecutive operators in the query, return false if the case
		18. otherwise return true
	19. if validating the query returns false, continue
	20. create a counters from the word array:
		21. create a temporary counters, initialize it to NULL. create a result counters, and initialize using counters_new
		22. if the word is "and", continue. If the word is "or", merge the result and the temporary counters by iterating through the temp and adding the total count of each key to the result. Then, set the temp to NULL. Otherwise, get the counters for that word using index_find(). If temp is NULL, merge the counters with temp. Otherwise, intersect the temp and word_count by finding the minimum count for each key in temp and setting the count to that. If the word is the last word of the query, merge the temp with result.
	23. return the result counters
	24. rank the documents by score:
		25. iterate through the result counters (ignoring any counters with a score of 0) to determine the size of the ranked array
		26. if counters is empty, then print out "No documents match."
		27. else, create an array and iterate through the counters again to insert into the sorted arrayin descending order:
			28. if count is more than 0, create a new doc pointer, allocating memory for it
			29. starting from the last doc in the array (number of docs inserted), if that doc's score is lower than the doc being inserted, move it one to the right.
			30. insert the doc in the sorted place and increment the number of docs inserted
		31. for each doc in the sorted array, get the URL by opening the file with that ID and reading the first line.
		32. print the score, docID, and URL for each doc	
33. delete the index
34. return 0


**normalize the word** means to convert it to lowercase
For example:
`PlaYground`
becomes
`playground`.

**operator** means "and" or "or"

### Dataflow through modules

 1. *main* creates the *page_directory* and *index_file* variables and passes it to validate args. It then creates an index. While the line from stdin isn't EOF, *main* gives *read_input* the line and gets a word array. It passes the word array to *validate_query* and then *process_query*, which then gets a counters. *main* passes the counters to *result_rank*. *main* deletes the index after stdin reaches EOF.
 2. *prompt* checks if stdin is a tty and prints a prompt if there is.
 3. *validate_args* takes `page_directory` and `index_filename` variables as the input and initializes them to the command-line arguments. If the variables are verified, the variables are passed back to the main function.
 4. *read_input* takes in the string 'line' from stdin in *main* and the address to an integer keeping track of the number of words added to the array. It returns an array of strings. The array of strings is just the query line broken up into individual words.
 4. *validate_query* takes in the array of strings and the number of words in the array. It returns a boolean indicating whether it has passed the query checks or not.
 5. *process_query* takes in the word array, number of words in the array, and index. It creates a temporary and final counters, which are used to separate the and sequences from the or sequences. At the end, it deletes the temporary counters returns the final counters.
 6. *result_rank* takes in the counters and the `page_directory` and creates a ranked array of documents holding the docID and score. It deletes documents and array at the end.

### Major data structures

`querier` contains these local data types to more conveniently hold and pass data through functions:
1. *counter_pair* that holds a 'result' counters and an 'other' counters
2. *doc* that holds the docID and score for a document
3. *array_doc* that holds an array of *docs* and the number of *docs* inserted into the array

Other helper module provide data structures:

 1. *index* of (word, counterset(webpage's docID, count))

### Testing plan

*Unit testing*.  A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the querier and test it as a whole.
In each case, examine the output carefully to be sure the format and output is correct.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the querier with a `page_directory` that points to a page directory not created by crawler.

3. Test the querier with a `index_file` that points to a non-readable file.

4. Run querier with fuzzquery on letters depth 2, toscrape depth 1, and wiki depth 1. Verify the output with the output produced by the querier in /thayerfs/courses/22spring/cosc050/workspace/ or by checking with the correct index.

5. Test valgrind with fuzzquery on letters depth 2