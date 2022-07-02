# indexer
## Ashley Liang, CS50 Spring 2022

### Overview

The `indexer` reads the documents in the page_dir output by the crawler, builds an inverted index mapping from words to documents, and writes that index to a file. It’s called an inverted index because it maps from words to documents.

### Assumptions

- `page_dir` has files name 1,2,3... without gaps
- The content of files in `page_dir` follow the format as defined in the spec

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation
* `indextest.c` - unit tester
* `testing.sh` - testing driver
* `testing.out` - testing results 

### Compilation

To compile, simply `make all`. To clean, simply `make clean`

### Testing

`indextest.c` tests the `index_load` and `index_save` in the index module by seeing if taking an indexfile, converting it to an index, and converting that back to an indexfile results in the same file content.

The `testing.sh` program includes hard-coded test cases to test command-line argument parsing and verify that the created indices are correct. To test, redirecting the output to testing.out, simply `make test &> testing.out`.

These are the following test cases:
1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the indexer with a `page_directory` that points to a page directory not created by crawler.

3. Test the indexer with a `index_file` that points to a non-writable file.

4. Run indexer on letters at depth 0, 2, 4, 5. Run index and verify that it matches expectations with the cs50 sample output. Run `index_load` to create a copy and verify that there are no differences.

5. Repeat with the Wikipedia playground at depths 0, 1, and 2.

To test with valgrind, `make valgrind`.