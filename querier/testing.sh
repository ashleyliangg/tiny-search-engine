#!/bin/bash
#
# testing.sh - performs testing on querier
#
# Output: the output of various test inputs and edge cases of querier in testing.out
#
# Usage: make test &> testing.out using the Makefile, or can just run and print to
# stdout using make test or ./testing.sh. Might need to chmod a+x testing.sh before running.
#
# Ashley Liang, CS50 Spring 2022

prog="./querier"

# testing bad arguments
echo "testing arguments"
echo "1. not enough parameters"
"$prog"
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "2. not directory made with crawler"
"$prog" ../libcs50 /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "3. not readable file"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 notindexfile
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

# test with fuzzquery
echo "testing with letters depth 2"
./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2 10 1 | \
./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 \
/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2

echo "testing with toscrapte depth 1"
./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1 5 2 | \
./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-depth-1 \
/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1

echo "testing with wikipedia depth 1"
./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-1 5 1 | \
./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-depth-1 \
/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-1

echo "all test cases ran correctly"
exit 0