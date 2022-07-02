#!/bin/bash
#
# testing.sh - performs testing on crawler
#
# Output: the output of various test cases and edge cases of crawler in testing.out
# the content of the webpages are printed in files in their respective directories
#
# Usage: make test &> testing.out using the Makefile, or can just run and print to
# stdout using make test or ./testing.sh. Might need to chmod a+x testing.sh before running.
#
# Ashley Liang, CS50 Spring 2022

prog="./crawler"

# testing bad arguments
echo "testing arguments"
echo "1. only 2 parameters"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 27

if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "2. not internal URL"
"$prog" http://www.youtube.com testdir 2

if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "3. not a writable directory"
echo "files and directorys in current directory"
ls
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html not_dir 3

if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "4. maxDepth not in range"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testdir 20

if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "5. non-existent server"
"$prog" http://www.ajfdk testdir 3

if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "6. non-existent page on valid server"
"$prog" http://cs50tse.cs.dartmouth.edu/nonexistent.html testdir 2

if [ $? == 0 ]; then
    echo "printed error statement and continued on"
fi

#testing letter playground at depths 0-5 and from a page E
echo "testing letter playground at depth 0"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index0 0

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 1
else 
    echo "test ran correctly"
fi

echo "testing letter playground at depth 1"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index1 1

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 1
else 
    echo "test ran correctly"
fi

echo "testing letter playground at depth 2"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index2 2

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 2
else 
    echo "test ran correctly"
fi

echo "testing letter playground at depth 3"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index3 3

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 3
else 
    echo "test ran correctly"
fi

echo "testing letter playground at depth 4"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index4 4

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 4
else 
    echo "test ran correctly"
fi

echo "testing letter playground at depth 5"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/index.html index5 5

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 5
else 
    echo "test ran correctly"
fi

echo "testing letter playground starting from E"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/letters/E.html indexE 5

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 6
else 
    echo "test ran correctly"
fi

#testing other playgrounds: toscrape, wikipedia 0-2, wikipedia from Dartmouth
echo "testing toscrape at depth 1"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html toscrape 1

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 7
else 
    echo "test ran correctly"
fi

echo "testing wikipedia at depth 0"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki0 0

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 8
else 
    echo "test ran correctly"
fi

echo "testing wikipedia at depth 1"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki1 1

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 8
else 
    echo "test ran correctly"
fi

echo "testing wikipedia at depth 2"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki2 2

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 8
else 
    echo "test ran correctly"
fi

echo "testing wikipedia starting from Dartmouth"
"$prog" http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Dartmouth_College.html wikidart 1

if [ $? != 0 ]; then
    echo >&2 "test ran incorrectly"
    exit 9
else 
    echo "test ran correctly"
fi

echo "all test cases ran correctly"
exit 0