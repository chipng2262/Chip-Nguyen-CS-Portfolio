## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test chill.c

echo -e "Testing for crawler.c\n"

echo -e "Make directory "data" with subdirectories to store testing outputs.\n"
mkdir ../data
mkdir ../data/letters-0
mkdir ../data/letters-2
mkdir ../data/letters-10
mkdir ../data/toscrape-0
mkdir ../data/toscrape-1
mkdir ../data/wikipedia-0
mkdir ../data/wikipedia-1

echo "=========="
echo -e "Testing non-valid arguments. make test would exit with error\n"

echo "Test with invalid number of arguments:"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2 do
echo -e "Error: Invalid number of arguments. Make sure there are only 3.\n"

echo "Test with an invalid (null) directory:"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html NULL 2 
echo -e "Error: Invalid page directory.\n"

echo "Test with an invalid (non-existent) directory:"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/cool 2 
echo -e "Error: Invalid page directory.\n"

echo "Test with invalid depth (>10):"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 18
echo -e "Error: Depth out of range.\n"

echo "Test with invalid depth (depth is passed in as a string):"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 string
echo -e "Error: Please input a number for depth instead.\n"

echo "=========="
echo -e "Testing valid cases.\n"

echo "Crawl http://cs50tse.cs.dartmouth.edu/tse/letters/index.html at depth 0,2, and 10 and write output to data/letters-(depth)."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

echo "Crawl http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html at depth 0 and 1 and write output to data/toscrape-(depth)"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

echo "Crawl http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html at depth 0 and 1 and write output to data/wikipedia-(depth)"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-1 1