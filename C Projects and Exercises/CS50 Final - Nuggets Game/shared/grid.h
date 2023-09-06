/* grid.h - header file for CS50 grid.c
 * 
 ** This header file defines a grid data structure and provides functions for creating, loading, and manipulating grids. 
 * The grid is represented by the `grid_t` struct, which contains a grid string, width, and height. 
 * Functions are provided for setting and retrieving points in the grid, calculating player visibility within the grid, 
 * obtaining grid dimensions, converting the grid to a string, and more.
 * 
 * The grid structure allows for efficient storage and retrieval of grid data, enabling various grid-based operations 
 * such as game board representation, pathfinding, and visibility analysis.
 *
 * Team Gitters, Spring 2023, CS50
 * 
 * 
 *   
 * 
 * 
 */





#ifndef __GRID_H
#define __GRID_H

#include "gridPoint.h"
#include <stdbool.h>

typedef struct grid grid_t;

/**************** Function Prototypes ****************/

/**************** grid_new ****************/
/**
 * Creates a new grid with the specified grid string, width, and height.
 * Takes ownership of the grid string.
 *
 * Caller provides:
 *   grid_str - The string representation of the grid.
 *   width - The width of the grid.
 *   height - The height of the grid.
 *
 * We return:
 *   pointer to the newly created grid_t structure.
 *   NULL if an error occurred while creating the grid.
 *
 * Caller is responsible for:
 *   later calling grid_delete to free the memory.
 */
grid_t *grid_new(char *grid_str, int width, int height);

/**************** grid_load ****************/
/**
 * Loads a grid from the specified file.
 *
 * Caller provides:
 *   filename - The name of the file to load the grid from.
 *
 * We return:
 *   pointer to the loaded grid_t structure.
 *   NULL if an error occurred while loading the grid.
 *
 * Caller is responsible for:
 *   later calling grid_delete to free the memory.
 */
grid_t *grid_load(const char *filename);

/**************** grid_copy ****************/
/**
 * Creates a copy of the given grid.
 *
 * Caller provides:
 *   grid - The grid to be copied.
 *
 * We return:
 *   pointer to the copied grid_t structure.
 *   NULL if an error occurred while copying the grid.
 *
 * Caller is responsible for:
 *   later calling grid_delete to free the memory.
 */
grid_t *grid_copy(grid_t *);

/**************** grid_delete ****************/
/**
 * Deletes a grid and frees the associated memory.
 *
 * Caller provides:
 *   grid - The grid to be deleted.
 *
 * We do:
 *   free the grid string and the grid.
 */
void grid_delete(grid_t *);

/**************** grid_set_point ****************/
/**
 * Sets the state of a point in the grid to the specified state.
 *
 * Caller provides:
 *   grid - The grid containing the point to be set.
 *   point - The gridPoint_t structure representing the point to be set.
 *   state - The state to set for the point.
 *
 * We do:
 *   update the state.
 */
void grid_set_point(grid_t *, gridPoint_t *, char state);

/**************** grid_get_point ****************/
/**
 * Retrieves the state of a point in the grid.
 *
 * Caller provides:
 *   grid - The grid containing the point.
 *   point - The gridPoint_t structure representing the point.
 *
 * We return:
 *   the character representing the state of the point.
 */
char grid_get_point(grid_t *, gridPoint_t *);

/**************** grid_get_player_visibility ****************/
/**
 * Retrieves a grid representing the visibility of the player on the working grid.
 *
 * Caller provides:
 *   master - The master grid containing the complete information.
 *   working - The working grid representing the current state of the game.
 *   player - The player grid representing the position and visibility of the player.
 *   point - The gridPoint_t structure representing the player's current position.
 *
 * We return:
 *   pointer to the visibility grid_t structure.
 *
 */
grid_t *grid_get_player_visibility(grid_t *master_grid, grid_t *working_grid, grid_t *player_grid, gridPoint_t *point);

/**************** grid_get_dimensions ****************/
/**
 * Retrieves the dimensions (width and height) of the grid.
 *
 * Caller provides:
 *   grid - The grid to retrieve the dimensions from.
 *
 * We return:
 *   gridPoint_t structure representing the dimensions.
 *
 */
gridPoint_t *grid_get_dimensions(grid_t *);

/**************** grid_get_height ****************/
/**
 * Retrieves the height of the grid.
 *
 * Caller provides:
 *   grid - The grid to retrieve the height from.
 *
 * We return:
 *   the height value as an integer.
 */
int grid_get_height(grid_t *);

/**************** grid_get_width ****************/
/**
 * Retrieves the width of the grid.
 *
 * Caller provides:
 *   grid - The grid to retrieve the width from.
 *
 * We return:
 *   the width value as an integer.
 */
int grid_get_width(grid_t *);

/**************** grid_to_string ****************/
/**
 * Converts the grid to a string representation.
 *
 * Caller provides:
 *   grid - The grid to be converted.
 *
 * We return:
 *   pointer to the string representation of the grid.
 *
 */
char *grid_to_string(grid_t *);

/**************** grid_get_point_ind ****************/
/**
 * Retrieves the index of a point in the grid.
 *
 * Caller provides:
 *   grid - The grid containing the point.
 *   point - The gridPoint_t structure representing the point.
 *
 * We return:
 *   the index value as an integer.
 */
int grid_get_point_ind(grid_t *, gridPoint_t *);

#endif