/*gridPoint.c - c file for CS50 nuggets project
 *
 *
 * gridPoint.c is a c file that aids the server and other modules with storing points on a grid
 * 
 * See header file for more detailed information on usage and implementation
 *
 * 
 * 
 * Team Gitters, Spring 2023, CS50
 * 
 * 
 * functions:
 * gridPoint_new
 * gridPoint_set
 * gridPoint_getX
 * gridPoint_getY
 * gridPoint_setY
 * gridPoint_setX
 * gridPoint_delete
 */




#include "gridPoint.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct gridPoint {
  int x, y;
} gridPoint_t;

gridPoint_t *gridPoint_new(int x, int y)
{
  gridPoint_t *point = malloc(sizeof(gridPoint_t));
  
  if (point == NULL) {
    fprintf(stderr, "ERROR: bad memory allocation when making grid point\n");
    return NULL;
  }
  point->x = x;
  point->y = y;

  return point;
}

bool gridPoint_set(gridPoint_t *point, int x, int y)
{
  if (point == NULL) {
    fprintf(stderr, "ERROR: null grid point when setting (x,y)\n");
    return false;
  }
  point->x = x;
  point->y = y;
  return true;
}

int gridPoint_getX(gridPoint_t *point)
{
  if (point == NULL) {
    fprintf(stderr, "ERROR: null grid point when getting X\n");
    return -1;
  }
  return point->x;
}

int gridPoint_getY(gridPoint_t *point)
{
  if (point == NULL) {
    fprintf(stderr, "ERROR: null grid point when getting Y\n");
    return -1;
  }
  return point->y;
}
void gridPoint_setY(gridPoint_t* gp, int val)
{
  gp->y = val;
}
void gridPoint_setX(gridPoint_t* gp, int val)
{
  gp->x = val;
}

void gridPoint_delete(gridPoint_t *point)
{
  if (point == NULL) {
    fprintf(stderr, "ERROR: trying to free null grid point\n");
    return;
  }
  free(point);
}
