## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test the indexer

echo -e "Testing for indexer.c\n"

echo "=========="
echo -e "Test with no arguments\n"
echo -e "Error in stderr: Invalid number of arguments!\n"
./indexer

echo -e "Test with 1 argument"
echo -e "Error in stderr: Invalid number of arguments!\n"
./indexer ../data/letters-10

echo -e "Test with more than 3 arguments"
echo -e "Error in stderr: Invalid number of arguments!\n"
./indexer ../data/letters-10 ../data/letters10.index 10

echo -e "Test with non-existent directory"
echo -e "Error in stderr: Invalid/Non-existent page directory.\n"
./indexer ../dummydirectory/letters-10 ../data/letters-10/letters-10.index

echo -e "Test with non-crawler-produced directory"
echo -e "Error in stderr: Not a valid crawler-produced directory!\n"
./indexer ../common ../data/letters-10/letters-10.index

echo -e "Test with invalid (non-existent) path"
echo -e "Error in stderr: Failed to create/open target directory.\n"
./indexer ../data/letters-10 ../dummydirectory/letters-10.index

echo -e "Test indexer with valid inputs: Index letters at depth 10 and 2 and wikipedia at depth 1."
echo -e "./indexer ../data/letters-10 ../data/letters-10/letters-10.index"
echo -e "./indexer ../data/letters-2 ../data/letters-2/letters-2.index"
echo -e "./indexer ../data/wikipedia-1 ../data/wikipedia-1/wikipedia-1.index\n"
./indexer ../data/letters-10 ../data/letters-10/letters-10.index
./indexer ../data/letters-2 ../data/letters-2/letters-2.index
./indexer ../data/wikipedia-1 ../data/wikipedia-1/wikipedia-1.index

echo -e "Test indextest valid inputs: Build inverted index from output of indexers"
echo -e "./indextest ../data/letters-10/letters-10.index ../data/letters-10/letters-10.indextest"
echo -e "./indextest ../data/letters-2/letters-2.index ../data/letters-2/letters-2.indextest"
echo -e "./indextest ../data/wikipedia-1/wikipedia-1.index ../data/wikipedia-1/wikipedia-1.indextest"

./indextest ../data/letters-10/letters-10.index ../data/letters-10/letters-10.indextest
./indextest ../data/letters-2/letters-2.index ../data/letters-2/letters-2.indextest
./indextest ../data/wikipedia-1/wikipedia-1.index ../data/wikipedia-1/wikipedia-1.indextest