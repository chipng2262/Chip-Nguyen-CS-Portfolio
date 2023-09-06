/**
 * player.h - header file for player module
 * 
 * The player module provides functionalities to create, manage, and retrieve information about players in the game.
 * The player_t struct represents a player, and it holds the player's sightgrid, score, position, alive state, username, 
 * address, and playerID. Player.c supports all the necessary methods related to these player attributes.
 * 
 * Gitters, Spring 2023
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include "../support/message.h"
#include "gridPoint.h"
#include "grid.h"

typedef struct player player_t;

/**************** player_new ****************/
/* Create a new player instance.
 *
 * Caller provides:
 *   name: the name of the player (can be NULL).
 *   address: a valid address for the player.
 *   pos: a valid gridPoint representing the player's position.
 *   ID: the ID of the player.
 * We return:
 *   pointer to the created player instance on success;
 *   NULL if memory allocation fails.
 * Notes:
 *   The caller is responsible for freeing the allocated memory using player_delete().
 */
player_t *player_new(char *name, addr_t *, gridPoint_t *pos, int ID);

/**************** player_delete ****************/
/* Delete a player instance and free associated memory.
 *
 * Caller provides:
 *   player: a valid player instance to delete.
 * Notes:
 *   This function will free memory allocated for the player and its attributes.
 */
void player_delete(player_t* player);

/**************** player_earn_gold ****************/
/* Add gold to a player's score.
 *
 * Caller provides:
 *   player: a valid player instance.
 *   gold: the amount of gold to add to the player's score.
 * We do:
 *   The player's score will be increased by the provided amount of gold.
 */
void player_earn_gold(player_t *, int gold);

/**************** player_move ****************/
/* Move a player by a given offset in the x and y directions.
 *
 * Caller provides:
 *   player: a valid player instance.
 *   dx: the offset in the x direction.
 *   dy: the offset in the y direction.
 * We do:
 *   The player's position will be updated by adding the given offsets to its current position.
 */
void player_move(player_t*, int dx, int dy);

/**************** player_get_id ****************/
/* Get the ID of a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * Returns:
 *   the player's ID if the player is valid.
 */
int player_get_id(player_t* player);

/**************** player_get_pos ****************/
/* Get the position of a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * Returns:
 *   a pointer to the player's position if it exists;
 *   NULL if the player or the position is NULL.
 */
gridPoint_t *player_get_pos(player_t* player);

/**************** player_get_addr ****************/
/* Get the address associated with a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   a pointer to the player's address if it exists;
 *   NULL if the player or the address is NULL.  
 */
addr_t *player_get_addr(player_t* player);

/**************** player_get_score ****************/
/* Get the score of a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   the player's score if the player is valid;
 *   0 if the player is NULL.
 */
int player_get_score(player_t* player);

/**************** player_get_alias ****************/
/* Get the alias of a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   the alias character corresponding to the player's ID if the player is valid;
 *   'x' if the player is NULL.
 */
char player_get_alias(player_t* player);

/**************** player_get_sightgrid ****************/
/* Get the sight grid associated with a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   a pointer to the player's sight grid if it exists;
 *   NULL if the player or the sight grid is NULL.
 */
grid_t *player_get_sightgrid(player_t* player);

/**************** player_set_sightgrid ****************/
/* Set the sight grid for a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 *   grid: a valid grid to set as the player's sight grid.
 * We do:
 *   The player's sight grid will be updated with the provided grid.
 *   The caller should ensure that the grid is valid and remains accessible throughout the player's lifetime.
 */
void player_set_sightgrid(player_t *, grid_t *);

/**************** player_is_alive ****************/
/* Check if a player is alive.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   true if the player is alive;
 *   false if the player is NULL or not alive.
 */
bool player_is_alive(player_t *);

/**************** player_set_alive ****************/
/* Set the alive state for a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 *   alive: a boolean value indicating the alive state of the player.
 * We do:
 *   The player's alive state will be updated with the provided value.
 */
void player_set_alive(player_t *player, bool alive);

/**************** player_get_username ****************/
/* Get the username of a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 * We return:
 *   a pointer to the player's username if it exists;
 *   NULL if the player or the username is NULL.
 */
char *player_get_username(player_t *player);

/**************** player_set_username ****************/
/* Set the username for a player.
 *
 * Caller provides:
 *   player: a valid player instance.
 *   new_username: a valid string to set as the player's username.
 * We do:
 *   The player's username will be updated with the provided string.
 *   The caller should ensure that the new_username remains accessible throughout the player's lifetime.
 */
void player_set_username(player_t *player, char *new_username);

#endif // !__PLAYER_H