/**
 * server.c - c file for nuggets project
 *
 * Nuggets server module implements the game server. The server communicates back and forth with clients via messages based on given keystrokes. 
 * It handles updating the game state for all clients. 
 * 
 * Gitters, Spring 2023
 */




#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../shared/game.h"
#include "../shared/grid.h"
#include "../shared/gridPoint.h"
#include "../shared/player.h"
#include "../support/message.h"
#include "../shared/map.h"

const int MaxNameLength = 50;   // max number of chars in playerName
const int MaxPlayers = 26;      // maximum number of players
const int GoldTotal = 250;      // amount of gold in the game
const int GoldMinNumPiles = 10; // minimum number of gold piles
const int GoldMaxNumPiles = 30; // maximum number of gold piles

// function prototypes
static bool handleMessage(void* arg, const addr_t from, const char* message);
static bool handleMessagePlay(game_t* game, addr_t* address, char* playerName);
static bool handleMessageKey(game_t* game, addr_t* address, const char* key);
static bool handleMessageSpectate(game_t* game, addr_t* address);
static void processName(char* playerName);

/**************** main ****************/
 /**
  * Function parses arguments, intialize the game, and start looping through messages
 */
int main(int argc, char* argv [])
{
    if(argc != 3 && argc != 2){
    fprintf(stderr, "Error: Invalid number of parameters given. Expects .txt, [Seed]");
    exit(1);
    }

    int seed;
    if(argc == 3) {
        if(sscanf(argv[2], "%d", &seed) <= 0){
            fprintf(stderr, "Error: expects second value to be a positive integer");
            exit(2);
        }
        srand(seed);
        }
        else{
            srand(getpid());
    }

    FILE* fp;
    char* mapFile = argv[1];
    if((fp = fopen(mapFile, "r")) == NULL){
        fprintf(stderr, "Error: unopenable file");
        exit(4);
    }
    fclose(fp);

    int port = message_init(stdout);
    if (port == 0) {
        fprintf(stderr, "Error: cannot initialize port");
        exit(5);
    }

    int numGoldPiles = rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles;
    
    game_t* game = game_new(mapFile, numGoldPiles, GoldTotal);

    if(game == NULL)
    {
        fprintf(stderr, "Error: cannot allocate memory");
        exit(6);
    }

    bool ok = (!message_loop(game, 0, NULL, NULL, handleMessage));

    if (!ok) {
      exit(7);
    }

    message_done();
    return ok? 0 : 1;
}

/**************** handleMessage ****************/
 /**
  * Function to handle incoming messages from the server
  * Caller provides:
  *    a valid address a client sender and a valid message
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop
 */
static bool handleMessage(void* arg, const addr_t from, const char* message)
{
    if(message == NULL) {
        return false;
    }

    addr_t* addressPointer = malloc(sizeof(addr_t));
    if(addressPointer == NULL) {
      fprintf(stderr, "Error: memory for address pointer");
      return false;
    }

    *(addressPointer) = from;
    game_t* game = (game_t*)(arg);
    
    // handle messages 
    if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
      char* playerName = (char *)(message + strlen("PLAY "));
      return handleMessagePlay(game, addressPointer, playerName);
    }

    if(strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
        return handleMessageSpectate(game, addressPointer);
    }
    if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {
        const char* key = message + strlen("KEY ");
        return handleMessageKey(game, addressPointer, key);
    }
    return false;
}

/**************** handleMessagePlay ****************/
 /**
  * Helper function to handle play messages.
  * Function sends all necessary messages to client to set up the initial display of the game
  * Caller provides:
  *    a valid address a client sender and a valid playerName of the sender
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop
 */
bool handleMessagePlay(game_t* game, addr_t* address, char* playerName)
{
    if (game_get_num_players(game) == MaxPlayers) {
        message_send(*(address), "QUIT Game is full: no more players are being accepted at this time.\n");
        return false;
    }

    // check if player's name is empty
    bool isNameEmpty = true;
    for (int i = 0; i < strlen(playerName); i++) {
        // found a non-space character, so name is not empty
        if (!isspace(playerName[i])) {
            isNameEmpty = false;
            break;
        }
    }
    if (isNameEmpty == true) {
        message_send(*(address), "QUIT Sorry - you must provide player's name.\n");
        return false;
    }

    // process playerName
    processName(playerName);

    // generate a random location for the new player
    gridPoint_t* dimensions = grid_get_dimensions(game_get_master_grid(game));
    int y = gridPoint_getY(dimensions);
    int x = gridPoint_getX(dimensions);
    gridPoint_delete(dimensions);
    gridPoint_t* randomStart;

    while(true) {
        int rand_x = rand() % x;
        int rand_y = rand() % y;
        randomStart = gridPoint_new(rand_x, rand_y);
        char state = grid_get_point(game_get_working_grid(game), randomStart);
        if(state == '.'){
            break;
        }
        gridPoint_delete(randomStart);
        randomStart = NULL;
    }
    
    // add new player
    player_t* addedPlayer = game_add_player(game, address, randomStart, playerName);

    // send OK message
    char alias = player_get_alias(addedPlayer);
    char* okMessage = malloc(5*sizeof(char));
    sprintf(okMessage, "%s%c", "OK ", alias);
    okMessage[3] = alias;
    message_send(*(address), okMessage);
    free(okMessage);

    char* grid_str = malloc(sizeof(char)*x*y);
    if(grid_str == NULL) {
        fprintf(stderr, "Error: cannot allocate memory");
        player_delete(addedPlayer);
    }

    for(int i = 0; i < x*y; i++) {
        grid_str[i] = ' ';
    }

    grid_str[(x*y)-1] = '\0';
    grid_t* playerGrid = grid_new(grid_str, x, y);

    if(playerGrid == NULL) {
        player_delete(addedPlayer);
    }
    
    // send GRID message
    char* gridMessage = malloc(sizeof(char) * (strlen("GRID ") + 10));
    sprintf(gridMessage, "GRID %d %d", y, x);
    message_send(*(address), gridMessage);
    free(gridMessage);

    // set sight grid of the player
    player_set_sightgrid(addedPlayer, playerGrid);
    game_update_sight_grid(game, address);

    // send DISPLAY
    char* command = "DISPLAY\n";
    const char* pGrid = grid_to_string(player_get_sightgrid(addedPlayer));
    char* displayMessage = malloc(sizeof(char) * (strlen(command)) + strlen(pGrid)*sizeof(char) + 10);
    sprintf(displayMessage, "%s%s", command, pGrid);
    message_send(*(address), displayMessage);
    game_redraw_working_grid(game);
    player_t* spec = game_get_spectator(game);

    if(spec != NULL) {
        char* spectatorGrid = grid_to_string(game_get_working_grid(game));
        sprintf(displayMessage, "%s%s", command, spectatorGrid);
        message_send(*(player_get_addr(spec)), displayMessage);
    }

    free(displayMessage);

    // send initial GOLD
    char* scoreMessage = malloc(100*sizeof(char));
    int numGoldRemaining = game_get_gold_active(game);
    sprintf(scoreMessage, "%s %d %d %d", "GOLD", 0,0, numGoldRemaining);

    message_send(*(address), scoreMessage);
    free(scoreMessage);
    
    return false;
}

/**************** handleMessageSpectate ****************/
 /**
  * Helper function to handle play messages.
  * Function sends all necessary messages to client to set up the initial display of the game
  * Caller provides:
  *    a valid address a client sender and a valid playerName of the sender
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop
 */
bool handleMessageSpectate(game_t* game, addr_t* address)
{
    player_t* formerSpectator = game_get_spectator(game);
    if(formerSpectator != NULL)
    {
        message_send(*player_get_addr(formerSpectator), "QUIT You have been replaced by a new spectator.");
        game_remove_spectator(game);
    }
    game_add_spectator(game, address);

    grid_t* workingGrid = game_get_working_grid(game);
    // player_set_sightgrid(formerSpectator, workingGrid);

    gridPoint_t* dimensions = grid_get_dimensions(game_get_master_grid(game));
    int y = gridPoint_getY(dimensions);
    int x = gridPoint_getX(dimensions);

    // send GRID message
    char* gridMessage = malloc(sizeof(char) * (strlen("GRID ") + 10));
    sprintf(gridMessage, "GRID %d %d", y, x);
    message_send(*(address), gridMessage);
    free(gridMessage);

    char* command = "DISPLAY\n";
    char* displayMessage = malloc(sizeof(char) * strlen(command) + strlen(grid_to_string(workingGrid))*sizeof(char));
    sprintf(displayMessage, "%s%s", command, grid_to_string(workingGrid));
    message_send(*(address), displayMessage);
    free(displayMessage);
    
    char* scoreMessage = malloc(100*sizeof(char));
    int numGoldRemaining = game_get_gold_active(game);
    sprintf(scoreMessage, "%s %d %d %d", "GOLD", 0,0, numGoldRemaining);
    message_send(*(address), scoreMessage);
    free(scoreMessage);
    
    return false;
}

/**************** handleMessageKey ****************/
 /**
  * Helper function to handle keystrokes from client
  * Function implements logic to move client, let client pick up gold, and quit if all golds are collected
  * Caller provides:
  *    a valid address a client sender and a valid keystroke
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop - when all golds have been collected
 */
bool handleMessageKey(game_t* game, addr_t* address, const char* key)
{

    player_t* spectator = game_get_spectator(game);
    player_t* sender = game_get_player(game, address);

    // if the spectator sent the message
    if (spectator != NULL && sender == NULL) {
      if (strncmp(key, "Q", strlen("Q")) == 0) {
        addr_t* spectatorAddress = player_get_addr(spectator);
        message_send(*spectatorAddress, "QUIT Thanks for watching!");
        game_remove_spectator(game);
      }
    }

    else{
        char key_char = key[0];
        int gold = 0;

        bool is_jump = false;
        switch(key_char)
        {
        // Player quits
            case 'Q':
            message_send(*player_get_addr(sender), "QUIT Thanks for playing!"); 
            game_remove_player(game, player_get_addr(sender));
            break;

            // Player moves 1 step
            case 'h':
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), -1, 0, is_jump); 
                break;
            case 'l': 
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), 1, 0, is_jump); 
                break;
            case 'j': 
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), 0, 1, is_jump); 
                break;
            case 'k':
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), 0, -1, is_jump); 
                break;
            case 'y':
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), -1, -1, is_jump); 
                break;
            case 'u': 
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), 1, -1, is_jump); 
                break;
            case 'b': 
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), -1, 1, is_jump); 
                break;
            case 'n': 
                is_jump = false;
                gold = game_move_player(game, player_get_addr(sender), 1, 1, is_jump); 
                break;

            // Move until you cannot anymore
            case 'H': 
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), -1, 0, is_jump); 
                break;
            case 'L': 
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), 1, 0, is_jump); 
                break;
            case 'J':
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), 0, 1, is_jump); 
                break;
            case 'K':
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), 0, -1, is_jump); 
                break;
            case 'Y': 
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), -1, -1, is_jump); 
                break;
            case 'U':
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), 1, -1, is_jump); 
                break;
            case 'B':
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), -1, 1, is_jump); 
                break;
            case 'N': 
                is_jump = true;
                gold = game_move_player(game, player_get_addr(sender), 1, 1, is_jump);
                break; 

            default:
            {
                char errorMessage[100];
                snprintf(errorMessage, 100, "ERROR Unknown keystroke.");
                message_send(*player_get_addr(sender), errorMessage);
            }
        }
        game_redraw_working_grid(game);

        // if gold is picked up
        if(gold > 0)
        {
            int collected;
            int purseTotal;
            int goldRemaining;

            collected = gold;
            goldRemaining = game_get_gold_active(game);
            printf("Gold collectedk : %d\n", collected);

            char* goldCommand = "GOLD";
            char* goldMessage = malloc(sizeof(char) * (strlen(goldCommand) + 20));

            // update gold information to all alive clients
            for (int i = 0; i < game_get_num_players(game); i++) {
                player_t* currPlayer = game_get_player_from_id(game, i);

                if (player_is_alive(currPlayer)) {
                    purseTotal = player_get_score(currPlayer);
                    // check if the current player is the sender
                    if (message_eqAddr(*player_get_addr(currPlayer), *player_get_addr(sender))) {
                        sprintf(goldMessage, "%s %d %d %d", goldCommand, collected, purseTotal, goldRemaining);
                        message_send(*player_get_addr(currPlayer), goldMessage);
                    }

                    else {
                        sprintf(goldMessage, "%s %d %d %d", goldCommand, 0, purseTotal, goldRemaining);
                        message_send(*player_get_addr(currPlayer), goldMessage);
                    }
                }
            }
                        
            if(spectator != NULL) {
                sprintf(goldMessage, "%s %d %d %d", goldCommand, 0, 0, goldRemaining);
                message_send(*player_get_addr(spectator), goldMessage);                
            }
            free(goldMessage);
        }

        // send new display to all new clients
        for(int i = 0; i < game_get_num_players(game); i++){
            player_t* currPlayer = game_get_player_from_id(game, i);

            if(player_is_alive(currPlayer))
            {
                game_update_sight_grid(game, player_get_addr(currPlayer));
                char* command = "DISPLAY\n";
                const char* currGrid = grid_to_string(player_get_sightgrid(currPlayer));
                char* displayMessage = malloc((sizeof(char)) * (strlen(command) + strlen(currGrid) + 2));
                sprintf(displayMessage, "%s%s", command, currGrid);
                message_send(*player_get_addr(currPlayer), displayMessage);
                free(displayMessage);
            }
        }

        if(spectator != NULL)
        {
            const char* currG = grid_to_string(game_get_working_grid(game));
            char* command = "DISPLAY\n";
            char* displayM = malloc((sizeof(char)) * (strlen(command) + strlen(currG) + 2));

            sprintf(displayM, "%s%s", command, currG);
            message_send(*(player_get_addr(spectator)), displayM);
            free(displayM);
        }
    }

    // loop through all player and send summary info if game ends (all golds are collected)
    if (game_get_gold_active(game) == 0) {
        char* summary = game_summary(game);
        char* quitMessage = malloc(sizeof(char) * (strlen("QUIT ") + strlen(summary) +10 ));
        sprintf(quitMessage, "QUIT %s", summary);

        for(int i = 0; i < game_get_num_players(game); i++){
            player_t* currPlayer = game_get_player_from_id(game, i);
            if(player_is_alive(currPlayer)) {
                message_send(*player_get_addr(currPlayer), quitMessage);
            }
        }
        free(quitMessage);
        return true;
    }
    return false;
}

/**************** processName ****************/
 /**
  * Helper function to process the name of a player
  * Function constrains the length of playerName and converts all non-space, non-graphic strings into _
  * Caller provides:
  *    a valid address a client sender and a valid playerName of the sender
 */
static void processName(char* playerName) {
    char* formattedName = malloc(strlen(playerName) + 1);
    int i;
    for (i = 0; i < strlen(playerName); i++) {
        if (i < MaxNameLength) {
            if (!isgraph(playerName[i]) && !isblank(playerName[i])) {
                formattedName[i] = '_';
            } 
            else {
                formattedName[i] = playerName[i];
            }
        } else {
            formattedName[i] = '\0';
            break;
        }
    }
    formattedName[i] = '\0';
    strcpy(playerName, formattedName);
    free(formattedName);
}