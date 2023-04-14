## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test words.c

## Standard input
echo "My name is Chip" | ./words

## Input with numbers and non-letter characters. Those should not appear in the output.
echo "She's 11 years old!" | ./words

## A blank file
./words blankfile.txt

## A valid file
./words testfile_1.txt

## A file that does not exist
./words chip.sh

## A "-" file
echo "Input for hyphen file" | ./words -

## A file name followed by a hyphen
echo "File followed by hypen" | ./words testfile_1.txt -

## A file name, a hyphen, then a file name
echo "This should be in the middlle" | ./words testfile_1.txt - testfile_2.txt