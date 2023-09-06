/*game.h - header file for CS50 nuggets project
 *
 *
 * The game module provides a comprehensive set of functions and data structures for creating and managing an interactive multiplayer game.
 * With the game module, multiple players can navigate a dynamic map, collect valuable resources, and engage with each other in real-time. 
 * It supports seamless player interactions, including movement, resource acquisition, and player-to-player communication. Additionally, 
 * the module offers a spectator mode for non-playing observers to enjoy the game. By integrating the game module into your code, you can 
 * easily create immersive and engaging multiplayer gaming experiences with rich gameplay mechanics and interactive elements.
 *
 * 
 * 
 * Team Gitters Spring 2023, CS50
 */




#ifndef __GAME_H
#define __GAME_H

#include <stdbool.h>
#include "../shared/map.h"
#include "../support/message.h"
#include "../shared/gridPoint.h"
#include "../shared/player.h"
#include "../shared/grid.h"

typedef struct game game_t;

/**************** game_new ****************/
/*
 * caller provides
 *  valid mapfile, number of gold piles, total gold
 * 
 * returns
 *  pointer to a new game that can be used by the server to 
 *  carry out various operation
 *  NULL if error with memory allocation
 *
 * we do
 *   Allocate memory for the game struct and check for any allocation errors.
 *   Allocate memory for the players and removed_players arrays and check for errors.
 *   Load the map from a file and create master_grid and working_grid.
 *   Create a new map struct using the master_grid and working_grid.
 *   Initialize various fields in the game struct.
 *   Place gold onto the grid.
 *   Return the initialized game struct.
 */
game_t *game_new(char *map_file, int gold_num_piles, int gold_total);

/**************** game_delete ****************/
/*
 * caller provides
 *  valid game
 *
 * returns
 *  void
 * 
 * we do
 *  free all possible memory associated with the game struct
 * */
void game_delete(game_t *game);

/**************** game_pickup_gold ****************/
/*
 * caller provides
 *  valid game and address to player
 * 
 * returns
 *  -1 if invalid game or player given or any other issue associated with 
 *  program
 *  else if returns the total amount of gold picked up by the player
 * 
 * we do
 *   Check if the game parameter is valid, and if not, return an error.
 *   Get the player from the given playerID address, and if not found, return an error.
 *   Calculate the amount of gold to be picked up based on the number of gold piles and the remaining gold in the game.
 *   Reduce the active gold amount in the game by the calculated gold amount.
 *   Give the gold to the player by calling the player_earn_gold function.
 *   Set the corresponding point on the master grid to '.'.
 *   Decrement the number of gold piles in the game.
 *   Return the amount of gold picked up.
 *
 */
int game_pickup_gold(game_t *game, addr_t *playerID);

/**************** game_place_gold ****************/
/*
 * caller provides
 *  valid pointer to game
 * 
 * return
 *  void function
 *  returns early if any issue associated with program (valid game)
 *
 * we do
 *   Check if the game parameter is valid, and if not, return.
 *   Initialize a counter i to 0.
 *   Create a gridPoint_t pointer gp.
 *   Enter a loop until the desired number of gold piles are placed.
 *   Randomize the coordinates x and y within the bounds of the master grid.
 *   Create a new gridPoint_t object with the randomized coordinates.
 *   Check if the position on the master grid is valid for placing gold (denoted by '.').
 *   Set the point on both the master grid and working grid to '*'.
 *   Increment the counter i.
 *   Delete the gridPoint_t object and set gp to NULL.
 *   Repeat the loop until the desired number of gold piles are placed.  
 * 
 */

void game_place_gold(game_t* game);

/**************** game_move_player ****************/
/*
 * caller provides 
 *   valid pointer to game and address, amount move dx and dy and if a capital
 *   key was pressed    
 *
 * returns
 *  -1 invalid parameter
 *  0 no gold was picked up
 *  total amount of gold picked up if picked up gold
 * 
 * we do
 *  Check if the game parameter is valid, and if not, return -1.
    Get the player object based on the playerID parameter, and if not found, return -1.
    Get the current position of the player.
    Calculate the new position based on the given movement offsets dx and dy.
    Get the working map from the game.
    If is_jump is true, enter a loop:
        Update the player's position and check the value of the new position on the working map.
        Check if the new position is a valid spot to move to (dot, asterisk, or hash symbol).
        Check if the new position is occupied by another player.
        If the new position has gold, pick it up and increment the gold count.
        If the new position is not a valid spot or occupied by another player, return the gold count and break the loop.
    If is_jump is false:
        Check the value of the new position on the working map.
        Check if the new position is a valid spot to move to (dot, hash symbol, or asterisk).
        Check if the new position is occupied by another player.
        If the new position has gold, pick it up and store the gold amount.
        If the new position is not a valid spot or occupied by another player, return the stored gold amount.
    If the function reaches this point, return 0
 */

int game_move_player(game_t *game, addr_t *playerID, int dx, int dy, bool is_jump);

/**************** game_move_player ****************/
/*
 * caller provides 
 *   valid pointer to game, address, grid point, and username
 *
 * returns
 *  NULL if error with parameters or memory allocaton
 *  else valid pointer to playuer
 * 
 * we do
 *   Create a new player with the given parameters.
 *   Initialize the player's sight grid.
 *   Calculate the visibility grid based on the game's master and working grids.
 *   Update the sight grid of the player.
 *   Increment the number of players in the game.
 *   Add the player to the players array and the address-to-players hashtable.
 *   Return the newly added player.
 */
player_t *game_add_player(game_t *game, addr_t *address, gridPoint_t *gridPoint, char *uname);

/**************** game_add_spectator ****************/
/*
 * caller provides
 *  valid pointer to game and address
 * 
 * returns
 *  void returns early if issue with parameters
 * 
 * we do
 *  Create a new spectator object with the given address.
 *   If there is already a spectator, remove it.
 *   Set the game's spectator to the new spectator.
 */
void game_add_spectator(game_t *game, addr_t *address);


/**************** game_remove_player ****************/
/* caller provides
 *  valid pointer to game and address
 *
 * returns
 *  void --> returns early if issue with params
 * 
 * we do
 *  set boolean of player associated with given address to false --> out of grid
 *  
 * */
void game_remove_player(game_t *game, addr_t *address);

/**************** game_summary ****************/
/*
 * caller provides
 *  valid pointer to game
 * 
 * returns
 *  string of alias score username for each player
 * 
 * we do
 *   Check if the game is not null. If it is null, return null and print an error message.
 *   Create an array called "summary" to store player summaries.
 *   Iterate through the players in the game.
 *   If a player is active (not null) and has a username, add the player to the "summary" array.
 *       Create a string called "summary_str" with enough memory to hold the summary.
 *   If memory allocation fails, return null and print an error message.
 *   Write "GAME OVER:" to the "summary_str".
 *   Iterate through the "summary" array.
 *   For each player, create a line string with their alias, score, and username.
 *       Concatenate the line to the "summary_str".
 *       Free the memory allocated for the line.
 *   Return the "summary_str". Remember to free the allocated memory later. 
 */

char *game_summary(game_t *game);

/**************** game_update_sight_grid ****************/
/*
 * caller provides
 *  valid pointer to game and address of player
 *
 * returns
 *  void returns early if issues with program
 * 
 * we do
 *   Check if the game or address is null. If either is null, return and print an error message.
 *   Get the player from the game using the address.
 *   If the player is null, return and print an error message.
 *   Create a visibility grid ("vis") using the player's sight grid, the master grid, and the working grid.
 *   Set the player's current position in the visibility grid as '@'.
 *   Set the player's sight grid to the visibility grid.
 *  
 */

void game_update_sight_grid(game_t *game, addr_t *address);

/**************** game_redraw_working_grid ****************/
/*
 * caller provides 
 *  valid pointer to game and address
 *
 * returns
 *  void --> early if invalid params or mem issues
 * 
 * we do
 *   Check if the game is null. If it is null, return and print an error message.
 *   Get the master grid from the game.
 *   Create a new working grid by making a copy of the master grid.
 *   Iterate over each player in the game:
 *       Get the player's alias and position.
 *       If the player is alive, set the corresponding point in the working grid with the player's alias.
 *   Get the map from the game.
 *   Set the working grid of the map to the newly created working grid..
 * 
 */

void game_redraw_working_grid(game_t *game);

/******** Getter functions ********/

/* extracts the map from the game */
map_t* game_get_map(game_t* game);

/* extracts the working grid from the game */
grid_t *game_get_working_grid(game_t *game);

/* extracts the master grid from the game */
grid_t *game_get_master_grid(game_t *game);

/* extracts the number of players from the game */
int game_get_num_players(game_t *game);

/* extracts the maximum number of players from the game */
int game_get_max_players(game_t *game);

 /* extracts the number of removed players from the game */
int game_get_num_removed_players(game_t *game);

 /* extracts the number of gold piles from the game */
int game_get_num_gold_piles(game_t *game);

 /* extracts the number of active gold from the game */
int game_get_gold_active(game_t *game);

 /* extracts the player from the game */
player_t *game_get_player_from_id(game_t *game, int playerID);

 /* extracts the player from the game */
player_t *game_get_player(game_t *game, addr_t *address);

 /* extracts the spectator from the game */
player_t *game_get_spectator(game_t *game);

void game_remove_spectator(game_t *game);



#endif // !__GAME_H
