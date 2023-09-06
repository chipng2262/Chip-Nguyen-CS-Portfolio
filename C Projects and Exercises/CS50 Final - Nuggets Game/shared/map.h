/*map.h - header file for CS50 nuggets project
 *
 *
 * map.h is a header file that implements the map data structure. It takes two grids the master and working
 * grid. The master grid is used in the server as the grid with no players on it where as the working grid 
 * is constantly being updated with moving players. The working grids draws from the master grid and then layers
 * the players on top.
 * 
 * 
 * 
 * Team Gitters, Spring 2023, CS50
 * 
 * 
 */




#ifndef __MAP_H
#define __MAP_H

#include "grid.h"

typedef struct map map_t;

map_t *map_new(grid_t *master, grid_t *working);

void map_delete(map_t *);

grid_t *map_get_working_grid(map_t* map);

grid_t *map_get_master_grid(map_t* map);

void map_set_working_grid(map_t* map, grid_t* grid);

void map_set_master_grid(map_t* map, grid_t* grid);

#endif // !__MAP_H
