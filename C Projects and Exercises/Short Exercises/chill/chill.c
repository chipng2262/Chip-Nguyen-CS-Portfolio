/*
 * chill.c - Program that calculates wind chill using current temperature and windspeed. 
 * Program can take no argument, one argument, or two arguments.
 * 
 * Chip Nguyen, COSC50 Spring 2023
 * April 11, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function prototypes
float calcWindChill(float temp, float speed);

/* ***************************
 * main function, which handles different cases for different numbers of inputs
 *
 */
int
main(const int argc, const char* argv[])
{
  float temp;
  float speed;

  // Handle 0 argument
  if (argc == 1) {
    printf(" Temp   Wind  Chill");
    printf("\n");
    printf("-----  -----  -----");    
    printf("\n");

    for (temp = -10; temp <= 40; temp += 10) {
      for (speed = 5; speed <= 15; speed += 5) {
        float windChill = calcWindChill(temp, speed);
        printf("%5.1f  %5.1f  %5.1f" , temp, speed, windChill);
        printf("\n");
      }
    }
  }
  
  // Handle 1 argument
  else if (argc == 2) {
    // Check if the input is valid
    if ((sscanf(argv[1], "%f", &temp)) == 0){
      printf("Invalid input. Try again with a number. \n");
      exit(1);
    }

    // Check if the temperature is within range
    if (temp < -99 || temp >50) {
      printf("Temperature out of range. Choose one between -99 and 50. \n");
      exit(1);
    }

    // Calculate and tabulate data
    printf(" Temp   Wind  Chill");
    printf("\n");
    printf("-----  -----  -----");
    printf("\n");
    
    for (speed = 5; speed <= 15; speed +=5) {
      float windChill = calcWindChill(temp, speed);
      printf("%5.1f  %5.1f  %5.1f" , temp, speed, windChill);
      printf("\n");
    }
  }

  // Handle 2 arguments
  else if (argc == 3) {
    // Check if input is valid
    if ((sscanf(argv[1], "%f", &temp)) == 0){
      printf("Invalid temperature. Try again with a number. \n");
      exit(1);
    }
    else {
      if (temp < -99 || temp >50) {
        printf("Temperature out of range. Choose one between -99 and 50. \n");
        exit(1);
    }
    
    if ((sscanf(argv[2], "%f", &speed)) == 0){
      printf("Invalid speed. Try again with a number. \n");
      exit(1);
    }
    else {
      if (speed < 0.5) {
        printf("Speed is too low. Choose one larger than 0.5. \n");
        exit(1);
      }
    }

    printf(" Temp   Wind  Chill");
    printf("\n");
    printf("-----  -----  -----");
    printf("\n");
    float windChill = calcWindChill(temp, speed);
    printf("%5.1f  %5.1f  %5.1f", temp, speed, windChill);
    printf("\n");
    }
  }

  // Handle more than 2 arguments
  else {
    printf("Too many arguments");
    exit(2);
  }
  return 0;
}

/* ***************************
 * function to calculate wind chill using the given formula
 */
float
calcWindChill(float temp, float speed)
{
  float windChill;
  windChill = (35.74 + (0.6215 * temp) - (35.75 * (pow(speed, 0.16))) + ((0.4275 * temp) * (pow (speed, 0.16))));
  return windChill;
}
