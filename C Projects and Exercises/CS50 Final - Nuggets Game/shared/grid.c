/* grid.c - c file for CS50 Nuggets Lab
 *
 * The grid.c program is a data structure that holds a grid string representation of the map for the users
 * in the nuggets lab. See grid.h for more detailed implementation
 * 
 * 
 * 
 * Team Gitters, Spring 23, CS50
 * 
 * functions:
 * is_visible
 * grid_new
 * grid_load
 * grid_delete
 * grid_set_point
 * grid_get_point
 * grid_get_point_ind
 * grid_get_player_visibility
 * grid_get_dimensions
 * grid_to_string
 * grid_get_width
 * grid_get_height
 * grid_copy
 * check_x_line
 * check_y_line
 * 
 * 
 * 
 */






#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "grid.h"
#include "gridPoint.h"
#include "../libcs50/mem.h"

typedef struct grid {
  char *grid_str;
  int width;
  int height;
} grid_t;
static bool is_visible(grid_t* master_grid, gridPoint_t* point, gridPoint_t* check_point);
static bool check_y_line(grid_t* grid, gridPoint_t* player, gridPoint_t* point);
static bool check_x_line(grid_t* grid, gridPoint_t* player, gridPoint_t* point);

/**************** grid_new ****************/
/*
 * see grid.h file for more information
 *
 */

grid_t *grid_new(char *grid_str, int width, int height)
{
  //initializes grid struct
  int grid_len =(width * height) + 1;
  grid_t *grid = malloc(sizeof(grid_t) + sizeof(char) * grid_len);
  grid_str[width*height -1] = '\0';
  grid->grid_str = grid_str;
  grid->width = width;
  grid->height = height;
  return grid;
}

/**************** grid_load ****************/
/*
 * see grid.h file for more information
 *
 */
grid_t *grid_load(const char *filename)
{
  //checks and makes sure valid file
  FILE *fp = fopen(filename, "r");

  if(fp == NULL) {
    fprintf(stderr, "ERROR: invalid filename when making a grid\n");
    return NULL;
  }
  //gets grid string
  char *grid_str = file_readFile(fp);
  if (grid_str == NULL) {
    fprintf(stderr, "ERROR: could not read file when making grid\n");
    fclose(fp);
    free(grid_str);
    return NULL;
  }

  int height = file_numLines(fp);

  int width = strlen(grid_str) / height;

  //creates new grid
  grid_t *grid = grid_new(grid_str, width, height);

  fclose(fp);

  return grid;
}

/**************** grid_delete ****************/
/*
 * see grid.h file for more information
 *
 */

void grid_delete(grid_t *grid)
{
  if (grid == NULL) {
    fprintf(stderr, "ERROR: trying to delete null grid\n");
    return;
  }
  if (grid->grid_str == NULL) {
    fprintf(stderr, "ERROR: trying to delete grid with null grid_str\n");
    return;
  } else {
    free(grid->grid_str);
  }
  free(grid);
}

/**************** grid_set_point ****************/
/*
 * see grid.h file for more information
 *
 */


void grid_set_point(grid_t *grid, gridPoint_t *point, char state)
{
  if (grid == NULL || point == NULL) {
    fprintf(stderr, "ERROR: trying to set point from null grid or point\n");
    return;
  }
  int ind = grid_get_point_ind(grid, point);
  grid->grid_str[ind] = state;
}

/**************** grid_get_point ****************/
/*
 * see grid.h file for more information
 *
 */
char grid_get_point(grid_t *grid, gridPoint_t *point)
{
  if (grid == NULL || point == NULL) {
    fprintf(stderr, "ERROR: trying to set point from null grid or point\n");
    exit(1);
  }
  int ind = grid_get_point_ind(grid, point);
  if (ind < 0 || ind > strlen(grid_to_string(grid))) {
    return ' ';
  }
  return grid->grid_str[grid_get_point_ind(grid, point)];
}

/**************** grid_get_point_ind ****************/
/*
 * see grid.h file for more information
 *
 */

int grid_get_point_ind(grid_t *grid, gridPoint_t *point)
{
  if (grid == NULL || point == NULL) {
    fprintf(stderr, "ERROR: trying to get index from null grid or point\n");
    exit(1);
  }
  return gridPoint_getY(point) * grid->width + gridPoint_getX(point);
}

/**************** grid_get_player_visibility ****************/
/*
 * see grid.h file for more information
 *
 */

grid_t *grid_get_player_visibility(grid_t *master_grid, grid_t *working_grid, grid_t *player_grid, gridPoint_t *point)
{
  if (master_grid == NULL || working_grid == NULL || player_grid == NULL || point == NULL) {
    fprintf(stderr, "ERROR: null grid or point when getting player visibility\n");
    return NULL;
  }

  int width = grid_get_width(working_grid);
  int height = grid_get_height(working_grid);
  grid_t *vis_grid = grid_copy(working_grid);
  if (vis_grid == NULL || grid_to_string(vis_grid) == NULL) {
    fprintf(stderr, "Error: cannot allocate memory");
    return NULL;
  }
//goes point by point to check visibility
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      gridPoint_t *check_point = gridPoint_new(x, y);
      char working_val = grid_get_point(working_grid, check_point);
      char player_val = grid_get_point(player_grid, check_point);
      char master_val = grid_get_point(master_grid, check_point);

      if (working_val == ' ' || working_val == '\n') {
        continue;
      }

      if (is_visible(master_grid, point, check_point))
      {
        grid_set_point(vis_grid, check_point, working_val);
      }
      else { // This space is invisible from the current location
        if (player_val != ' ') {
          // If we've seen the space before...
          if (master_val == '*') {
            grid_set_point(vis_grid, check_point, '.');
          } else {
            grid_set_point(vis_grid, check_point, master_val);
          }
        } else {
          grid_set_point(vis_grid, check_point, ' ');
        }
      }
      gridPoint_delete(check_point);
    }
  }

  return vis_grid;
}

/**************** grid_get_dimensions ****************/
/*
 * see grid.h file for more information
 *
 */

gridPoint_t *grid_get_dimensions(grid_t *grid)
{
  if (grid == NULL) {
    fprintf(stderr, "ERROR: trying to get dimensions of null grid\n");
    return NULL;
  }
  return gridPoint_new(grid->width, grid->height);
}

char *grid_to_string(grid_t *grid)
{
  if (grid == NULL) {
    fprintf(stderr, "ERROR: trying to get string of null grid\n");
    return NULL;
  }
  return grid->grid_str;
}
int grid_get_width(grid_t* grid)
{
  return grid->width;
}
int grid_get_height(grid_t* grid)
{
  return grid->height;
}

/**************** grid_copy ****************/
/*
 * see grid.h file for more information
 *
 */

grid_t* grid_copy(grid_t* grid)
{
  char *new_str = malloc(sizeof(char) * strlen(grid->grid_str) + 1);
  char *grid_str = grid->grid_str;
  sprintf(new_str, "%s", grid_str);
  grid_t* grd = grid_new(new_str, grid->width, grid->height);
  return grd;
}





/**************** check_x_line ****************/
/*
 * caller provides
 *  valid pointer to grid, 2 gridPoints 
 *
 * returns
 *  true if visble 
 *  false if not
 * 
 * we do
 *  use slope to check if a point is visible for the player --> can be added to sight grid
 * 
 */


static bool check_x_line(grid_t* grid, gridPoint_t* player, gridPoint_t* point)
{
  // for calculating the slope
  int dx = gridPoint_getX(point) - gridPoint_getX(player);
  int dy = gridPoint_getY(point) -  gridPoint_getY(player);
  
  if (dx == 0) {
    return true;
  }
  double slope = ((double) dy) / ((double) dx);

  // start for while loop
  int start = (dx > 0) ? 1 : -1;

  // start loop
  while (abs(start) < abs(dx)) {
    // find y through the x line
    double through_point = ((double) gridPoint_getY(player)) + ((double) start) * slope;

    
    if (round(through_point) == through_point) {
      // if it lies exactly  
      gridPoint_t* on_point = gridPoint_new(gridPoint_getX(player) + start, (int) through_point);

      // check if on_point is NULL
      if (on_point == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        return false;
      }

      char result = grid_get_point(grid, on_point);
      gridPoint_delete(on_point);

      // check the point
      if (result != '.' && result != '*') {
        return false;
      }
    }
    else {
      // else the point passes between points

      // create grid points 
      gridPoint_t* under_point = gridPoint_new(gridPoint_getX(player) + start, round(through_point - 0.5f));
      gridPoint_t* over_point = gridPoint_new(gridPoint_getX(player) + start, round(through_point + 0.5f));

      if (under_point == NULL || over_point == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        return false;
      }

      char under_result = grid_get_point(grid, under_point);
      char over_result = grid_get_point(grid, over_point);
      gridPoint_delete(under_point);
      gridPoint_delete(over_point);

      // check the points
      if (under_result != '.' && over_result != '.' && under_result != '*' && over_result != '*') {
        return false;
      }
    }

    start += (dx > 0) ? 1 : -1;
  }

  return true;
}

/**************** check_y_line ****************/
/*
 * caller provides
 *  valid pointer to grid, 2 gridPoints 
 *
 * returns
 *  true if visble 
 *  false if not
 * 
 * we do
 *  use slope to check if a point is visible for the player --> can be added to sight grid
 * 
 */

static bool check_y_line(grid_t* grid, gridPoint_t* player, gridPoint_t* point)
{
  int dx = gridPoint_getX(point) - gridPoint_getX(player);
  int dy = gridPoint_getY(point) -  gridPoint_getY(player);
  double slope = ((double) dx) / ((double) dy);

  if (dy == 0) {
    return true;
  }

  // start for while loop
  int start = (dy > 0) ? 1 : -1;

  // start loop
  while (abs(start) < abs(dy)) {
    // find x through the y line
    double through_point = ((double) gridPoint_getX(player)) + ((double) start) * slope;

    // if it lies exactly 
    if (round(through_point) == through_point) {

      gridPoint_t* on_point = gridPoint_new((int) through_point, gridPoint_getY(player) + start);

      if (on_point == NULL) {
        fprintf(stderr, "error: out of memory\n");
        return false;
      }

      char result = grid_get_point(grid, on_point);
      gridPoint_delete(on_point);

      // check the point
      if (result != '.' && result != '*') {
        return false;
      }
    }

    
    else {
      // else the point passes between points 

      // under and over points
      gridPoint_t* under_point = gridPoint_new(round(through_point - 0.5f), gridPoint_getY(player) + start);
      gridPoint_t* over_point = gridPoint_new(round(through_point + 0.5f), gridPoint_getY(player) + start);

      if (under_point == NULL || over_point == NULL) {
        fprintf(stderr, "error: out of memory\n");
        return false;
      }

      char under_result = grid_get_point(grid, under_point);
      char over_result = grid_get_point(grid, over_point);
      gridPoint_delete(under_point);
      gridPoint_delete(over_point);

      // check the points
      if (under_result != '.' && over_result != '.' && under_result != '*' && under_result != '*') {
        return false;
      }
    }

    start += (dy > 0) ? 1 : -1;
  }

  return true;
}

/**************** is_visible ****************/
/*
 * caller provides
 *  valid pointer to grid, 2 gridPoints 
 *
 * returns
 *  true if visble 
 *  false if not
 * 
 * we do
 *  use check_x_line and check_y_line to see if point on grid is visible to 
 *  player
 * 
 */

static bool is_visible(grid_t *grid, gridPoint_t *player, gridPoint_t *point)
{
  // check args
  if (grid == NULL || player == NULL || point == NULL) {
    return false;
  }
  
  int x = gridPoint_getX(player);
  int y = gridPoint_getY(player);

  gridPoint_t* dimensions = grid_get_dimensions(grid);

  // check if player is out of bounds
  if (x < 0 || x >= gridPoint_getX(dimensions) || y < 0 || y >= gridPoint_getY(dimensions)) {
    fprintf(stderr, "ERROR: player is out of bounds\n");
    return false;
  }

  x = gridPoint_getX(point);
  y = gridPoint_getY(point);

  // check if point is out of bounds
  if (x < 0 || x >= gridPoint_getX(dimensions) || y < 0 || y >= gridPoint_getY(dimensions)) {
    fprintf(stderr, "ERROR: point is out of bounds\n");
    return false;
  }

  gridPoint_delete(dimensions);

  return check_x_line(grid, player, point) && check_y_line(grid, player, point);
}
