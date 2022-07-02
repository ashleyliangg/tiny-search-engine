#!/bin/bash
#
# testing.sh - performs testing on indexer
#
# Output: the output of various test cases and edge cases of indexer in testing.out
# the index files/sorted are created in this directory
#
# Usage: make test &> testing.out using the Makefile, or can just run and print to
# stdout using make test or ./testing.sh. Might need to chmod a+x testing.sh before running.
#
# Ashley Liang, CS50 Spring 2022

prog="./indexer"

# testing bad arguments
echo "testing arguments"
echo "1. not enough parameters"
"$prog"
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "2. not directory made with crawler"
"$prog" ../libcs50 testfile
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

echo "3. not writable file"
touch badfile
chmod -wx badfile
"$prog" ../indexer badfile
if [ $? != 0 ]; then
    echo "exited with non-zero correctly"
fi

#creating/verifying indices with letters playground (depth 0, 2, 4, 5)
echo "created index from crawler-created directory of letters depth 0"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-0 letters0index
sort letters0index > letters0sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-0 > letters0ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q letters0sorted letters0ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 1
fi

echo "create new index from index file and compare"
./indextest letters0index letters0cpy
sort letters0cpy > srtletters0cpy
difference=$(diff -q letters0sorted srtletters0cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 2
fi


echo "created index from crawler-created directory of letters depth 2"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-2 letters2index
sort letters2index > letters2sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2 > letters2ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q letters2sorted letters2ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 3
fi

echo "create new index from index file and compare"
./indextest letters2index letters2cpy
sort letters2cpy > srtletters2cpy
difference=$(diff -q letters2sorted srtletters2cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 4
fi

echo "created index from crawler-created directory of letters depth 4"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-4 letters4index
sort letters4index > letters4sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-4 > letters4ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q letters4sorted letters4ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 5
fi

echo "create new index from index file and compare"
./indextest letters4index letters4cpy
sort letters4cpy > srtletters4cpy
difference=$(diff -q letters4sorted srtletters4cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 6
fi

echo "created index from crawler-created directory of letters depth 5"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-depth-5 letters5index
sort letters5index > letters5sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-5 > letters5ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q letters5sorted letters5ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 7
fi

echo "create new index from index file and compare"
./indextest letters5index letters5cpy
sort letters5cpy > srtletters5cpy
difference=$(diff -q letters5sorted srtletters5cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 8
fi

#creating/verifying indices with wikipedia playground (depth 0 and 1)
echo "created index from crawler-created directory of wikipedia depth 0"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-depth-0 wiki0index
sort wiki0index > wiki0sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-0 > wiki0ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q wiki0sorted wiki0ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 5
fi

echo "create new index from index file and compare"
./indextest wiki0index wiki0cpy
sort wiki0cpy > srtwiki0cpy
difference=$(diff -q wiki0sorted srtwiki0cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 6
fi

echo "created index from crawler-created directory of wikipedia depth 1"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-depth-1 wiki1index
sort wiki1index > wiki1sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-1 > wiki1ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q wiki1sorted wiki1ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 5
fi

echo "create new index from index file and compare"
./indextest wiki1index wiki1cpy
sort wiki1cpy > srtwiki1cpy
difference=$(diff -q wiki1sorted srtwiki1cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 6
fi

echo "created index from crawler-created directory of wikipedia depth 2"
"$prog" /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-depth-2 wiki2index
sort wiki2index > wiki2sorted
sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-2 > wiki2ansr
echo "Comparing sorted index output with sorted example output"
difference=$(diff -q wiki2sorted wiki2ansr)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 5
fi

echo "create new index from index file and compare"
./indextest wiki2index wiki2cpy
sort wiki2cpy > srtwiki2cpy
difference=$(diff -q wiki2sorted srtwiki2cpy)
if [ -z "$difference" ]; then
    echo "No differences"
else
    echo "$difference"
    exit 6
fi

echo "all test cases ran correctly"
exit 0