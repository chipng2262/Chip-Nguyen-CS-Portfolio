/*game.c - c file for CS50 nuggets project
 *
 *
 * game.c is a c file that does the true work of the server. The server calls the various methods throughout game 
 * in order to send the appropriate messages to the clients
 * 
 * See header file for more detailed information on usage and implementation
 *
 * 
 * 
 * Team Gitters, Spring 2023, CS50
 * 
 * 
 * functions:
 * 
 * game_new
 * game_delete
 * game_pickup_gold
 * game_place_gold
 * game_move_player
 * game_add_player
 * game_add_spectator
 * game_remove_player
 * game_remove_spectator
 * game_get_map
 * game_get_working_grid
 * game_get_master_grid
 * game_get_num_players
 * game_get_max_players
 * game_get_num_removed_players
 * game_get_num_gold_piles
 * game_get_gold_active
 * game_get_player
 * game_get_player_from_id
 * game_get_spectator
 * game_summary
 * game_update_sight_grid  
 * 
 */






#include "game.h"
#include "constants.h"
#include <ctype.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../shared/player.h"
#include "../shared/map.h"
#include <string.h>

typedef struct game {
  // Array of player pointers
  player_t **players;
  player_t **removed_players;
  player_t *spectator;
  map_t *map;
  int num_players;
  int max_players;
  int num_removed_players;
  int gold_num_piles;
  int gold_active;
  int total_gold;
  hashtable_t *addr_to_players;
} game_t;

/**************** game_new ****************/
/*
 * see game.h file for more information
 *
 */

game_t *game_new(char *map_file, int gold_num_piles, int gold_total)
{ 
  //initializes game and checks if can allocate memory
  game_t *game = malloc(sizeof(game_t));
  if (game == NULL) {
    fprintf(stderr, "ERROR: bad memory allocation in game initialization\n");
    return NULL;
  }

  game->players = malloc(sizeof(player_t *) * MAX_NUM_PLAYERS);
  if (game->players == NULL) {
    fprintf(stderr, "ERROR: bad memory allocation in game initialization\n");
    return NULL;
  }

  game->removed_players = malloc(sizeof(player_t *) * MAX_NUM_PLAYERS);
  if (game->removed_players == NULL) {
    fprintf(stderr, "ERROR: bad memory allocation in game initialization\n");
    return NULL;
  }

  //creates master and working grid for map struct
  grid_t *master_grid = grid_load(map_file);
  grid_t *working_grid = grid_load(map_file);
  if(working_grid == NULL || master_grid == NULL)
  {
    return NULL;
  }
  game->map = map_new(master_grid, working_grid);
  if (game->map == NULL) {
    fprintf(stderr, "ERROR: bad memory allocation in game initialization\n");
    return NULL;
  }
  //initializes game struct
  game->addr_to_players = hashtable_new(50);
  game->num_players = 0;
  game->max_players = MAX_NUM_PLAYERS;
  game->num_removed_players = 0;
  game->gold_num_piles = gold_num_piles;
  game->gold_active = gold_total;
  game->total_gold = gold_total;
  game->spectator = NULL;

  //places gold onto grid
  game_place_gold(game);

  return game;
}
/**************** game_delete ****************/
/*
 * see game.h file for more information
 *
 */
void game_delete(game_t *game)
{ 
  //frees 
  free(game->players);
  free(game->removed_players);
  map_delete(game->map);
  free(game);
  free(game->addr_to_players);
}

/**************** game_pickup_gold ****************/
/*
 * see game.h file for more information
 *
 */

int game_pickup_gold(game_t *game, addr_t *playerID)
{
  //checks if can allocate
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when picking up gold\n");
    return 0;
  }
  //gets player from address given
  player_t *player = game_get_player(game, playerID);
  if (player == NULL) {
    fprintf(stderr, "ERROR: could not get player when picking up gold\n");
    return 0;
  }

  //initializes gold amount and calculates how much per pile
  int goldAmount;
  if(game->gold_num_piles == 1)
  {
    //if last pile --> gives remaining amount
    goldAmount = game->gold_active;
  }
  else{
    //calculates randomized amount of gold for the picked up pile
    double avgGold = (double)game->gold_active / (double)game->gold_num_piles;
    int deviation = (int)(avgGold/1.5);
    int hD = (int)((double)deviation/2);
    goldAmount = ((rand() % deviation) - hD) + (int)avgGold;
  }
  //reduces total gold availible by randomized gold amount
  game->gold_active = game->gold_active - goldAmount;

  //gives gold to player that picked up
  player_earn_gold(player, goldAmount);
  gridPoint_t *gp = player_get_pos(player);
  
  //sets point on master grid to '.'
  grid_set_point(game_get_master_grid(game), gp, '.');
  game->gold_num_piles--;
  return goldAmount;
}

/**************** game_place_gold ****************/
/*
 * see game.h file for more information
 *
 */

void game_place_gold(game_t* game)
{
  //checks if game is properly passed
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when picking up gold\n");
    return;
  }
  int i = 0;
  gridPoint_t* gp;
  //loops until proper amount of piles are placed
  while(i < game_get_num_gold_piles(game))
  {
    //randomizes positioning of placed gold
    int y = rand() % grid_get_height(game_get_master_grid(game));
    int x = rand() % grid_get_width(game_get_master_grid(game));
    gp = gridPoint_new(x,y);
    
    //checks if valid position to place gold
    if((grid_get_point(game_get_master_grid(game), gp)) == '.')
    {
      grid_set_point(game_get_master_grid(game), gp, '*');
      grid_set_point(game_get_working_grid(game), gp, '*');
      i++;
    }
    //deletes gridpoint each iteration
    gridPoint_delete(gp);
    gp = NULL;

  }
}

/**************** game_move_player ****************/
/*
 * see game.h file for more information
 *
 */

int game_move_player(game_t *game, addr_t *playerID, int dx, int dy, bool is_jump)
{

  //checks if proper game
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when moving player\n");
    return -1;
  }

  //checks if valid player
  player_t *player = game_get_player(game, playerID);
  if (player == NULL) {
    fprintf(stderr, "ERROR: could not get player when picking up gold\n");
    return -1;
  }


  gridPoint_t *player_pos = player_get_pos(player);
  gridPoint_t *player_pos_new = gridPoint_new(gridPoint_getX(player_pos) + dx, gridPoint_getY(player_pos) + dy);
  grid_t *working_map = game_get_working_grid(game);
  //grid_t *master_map = game_get_master_grid(game);

  //checks if capital letter --> will loop until hit wall or switch with another player
  if (is_jump) {
    //initializes gold
    int gold = 0;
    while (true) {
      player_pos = player_get_pos(player);
      player_pos_new = gridPoint_new(gridPoint_getX(player_pos) + dx, gridPoint_getY(player_pos) + dy);
      char new_pos_val = grid_get_point(working_map, player_pos_new);

      //checks if can move to certain position
      bool is_spot_free = new_pos_val == '.' || new_pos_val == '*' || new_pos_val == '#';
      bool is_spot_on_player = isalpha(new_pos_val);

      //checks if free spot to move to
      if (is_spot_free) {
        player_move(player, dx, dy);
        if(new_pos_val == '#')
        {
          grid_set_point(player_get_sightgrid(player), player_pos_new, '#');
        }
      }
      //checks if player is on existing spot
      if (is_spot_on_player) {
        int player_id = new_pos_val - ASCII_OFFSET;
        player_t *on_player = game->players[player_id];

        // Swap places
        player_move(on_player, -dx, -dy);
        player_move(player, dx, dy);
      }

    //checks if gold on place
    bool is_spot_on_gold = new_pos_val == '*';
    if (is_spot_on_gold) {
      //increment gold picked up
      gold += game_pickup_gold(game, playerID);
    }  
    gridPoint_delete(player_pos_new);
    if (!is_spot_free || is_spot_on_player) {
      return gold;
      break;
    }
    }
  } 
  //for single moves
  else {
    //gets char of player
    char new_pos_val = grid_get_point(working_map, player_pos_new);

    //booleans for dictating movement
    bool is_spot_free = new_pos_val == '.' || new_pos_val == '#' || new_pos_val == '*';
    bool is_spot_on_player = isalpha(new_pos_val);

    //movement for not switching
    if (is_spot_free) {
      player_move(player, dx, dy);
    }
    if (is_spot_on_player) {
      int player_id = new_pos_val - ASCII_OFFSET;
      player_t *on_player = game->players[player_id];

      // Swap places
      player_move(on_player, -dx, -dy);
      player_move(player, dx, dy);
    }
  }
  //returns total gold picked up for single movement 
  char new_pos_val = grid_get_point(game_get_master_grid(game), player_get_pos(player));
  bool is_spot_on_gold = new_pos_val == '*';
  if (is_spot_on_gold) {
    int picked_up_amt = game_pickup_gold(game, playerID);
    return picked_up_amt;
  } else {
    return 0;
  }
}

/**************** game_add_player ****************/
/*
 * see game.h file for more information
 *
 */

player_t *game_add_player(game_t *game, addr_t *address, gridPoint_t *gridPoint, char *uname)
{
  //checks parameters
  if (game == NULL || address == NULL || gridPoint == NULL || uname == NULL) {
    fprintf(stderr, "Error: null arguments\n");
    return NULL;
  }

  //creates new player to be added to the game
  player_t *player = player_new(uname, address, gridPoint, game->num_players);
  if (player == NULL) {
    fprintf(stderr, "Error in valid player added\n");
  }

  //gets working grid
  grid_t *working_grid = game_get_working_grid(game);
  int grid_size = grid_get_height(working_grid) * grid_get_width(working_grid);
  char *grid_str = malloc(sizeof(char) * (grid_size + 1)); // Allocate grid_size + 1 characters
  //initializes grid to an empty string
  for (int i = 0; i < grid_size; i++) {
    grid_str[i] = ' ';
  }
  grid_str[grid_size] = '\0'; // Set the null terminator at grid_size
  //initializes sightgrid to be used by plaeter
  grid_t *init_sightgrid = grid_new(grid_str, grid_get_width(working_grid), grid_get_height(working_grid));
  player_set_sightgrid(player, init_sightgrid);

  //creates visibility grid for player
  grid_t *vis = grid_get_player_visibility(
    game_get_master_grid(game),
    game_get_working_grid(game),
    init_sightgrid,
    player_get_pos(player)
  );
  //updates sight grid
  game_update_sight_grid(game, address);
  player_set_sightgrid(player, vis);

  game->num_players++; 
  game->players[player_get_id(player)] = player;
  //inserts player into hashtable for addresses --> easy access
  hashtable_insert(game->addr_to_players, message_stringAddr(*address), player);

  return player;
}

/**************** game_add_spectator ****************/
/*
 * see game.h file for more information
 *
 */

void game_add_spectator(game_t *game, addr_t *address)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when adding spectator\n");
    return;
  }
  //creates new player object of spectator
  player_t *spectator = player_new(NULL, address, NULL, -1);
  // checks if spectator already exists --> must remove
  if (game->spectator != NULL) {
    game_remove_spectator(game);
  }
  //sets spectator to newly added spectator
  game->spectator = spectator;
}

void game_remove_player(game_t *game, addr_t *address)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when removing player\n");
    return;
  }
  player_t *remove = game_get_player(game, address);
  player_set_alive(remove,false);
  //game->players[player_get_id(remove)] = NULL;
  game->num_removed_players++;
  //game->num_players--;
}
void game_remove_spectator(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when removing spectator\n");
    return;
  }

  player_t *spectator = game->spectator;
  if (spectator == NULL) {
    return;
  }
  game->spectator = NULL;
  player_delete(spectator);
}
/**************** game_summary ****************/
/*
 * see game.h file for more information
 *
 */
char* game_summary(game_t *game) {
  //checks param
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when getting game summary\n");
    return NULL;
  }
  
  player_t *summary[game->num_players];
  for (int i = 0; i < game->num_players; i++) {
    // active players
    if (game->players[i] != NULL && player_get_username(game->players[i]) != NULL) {
      summary[i] = game->players[i];
    }
    // removed players
  }
  
  char *summary_str = calloc(game->num_players, sizeof(char) * 50);
  if (summary_str == NULL) {
    fprintf(stderr, "ERROR: out of memory\n");
    return NULL;
  }

  sprintf(summary_str, "GAME OVER: \n");
  
  // loop through players
  for (int i = 0; i < game->num_players; i++) {
    char *line = malloc(sizeof(char) * 50);
    if (line == NULL) {
      fprintf(stderr, "ERROR: out of memory\n");
      return NULL;
    }
    // make output line
    sprintf(line, "%c \t%d \t%s\n", 
            player_get_alias(summary[i]), 
            player_get_score(summary[i]), 
            player_get_username(summary[i]));
    strcat(summary_str, line);
    free(line);
  }// Print or return the summary string as per your requirements
  return summary_str; // Don't forget to free the allocated memory
}
/**************** game_update_sight_grid ****************/
/*
 * see game.h file for more information
 *
 */
void game_update_sight_grid(game_t *game, addr_t *address)
{
  if (game == NULL || address == NULL) {
    fprintf(stderr, "ERROR: game or address is null when updating sight grid\n");
    return;
  }

  player_t *player = game_get_player(game, address);
  if (player == NULL) {
    fprintf(stderr, "ERROR: could not get player from address when updating sight grid\n");
    return;
  }

  grid_t *vis = grid_get_player_visibility(
    game_get_master_grid(game),
    game_get_working_grid(game),
    player_get_sightgrid(player),
    player_get_pos(player)
  );
  // grid_t *vis = game_get_working_grid(game);
  grid_set_point(vis, player_get_pos(player), '@');
  player_set_sightgrid(player, vis);
}


void game_redraw_working_grid(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: game null when redrawing working grid\n");
    return;
  }

  grid_t *master_grid = game_get_master_grid(game);
  grid_t *working_grid_new = grid_copy(master_grid);

  for (int i = 0; i < (game_get_num_players(game)); i++) {
    player_t *player = game->players[i];
    if(player_is_alive(player)){
      char player_alias = player_get_alias(player);
      gridPoint_t *player_pos = player_get_pos(player);
      grid_set_point(working_grid_new, player_pos, player_alias);
    }
  }
  map_t *map = game_get_map(game);
  map_set_working_grid(map, working_grid_new);
}

/******** Getter functions ********/
map_t* game_get_map(game_t* game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: Null game pointer\n");
    return NULL;
  }
  return game->map;
}

grid_t *game_get_working_grid(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: Null game when getting working grid\n");
    return NULL;
  }
  return map_get_working_grid(game->map);
}

grid_t *game_get_master_grid(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: Null game when getting master grid\n");
    return NULL;
  }
  return map_get_master_grid(game->map);
}

int game_get_num_players(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: Null game pointer\n");
  }
  return game->num_players;
}

int game_get_max_players(game_t *game)
{
  return MAX_NUM_PLAYERS;
}

int game_get_num_removed_players(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr,"ERROR: null game when getting num removed players\n");
    return -1;
  }
  return game->num_removed_players;
}

int game_get_num_gold_piles(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr,"ERROR: null game when getting num gold piles\n");
    return -1;
  }
  return game->gold_num_piles;
}

int game_get_gold_active(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr,"ERROR: null game when getting num active gold piles\n");
    return -1;
  }
  return game->gold_active;
}

player_t *game_get_player(game_t *game, addr_t *address)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when getting player\n");
    return NULL;
  }
  return hashtable_find(game->addr_to_players, message_stringAddr(*address));
}

player_t *game_get_player_from_id(game_t *game, int playerID)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when getting player by ID\n");
    return NULL;
  }
  if (playerID < 0 || playerID > game->num_players) {
    fprintf(stderr, "ERROR: player ID index out of bounds\n");
    return NULL;
  }
  return game->players[playerID];
}

player_t *game_get_spectator(game_t *game)
{
  if (game == NULL) {
    fprintf(stderr, "ERROR: null game when getting spectator\n");
    return NULL;
  }
  return game->spectator;
}





