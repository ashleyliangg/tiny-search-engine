# querier
## Ashley Liang, CS50 Spring 2022

### Overview

The `querier` reads the *index_filename* produced by the `indexer` and the page files in the *page_directory* produced by the `crawler` to produce a ranked list of webpages answering the user's written query.

### Assumptions

* `page_directory` has files named 1, 2, 3, ..., without gaps.
* The content of files in `page_directory` follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.
* The content of the file named by `index_filename` follows our [Index file format]({{site.labs}}/lab5/REQUIREMENTS.html#index-file-format); thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation for querier
* `fuzzquery.c` - the implementation to create random input for querier testing
* `testing.sh` - testing driver
* `testing.out` - testing results 

### Compilation

To compile, simply `make all`. To clean, simply `make clean`

### Testing

The `testing.sh` program first runs some hard-coded test cases to test command-line argument parsing. It then tests random query inputs using fuzzquery. To test, redirecting the output to testing.out, simply `make test &> testing.out`.

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly. First, test the program with an incorrect number of arguments.

2. Test the querier with a `page_directory` that points to a page directory not created by crawler.

3. Test the querier with a `index_file` that points to a non-readable file.

4. Run querier with fuzzquery on letters depth 2, toscrape depth 1, and wiki depth 1. Verify the output with the output produced by the querier in /thayerfs/courses/22spring/cosc050/workspace/ or by checking with the correct index.

5. Test valgrind with fuzzquery on letters depth 2

To test with valgrind, `make valgrind`.