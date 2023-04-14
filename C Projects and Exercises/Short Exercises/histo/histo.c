/*
 * histo.c - Program that reads a series of positive integers from stdin and prints a histogram.
 * There are 16 scalable bins in the histogram.
 * 
 * Chip Nguyen, COSC 50 Spring 2023
 * April 11, 2023
 */

#include <stdio.h>
#include <stdlib.h>

/* ***************************
 * main function, which creates a histogram
 *
 */

int
main(const int argc, const char* argv[])
{
  const int num_bins = 16;
  int bin_size = 1;
  int bins_array[num_bins];
 
  // initialize values of all bins to 0
  for (int i = 0; i < num_bins; i++) {
    bins_array[i] = 0;
  }

  printf("%d bins of size %d for range [0, %d) \n", num_bins, bin_size, num_bins * bin_size);
  
  int input;
  
  while(scanf("%d", &input) == 1) {  
    // loop to handle when the input is out of range
    while (input >= bin_size * num_bins) {
      // compress the histogram to the first 8 bins
      for (int i = 0; i < num_bins/2; i++) {
        bins_array[i] = bins_array[i*2] + bins_array[i*2 + 1];
      }
      
      // set values of remaining bins to 0
      for (int i = num_bins/2; i < num_bins; i++) {
        bins_array[i] = 0;
      }
      
      bin_size *= 2; // double the size of the bins
      printf("%d bins of size %d for range [0, %d) \n", num_bins, bin_size, num_bins * bin_size);
    }
    // calculate the bin index for the input and increment count in respective bin in array
    int bin_index = input / bin_size;
    bins_array[bin_index] ++;
  }
  
  // create the histogram
  for (int i = 0; i < num_bins; i++) { // loop through each bin
    int bin_count = bins_array[i]; // get count for current bin
    int lower_bound = i * bin_size; // get lower bound of bin
    int upper_bound = lower_bound + bin_size - 1; // get upper bound of bin

    printf("[%2d:%2d] ", lower_bound, upper_bound);
    for (int count = 0; count < bin_count; count++) {
      printf("*");
    }
    printf("\n");
  }
  return 0;
}

