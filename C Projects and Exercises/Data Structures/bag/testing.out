gcc -Wall -pedantic -std=c11 -ggdb  -I../lib   -c -o bagtest.o bagtest.c
gcc -Wall -pedantic -std=c11 -ggdb  -I../lib   -c -o bag.o bag.c
gcc -Wall -pedantic -std=c11 -ggdb  -I../lib   -c -o ../lib/file.o ../lib/file.c
gcc -Wall -pedantic -std=c11 -ggdb  -I../lib   -c -o ../lib/mem.o ../lib/mem.c
gcc -Wall -pedantic -std=c11 -ggdb  -I../lib bagtest.o bag.o ../lib/file.o ../lib/mem.o  -o bagtest
./bagtest < test.names
Create first bag...

Test with null bag, good item...
test with null item...
test with null bag, null item...

Count (should be zero): 0

Testing bag_insert...

Count (should be 7): 7

The bag:
{"John","Paul","Ringo","George","Mary","Paul","Peter",}

Move items to a new bag...

The old bag:
Count (should be zero): 0
{}

The new bag:
Count (should be 7): 7
{"Peter","Paul","Mary","George","Ringo","Paul","John",}

delete the bags...
