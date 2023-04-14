## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test histo.c

## One number within the range
echo 10 | ./histo

## One number outside the range
echo 20 | ./histo

## One number way outside the range
echo 210 | ./histo

## Mix of numbers within range
echo 1 1 4 5 8 10 15 9 9 9 | ./histo

## Mix of numbers outside range
echo 1 7 20 50 18 19 20 21 49 38| ./histo

## Negative number
echo -10 | ./histo