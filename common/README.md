# common
## Ashley Liang, CS50 Spring 2022

### Overview

The common sub-directory in the tse lab includes modules/functions that will be shared by all parts of the tiny search engine (crawler, indexer, querier). It creates a common library that can be accessed. This directory allows for uniformity across the lab sections.

### Assumptions

No assumptions beyond those that are clear from the spec. Each function's specific assumptions can be found in `pagedir.h`, `index.h`, and `word.c`.

### Files

* `Makefile` - compilation procedure
* `pagedir.h` - the interface for pagedir
* `pagedir.c` - the implementation for pagedir
* `index.h` - the interface for index
* `index.c` - the implementation for index
* `word.h` - the interface for word
* `word.c` - the implementation for word

### Compilation

To compile, simply `make`.