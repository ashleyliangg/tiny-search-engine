# TSE Indexer Design Spec
## Ashley Liang, CS50

`Indexer` reads the document files produced by the TSE crawler, builds an _index_ mapping words to webpage docIDs, and writes that _index_ to a _file_.

## Overview of the Design Spec
* User interface
* Inputs and Outputs
* Functional decomposition into modules
* Pseudo code for logic/algorithmic flow
* Dataflow through modules
* Major data structures
* Testing plan

### User interface

The indexer's interface with the user is on the command-line; it must always have two arguments.

```
./indexer page_directory index_filename
```

For example:

``` bash
$ ./indexer wiki0 wiki0index
```

### Inputs and outputs

Input: The only inputs are command-line parameters; see the User Interface above.

Output: We create an index that maps words to webpage (identified by docIDs) by going through each file in the page directory given. We save the index into the indexfile given. Within the indexfile, the output is the following for each word:
```
word docID count docID count [continue for as many docID/count pairs as needed]
```

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which creates the `page_dir` and `index_file` strings
 2. *validate_args*, which checks each argument and exits non-zero if the arguments are not valid
 3. *index_build*, which is the overall function that builds the `index`
 4. *index_page*, which parses through the webpage words and adds that word/webpage count to the `index`
 4. *file_to_web*, which takes the file contents and converts it into a `webpage`
 5. *index_saver*, which saves an index into a file
 6. *index_load*, which takes the contents of an `index_file` and converts it into an `index`
 
And some helper modules that provide data structures:

 1. *index*: a hashtable variant with the word as the key and a counters of docIDs as the item

### Pseudo code for logic/algorithmic flow

The index will run as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters
3. creates a new *index*
4. open each file in the *page_directory* starting from docID = 1 (to make filename *page_directory/1*) by checking if the file is readable (aka exists)
	5. create a new webpage from the URL, depth, and HTML (the format of the file are explained more clearly in `REQUIREMENTS.md`)
	6. close the file
	7. if the webpage isn't NULL, go through each word in the webpage, freeing each word after
		8. with each word, if the word is at least 3 characters, normalize the word and check if the word is in the index. If it is, add the webpage's docID to that word's counters. If the word is not, create a new counters for that word, add the webpage's docID to the counters and insert te counters into index.
		9. free the webpage
	10. increment the docID and create a new filename from that
11. return the index
12. save the index to the given index_filename by iterating through the index and printing out the components in the format mentioned in "Inputs and Outputs"
13. delete the index


**normalize the word** means to convert it to lowercase
For example:
`PlaYground`
becomes
`playground`.


### Dataflow through modules

 1. *main* creates variables and passes them to validate_args.
 2. *validate_args* parses the arguments and sets the variables to the correct command-line arguments, which are passed back to main
 3. *indexer* creates an index and redirects the file pointer to each file it opens, passing the file pointer to file_to_web
 4. *file_to_web* takes in the file pointer and returns a webpage
 5. *index_page* takes in the address of the index, the webpage, and the current docID, and adds to the index
 6. *indexer* returns the index to main
 7. *index_save* takes the index and saves the contents to index_filename

### Major data structures

`querier` creates a few data structures to r

### Testing plan

*Unit testing*.  A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output indexfile carefully to be sure the format and output is correct.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the indexer with a `page_directory` that points to a page directory not created by crawler.

3. Test the indexer with a `index_file` that points to a non-writable file.

4. Run indexer on letters at depth 0, 2, 4, 5. Run index and verify that it matches expectations. Run index_load to create a copy and verify that it is the same.

5. Repeat with the Wikipedia playground at depths 0, 1, and 2.