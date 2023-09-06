/**
 * player.c - c file for nuggets project
 * 
 * Nuggets player module defines player struct and implements related player functions.
 * See player.h for detailed information
 * 
 * Gitters, Spring 2023
 */

#include "player.h"
#include <../libcs50/mem.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../support/message.h"
#include "grid.h"
#include <string.h>

extern const int MAX_NAME_LENGTH;

typedef struct player {
  grid_t *sight_grid;
  gridPoint_t *position;
  int score;
  int playerID;
  addr_t *address;
  char *username;
  bool is_alive;
} player_t;

player_t *player_new(char *name, addr_t *address, gridPoint_t *pos, int ID) {
  player_t *player = malloc(sizeof(player_t));
  if (player == NULL) {
    fprintf(stderr, "ERROR: Couldnt allocate memory.\n");
    return NULL;
  }
  printf("made new player\n");
  
  player->playerID = ID;
  player->address = address;
  player->is_alive = true;
  player->score = 0;
  player->position = pos;
  player->sight_grid = NULL;

  // allocate memory
  char* usernameCopy = malloc(sizeof(char) * (MAX_NAME_LENGTH + 1));

  // check if usernameCopy is NULL
  if (usernameCopy == NULL) {
    fprintf(stderr, "error: out of memory\n");
    return NULL;
  }
  
  // copy string
  if (name != NULL) {
  strcpy(usernameCopy, name);
  player->username = usernameCopy;
  }

  else player->username = NULL;
  return player;
}

void player_delete(player_t *player) {
  free(player->sight_grid);
  free(player->position);
  free(player->address);
  free(player->username);
  free(player);
}

void player_earn_gold(player_t *player, int gold) {
  player->score = player->score + gold;
}

grid_t *player_get_sightgrid(player_t *player) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting player's sight grid\n");
    return NULL;
  }
  return player->sight_grid;
}

void player_set_sightgrid(player_t *player, grid_t *grid) {
  if (player == NULL || grid == NULL) {
    fprintf(stderr, "ERROR: null player or grid when setting player's sight grid\n");
    return;
  }
  player->sight_grid = grid;
}


char *player_get_username(player_t *player) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting player's username\n");
    return NULL;
  }
  return player->username;
}

void player_set_username(player_t *player, char *new_username) {
  if (player == NULL || new_username == NULL) {
    fprintf(stderr, "ERROR: null player or username when setting player's username\n");
    return;
  }
  player->username = new_username;
}

int player_get_score(player_t *player) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting score\n");
    return 0;
  }

  return player->score;
}

addr_t *player_get_addr(player_t *player) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting address\n");
    return NULL;
  }

  return player->address;
}

char player_get_alias(player_t *player)
{
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting alias\n");
    return 'x';
  }

  return ((char)(player->playerID)) + 'A';
}

bool player_is_alive(player_t *player) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player when getting alive states\n");
    return false;
  }
  return player->is_alive;
}

void player_set_alive(player_t *player, bool alive) {
  if (player == NULL) {
    fprintf(stderr, "ERROR: null player or boolean when setting player's alive state.\n");
    return;
  }
  player->is_alive = alive;
}

int player_get_id(player_t* player){
  return player->playerID;
}

void player_move(player_t* player, int dx, int dy)
{
  gridPoint_t* gp = player->position;
  gridPoint_setX(gp,gridPoint_getX(gp) + dx);
  gridPoint_setY(gp,gridPoint_getY(gp) + dy);
}
gridPoint_t* player_get_pos(player_t* player)
{
  return player->position;
}