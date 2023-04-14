## Chip Nguyen, CS50 Spring 2023
## testing.sh provides test cases to test chill.c

## No argument given
./chill

## One valid argument
./chill 20

## One invalid argument
./chill 100

## Two valid arguments
./chill 20 1

## Two arguments, speed is invalid
./chill 20 0.4

## Two arguments, temp is invalid
./chill 100 0.6

## One non-numeric argument
./chill argument

## More than 2 arguments given
./chill 20 1 10
