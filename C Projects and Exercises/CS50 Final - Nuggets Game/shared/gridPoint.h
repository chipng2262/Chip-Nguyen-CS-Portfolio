/* gridPoint.h - header file for CS50 nuggets project
 *
 *
 * The gridPoint module provides an x y coordinate for the server as well as other 
 * modules in the shared library. It is particulary useful in helping to locate a player
 * that is moving on a grid as well as for getting and storing the dimensions of maps
 *
 *
 * Team Gitters, Spring 23, CS50
 * 
 */


#ifndef __GRIDPOINT_H
#define __GRIDPOINT_H

#include <stdbool.h>

/**************** Global types ****************/
typedef struct gridPoint gridPoint_t;

/**************** Functions ****************/

/**************** gridPoint_new ****************/
/**
 * Creates a new (x, y) point that stores the location provided by the x and y
 * coordinates.
 *
 * Caller provides:
 *   x and y coordinates - The coordinates must be greater than or equal to 0
 *                         and within the dimensions of the display window and
 *                         within empty spots in the map.
 *
 * We return:
 *   pointer to the newly created grid point object
 *   NULL if an error occurred while creating the pointer.
 *
 * Caller is responsible for:
 *   later calling gridPoint_delete.
 */
gridPoint_t* gridPoint_new(int x, int y);

/**************** gridPoint_set ****************/
/**
 * Sets the coordinates of the given grid point object to the provided (x, y) values.
 *
 * Caller provides:
 *   pointer to the grid point object
 *   x and y coordinates - The coordinates must be greater than or equal to 0
 *                         and within the dimensions of the display window and
 *                         within empty spots in the map.
 *
 * We return:
 *   true if the operation is successful
 *   false otherwise (e.g., if the point is NULL).
 */
bool gridPoint_set(gridPoint_t* point, int x, int y);

/**************** gridPoint_getX ****************/
/**
 * Retrieves the x-coordinate of the given grid point object.
 *
 * Caller provides:
 *   pointer to the grid point object
 *
 * We return:
 *   the x-coordinate of the grid point
 *   -1 if the point is NULL.
 */
int gridPoint_getX(gridPoint_t* point);

/**************** gridPoint_getY ****************/
/**
 * Retrieves the y-coordinate of the given grid point object.
 *
 * Caller provides:
 *   pointer to the grid point object
 *
 * We return:
 *   the y-coordinate of the grid point
 *   -1 if the point is NULL.
 */
int gridPoint_getY(gridPoint_t* point);

/**************** gridPoint_delete ****************/
/**
 * Deletes a grid point object and frees the associated memory.
 *
 * Caller provides:
 *   pointer to the grid point object to be deleted.
 * 
 * We do:
 *   If the point is NULL, no action is performed.
 *   Otherwise, free the grid point. 
 * 
 */
void gridPoint_delete(gridPoint_t* point);

void gridPoint_setY(gridPoint_t* gp, int val);

void gridPoint_setX(gridPoint_t* gp, int val);



#endif