#ifndef __GAME_H
#define __GAME_H

#include <stdbool.h>
#include "../shared/map.h"
#include "../support/message.h"
#include "../shared/gridPoint.h"
#include "../shared/player.h"
#include "../shared/grid.h"

typedef struct game game_t;

game_t *game_new(char *map_file, int gold_num_piles, int gold_total);

void game_delete(game_t *);

int game_pickup_gold(game_t *, addr_t *playerID);

int game_move_player(game_t *, addr_t *playerID, int dx, int dy, bool is_jump);

player_t *game_add_player(game_t *, addr_t *, gridPoint_t *, char *username);

void game_add_spectator(game_t *, addr_t *);

void game_remove_player(game_t *, addr_t *);

void game_remove_player_by_id(game_t *, char playerID);

void game_remove_spectator(game_t *);

/******** Getter functions ********/

/* extracts the map from the game */
map_t* game_get_map(game_t* game);

grid_t *game_get_working_grid(game_t *);

grid_t *game_get_master_grid(game_t *);

/* extracts the number of players from the game */
int game_get_num_players(game_t* game);

/* extracts the maximum number of players from the game */
int game_get_max_players(game_t* game);

 /* extracts the number of removed players from the game */
int game_get_num_removed_players(game_t* game);

 /* extracts the number of gold piles from the game */
int game_get_num_gold_piles(game_t* game);

 /* extracts the number of active gold from the game */
int game_get_gold_active(game_t* game);

char game_get_player_id(game_t *, addr_t *);

player_t *game_get_player(game_t *, addr_t *);

player_t *game_get_removed_player(game_t *, addr_t *);

player_t *game_get_player_from_id(game_t *, int playerID);

player_t *game_get_spectator(game_t *);

char *game_summary(game_t *);

void game_update_sight_grid(game_t *, addr_t *address);

void game_redraw_working_grid(game_t *);

#endif // !__GAME_H
