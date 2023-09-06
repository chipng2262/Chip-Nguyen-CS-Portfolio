/*map.c - c file for CS50 nuggets project
 *
 *
 * map.c is a c file that implements the map data structure. 
 * 
 * See header file for more detailed information on usage and implementation
 *
 * 
 * 
 * Team Gitters, Spring 2023, CS50
 * 
 * 
 * functions:
 * map_new
 * map_delete
 * map_get_working_grid
 * map_get_master_grid
 * map_set_working_grid
 * map_set_master_grid
 */




#include "map.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct map {
  grid_t *master_grid;
  grid_t *working_grid;
} map_t;

map_t *map_new(grid_t *master_grid, grid_t *working_grid)
{
  map_t *map = malloc(sizeof(map_t));

  map->master_grid = master_grid;
  map->working_grid = working_grid;

  return map;
}

void map_delete(map_t *map)
{
  free(map->master_grid);
  free(map->working_grid);
  free(map);
}

grid_t *map_get_working_grid(map_t *map)
{
  if (map == NULL) {
    fprintf(stderr, "ERROR: null map when getting working grid\n");
    return NULL;
  }
  return map->working_grid;
}

grid_t *map_get_master_grid(map_t *map)
{
  if (map == NULL) {
    fprintf(stderr, "ERROR: null map when getting master grid\n");
    return NULL;
  }
  return map->master_grid;
}
void map_set_working_grid(map_t* map, grid_t* grid)
{
  grid_delete(map->working_grid);
  map->working_grid = grid;
}
void map_set_master_grid(map_t* map, grid_t* grid)
{
  grid_delete(map->master_grid);
  map->master_grid = grid;
}

