# crawler
## Ashley Liang, CS50 Spring 2022

The `crawler` crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded href URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to _maxDepth_ number of hops from the _seedURL_.

### Definitions

Internal URL: Only those beginning http://cs50tse.cs.dartmouth.edu.

### Implementation

Details of the implementation of crawler can be found in IMPLEMENTATION.md

### Assumptions

The `pageDirectory` does not contain any files whose name is an integer (i.e., 1, 2, ...). `pageDirectory` also must be an existing, writable directory. If `pageDirectory` does not exist in the current directory, you must provide the path to the directory.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the implementation
* `testing.sh` - test driver
* `testing.out` - testing results

### Compilation/Running the Program

To compile, simply `make`. The program only takes a directory that already exists and is writable. `make valgrind` creates the directory _testdir_ for the user to test memory of the hardcoded case. `make valgrind` also creates directories for each of the playground test cases. However, if the user runs the program in a different way, they must create their own directory first before running the program.

### Testing

The `testing.sh` program includes hard-coded test cases. These are the following test cases:
1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Test the crawler with a seedURL that points to a non-existent server.

3. Test the crawler with a seedURL that points to a non-internal server.

4. Test the crawler with a seedURL that points to a valid server but non-existent page.

5. Crawl a simple, closed set of cross-linked web pages (letters playground). Ensure that some page(s) are mentioned multiple times within a page, and multiple times across the set of pages. Ensure there is a loop (a cycle in the graph of pages). In such a little site, you know exactly what set of pages should be crawled, at what depths, and you know where your program might trip up.

6. Point the crawler at a page in that site (letters playground), and explore at depths 0, 1, 2, 3, 4, 5. Verify that the files created match expectations.

7. Repeat with a different seed page in that same site. If the site is indeed a graph, with cycles, there should be several interesting starting points.

8. Point the crawler at our Wikipedia playground. Explore at depths 0, 1, 2. (It takes a long time to run at depth 2 or higher!) Verify that the files created match expectations.

To test, redirecting the output to testing.out, simply `make test &> testing.out`.

To test with valgrind, `make valgrind`.