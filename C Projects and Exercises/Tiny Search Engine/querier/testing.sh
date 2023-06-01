## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test the querier

# TESTS FOR INVALID NUMBER OF ARGUMENTS --------------------------------------------------

# No argument
./querier

# One argument
./querier ../data/letters-2 

# Three arguments - way too many
./querier ../data/letters-2 ../data/letters-2/letters-2.index make

# TESTS FOR INVALID FILE PATHS -----------------------------------------------------------

# Non-existent pageDirectory
./querier ../data/dummyfile ../data/letters-2/letters-2.index

# pageDirectory exists but not produced by crawler
./querier ../common ../data/letters-2/letters-2.index 

# Non-existent indexFile
./querier ../data/letters-2 ../data/letters-2/dummyfile

# TESTS FOR INVALID OPERATOR POSITIONS -----------------------------------------------------

# 'and' and 'or' at beginning
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "and home"
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "or school"

# 'and' and 'or' at the end
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home or school and"
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home and school or"

# 'and' and 'or' next to each other
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home or and school"
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home and or school"

# TESTS FOR INVALID CHARACTERS ------------------------------------------------------------

./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home or school,"
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "home ! and school"

# TESTS FOR BLANK SPACES AND CAPITAL LETTERS IN QUERY -------------------------------------
# To make sure normalization and tokenization works as intended

# Leading and trailing blank spaces in query
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "    home     or     for       "

# Capital letters
./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "hOmE OR FOR"

# TESTS FOR VALID CASES -------------------------------------------------------------------
# Just one word
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "for"

# Just one word (but doesnt exist)
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "chipnguyen"

# 2 words in an "and" sequence, 1 doesn't exist
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "chipnguyen and for"

# 2 words
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "for and search"
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "for or search"

# More than 3 words
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "search for home"
./querier ../data/letters-10 ../data/letters-10/letters-10.index <<< "search and for home"

# With fuzzquery
./fuzzquery ../data/wikipedia-1/wikipedia-1.index 10 1 | ./querier ../data/wikipedia-1 ../data/wikipedia-1/wikipedia-1.index


# VALGRIND TEST FOR MEMORY LEAK -------------------------------------------------------------------

valgrind ./querier ../data/letters-2 ../data/letters-2/letters-2.index <<< "search or home"