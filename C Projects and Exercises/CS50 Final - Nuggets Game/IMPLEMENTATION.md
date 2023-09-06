# CS50 Nuggets
## Implementation Spec
### Team Gitters, Spring 2023

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions of the Nuggets game.

## Plan for division of labor

- Warren and Ethan:
  + Wrote grid, gridPoint, and game structs
  + Implemented game logic such as gold operations, adding and removing of players and spectators, game summary, and player movement.
  + Helped with debugging and documentation.
  + In charge of Scrum.

- Chip:
  + Ensured the full implementation of functional requirements and message protocols in accordance with the Requirements Specs through.
  + Built the client and the core architecture of the server.
  + Helped with debugging and documentation. 
  
- Rishav:
  + In charge of version controls and Git flow.
  + Developed and debugged visibility, game logic, and other core data structures.

### Data structures
To assist the implementation of Nuggets, we created these new data structures:

- `game_t` (in `game` module): Stores information about the game status such as list of players, spectator, the game map, and all relevant values for gold piles.
```c
typedef struct game {
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
```

- `grid_t` (in `grid` module): Represents and stores information about the map that players move on.
```c
typedef struct grid {
  char* grid_str;
  int width;
  int height;
} grid_t;
```

- `gridpoint_t` (in `gridPoint` module): Stores an x and y coordinate to keep track of points on the grid.
```c
typedef struct gridPoint {
  int x;
  int y;
} gridPoint_t;
```

- `map_t` (in `map` module): Stores two grids: the master grid, which stores the whole grid, and the working grid, which represents current visibility of a player or spectator.
```c
typedef struct map {
  grid_t *master_grid;
  grid_t *working_grid;
} map_t;
```

- `player_t` (in `player` module): Stores information about a player in the game. A spectator has a null username.
```c
typedef struct player {
  grid_t *sight_grid;
  gridPoint_t *position;
  int score;
  int playerID;
  addr_t *address;
  char *username;
  bool is_alive;
} player_t;
```

### Definition of function prototypes
These functions are used to assist the implementation of the server, which handles most of the game logic including movements, updating visibility, picking up gold, and so on. Functions from the `message` module were extensively used to handle communications between client and server.

#### game.c
```c
typedef struct game game_t;

/* creates a new game structure */
game_t *game_new(char *map_file, int gold_num_piles, int gold_total);

/* deletes a game structure */
void game_delete(game_t *game);

/* handles picking up a gold pile and giving gold to the player */
int game_pickup_gold(game_t *game, addr_t *playerID);

/* places gold piles on the map */
void game_place_gold(game_t* game);

/* handles player movement */
int game_move_player(game_t *game, addr_t *playerID, int dx, int dy, bool is_jump);

/* adds a player to the game at a point */
player_t *game_add_player(game_t *game, addr_t *address, gridPoint_t *gridPoint, char *uname);

/* adds a new spectator to the game, kicking out the current spectator if needed */
void game_add_spectator(game_t *game, addr_t *address);

/* removes a player from the game via address */
void game_remove_player(game_t *game, addr_t *address);

/* removes a player from the game via playerID */
void game_remove_player_by_id(game_t *, char playerID);

/* removes the game spectator */
void game_remove_spectator(game_t *game);

/* gets the map from the game */
map_t* game_get_map(game_t* game);

/* gets the working grid from the game */
grid_t *game_get_working_grid(game_t *game);

/* gets the master grid from the game */
grid_t *game_get_master_grid(game_t *game);

/* gets the number of players from the game */
int game_get_num_players(game_t *game);

/* gets the maximum number of players from the game */
int game_get_max_players(game_t *game);

/* gets the number of removed players from the game */
int game_get_num_removed_players(game_t *game);

/* gets the number of gold piles from the game */
int game_get_num_gold_piles(game_t *game);

/* gets the number of active gold from the game */
int game_get_gold_active(game_t *game);

/* gets a player from a playerID */
player_t *game_get_player_from_id(game_t *game, int playerID);

/* gets a player from an address */
player_t *game_get_player(game_t *game, addr_t *address);

/* gets the spectator from the game */
player_t *game_get_spectator(game_t *game);

/* returns a formatted string game summary */
char *game_summary(game_t *game);

/* updates the sightgrid for a player */
void game_update_sight_grid(game_t *game, addr_t *address);

/* redraws the working grid based on visibility */
void game_redraw_working_grid(game_t *game);
```

#### game.c pseudocodes:
- `game_new`: 
```
 Allocate memory for the game struct and check for any allocation errors.
 Allocate memory for the players and removed_players arrays and check for errors.
 Load the map from a file and create master_grid and working_grid.
 Create a new map struct using the master_grid and working_grid.
 Initialize various fields in the game struct.
 Place gold onto the grid.
 Return the initialized game struct.
```

- `game_delete`:
```
free all possible memory associated with the game struct
```

- `game_pickup_gold`:
```
 Check if the game parameter is valid, and if not, return an error.
 Get the player from the given playerID address, and if not found, return an error.
 Calculate the amount of gold to be picked up based on the number of gold piles and the remaining gold in the game.
 Reduce the active gold amount in the game by the calculated gold amount.
 Give the gold to the player by calling the player_earn_gold function.
 Set the corresponding point on the master grid to '.'.
 Decrement the number of gold piles in the game.
 Return the amount of gold picked up.
```

- `game_place_gold`:
```
 Check if the game parameter is valid, and if not, return.
 Initialize a counter i to 0.
 Create a gridPoint_t pointer gp.
 Enter a loop until the desired number of gold piles are placed.
 Randomize the coordinates x and y within the bounds of the master grid.
 Create a new gridPoint_t object with the randomized coordinates.
 Check if the position on the master grid is valid for placing gold (denoted by '.').
 Set the point on both the master grid and working grid to '*'.
 Increment the counter i.
 Delete the gridPoint_t object and set gp to NULL.
 Repeat the loop until the desired number of gold piles are placed.  
```

- `game_move_player`:
```
    Check if the game parameter is valid, and if not, return -1.
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
```

- `game_add_player`:
```
 Create a new player with the given parameters.
 Initialize the player's sight grid.
 Calculate the visibility grid based on the game's master and working grids.
 Update the sight grid of the player.
 Increment the number of players in the game.
 Add the player to the players array and the address-to-players hashtable.
 Return the newly added player.
```

- `game_add_spectator`:
```
 Create a new spectator object with the given address.
 If there is already a spectator, remove it.
 Set the game's spectator to the new spectator.
```

- `game_remove_player`:
```
 Set boolean is_alive to true.
```

- `game_summary`:
```
    Check if the game is not null. If it is null, return null and print an error message.
    Create an array called "summary" to store player summaries.
    Iterate through the players in the game.
    If a player is active (not null) and has a username, add the player to the "summary" array.
        Create a string called "summary_str" with enough memory to hold the summary.
    If memory allocation fails, return null and print an error message.
    Write "GAME OVER:" to the "summary_str".
    Iterate through the "summary" array.
    For each player, create a line string with their alias, score, and username.
        Concatenate the line to the "summary_str".
        Free the memory allocated for the line.
    Return the "summary_str". Remember to free the allocated memory later. 
```

- `game_update_sight_grid`:
```
 Check if the game or address is null. If either is null, return and print an error message.
 Get the player from the game using the address.
 If the player is null, return and print an error message.
 Create a visibility grid ("vis") using the player's sight grid, the master grid, and the working grid.
 Set the player's current position in the visibility grid as '@'.
 Set the player's sight grid to the visibility grid.
```
- `game_redraw_working_grind`:
```
 Check if the game is null. If it is null, return and print an error message.
 Get the master grid from the game.
 Create a new working grid by making a copy of the master grid.
 Iterate over each player in the game:
        Get the player's alias and position.
        If the player is alive, set the corresponding point in the working grid with the player's alias.
    Get the map from the game.
    Set the working grid of the map to the newly created working grid..
```

The following modules are helper modules to game.c:

#### grid.c
```c
typedef struct grid grid_t;

/* creates a new grid structure from a grid string */
grid_t *grid_new(char *grid_str, int width, int height);

/* loads a grid from a string filename */
grid_t *grid_load(const char *filename);

/* returns a copy of the passed grid */
grid_t *grid_copy(grid_t* grid);

/* deletes a grid structure */
void grid_delete(grid_t* grid);

/* sets a point on the grid to a new character */
void grid_set_point(grid_t *, gridPoint_t *, char state);

/* gets the character at a point on the grid */
char grid_get_point(grid_t *, gridPoint_t *);

/* builds a visibility grid */
grid_t *grid_get_player_visibility(grid_t *master, grid_t *working, grid_t *player, gridPoint_t *);

/* checks if a point is visible from a starting position on the grid */
static bool is_visible(grid_t *grid, gridPoint_t *player, gridPoint_t *point);

/* gets height and width of a grid */
gridPoint_t *grid_get_dimensions(grid_t *);

/* gets the grid height */
int grid_get_height(grid_t *);

/* gets the grid width */
int grid_get_width(grid_t *);

/* gets the grid string from a grid */
char *grid_to_string(grid_t *);

/* gets the index in the grid string of a point on the grid */
int grid_get_point_ind(grid_t *, gridPoint_t *);
```

#### gridPoint.c
```c
typedef struct gridPoint gridPoint_t;

/* creates a new gridpoint structure */
gridPoint_t* gridPoint_new(int x, int y);

/* sets new x and y values for a gridpoint */
bool gridPoint_set(gridPoint_t* point, int x, int y);

/* gets gridpoint x value */
int gridPoint_getX(gridPoint_t* point);

/* gets gridpoint y value */
int gridPoint_getY(gridPoint_t* point);

/* sets new value for x coordinate */
void gridPoint_setX(gridPoint_t* gp, int val);

/* sets new value for y coordinate */
void gridPoint_setY(gridPoint_t* gp, int val);

/* deletes gridpoint structure */
void gridPoint_delete(gridPoint_t* point);
```

#### map.c
```c
typedef struct map map_t;

/* creates a new map structure from master and working grids */
map_t *map_new(grid_t *master, grid_t *working);

/* deletes map structure */
void map_delete(map_t *);

/* gets working grid from map */
grid_t *map_get_working_grid(map_t* map);

/* gets master grid from map */
grid_t *map_get_master_grid(map_t* map);

/* sets a new working grid for a map */
void map_set_working_grid(map_t* map, grid_t* grid);

/* sets a new master grid for a map */
void map_set_master_grid(map_t* map, grid_t* grid);
```

##### player.c
```c
typedef struct player player_t;

/* creates a new player structure from a name, address, gridpoint, and ID */
player_t *player_new(char *name, addr_t *, gridPoint_t *pos, int ID);

/* deletes a player structure */
void player_delete(player_t* player);

/* increments player's score by a gold amount */
void player_earn_gold(player_t *, int gold);

/* changes position of the player */
void player_move(player_t*, int dx, int dy);

/* gets ID associated with the player */
int player_get_id(player_t* player);

/* gets the grid point positon of a player */
gridPoint_t *player_get_pos(player_t* player);

/* gets the address of a given player */
addr_t *player_get_addr(player_t* player);

/* gets score of a given player */
int player_get_score(player_t* player);

/* gets the alias as displayed on the grid of a given player */
char player_get_alias(player_t* player);

/* gets the sight grid of a player which is the players visibility */
grid_t *player_get_sightgrid(player_t* player);

/* sets the sight grid of a given player to a new grid based on updated visibility */
void player_set_sightgrid(player_t *, grid_t *);

/* checks if player is still participating in the game */
bool player_is_alive(player_t *);

/* used when a player quits to signify that they have left the game */
void player_set_alive(player_t *player, bool alive);

/* gets the username associated with a given player */
char *player_get_username(player_t *player);

/* sets the username of a given player */
void player_set_username(player_t *player, char *new_username);
```

### Client implementation
**client.c**
```c
/* function to intialize screen using ncurses */
static void initializeScreen();

/* Function to handle keystroke inputs from the client */
static bool handleInput(void* arg);

/* Function to handle incoming messages from the client */
static bool handleMessage(void* arg, const addr_t from, const char* message);
```

### Pseudo code (client)
The main client will run as follow:
```
  parse the command line and validate parameters
  if playerName is provided set player mode to player, else set to spectate
  loop through messages:
    if receive valid input keystrokes from clients, send keystroke message to server and refresh screen.
    if receive message from server, parse message and handle message
  stop loop if handleMessage() or handleInpur return true
```

where *initializeScreen*:
```
 initialize the display window of the game using ncurses
 get the dimension of the current screen
 while the display window is too small to contain the full map:
  prompt player to enlarge screen and press enter after
 refresh the display window
```

where *handleInput*:
```
 gets the key the client just pressed
 if spectator, only consider key Q for Quit
 if client:
  if key is valid: send message to server
  else: print Unknown keystroke to play screen.
 refresh and return true;
```

where *handleMessage*:
```
 gets and parses message from server
 if:
    message is "OK alias":
      get alias and return false
    message is "GRIND nrows ncols":
      call initializeScreen to draw the display window
    message is "GOLD n p r":
       display updated gold status of player
    message is "DISPLAY\nstring"
      print out the grid represented by the string
    message is "QUIT message":
      end curses and return true to prompt loop to stop.
    message is "ERROR "
      print the error in stderr
  return false
```

### Server implementation
**server.c**
```c
/* Function to handle incoming messages from the client */
static bool handleMessage(void* arg, const addr_t from, const char* message);

/* Helper function to handle play messages from client */
static bool handleMessagePlay(game_t* game, addr_t* address, char* playerName);

/* Helper function to handle keystrokes from client. */
static bool handleMessageKey(game_t* game, addr_t* address, const char* key);

/* Helper function to handle spectate messages from client. */
static bool handleMessageSpectate(game_t* game, addr_t* address);

/* Helper function to process the name of a player */
static void processName(char* playerName);
```

### Pseudo code (server)
The main server will run as follow:
```
  parse the command line, validate parameters, and intilize new game
  loop through message:
    handle each message according to its type
    update spectator and current players on new changes in the game via messages
  stop loop if handleMessage() or handleInput return true
```

where *handleMessage*
```
 gets the address of sender
 determines the type of message
 calls other functions to handle message based on type
```

where *handleMessagePlay*
```
 if max number of players is reached, send QUIT message to client. 
 else:
  process new player's name
  generate a random location for new player
  create new player and drop player to location
  send all necessary messages to new player's client to set up initial game
```

where *handleMessageSpectate*
```
  if there's already a spectator, replace old spectator with new spectator
  set working grid for spectator
  send all necessary messages to spectator's client to set up game display
```

where *handleMessageKey*
```
  if key was sent from spectator and key is Q, remove spectator and send QUIT message. else:
    get key sent by the sender
    update location of sender
    if sender picked up gold:
      loop: send new gold information to sender, all other players, and spectator
    loop: send new display to all current clients and spectatir
    if game ends (gold_active = 0):
      loop: send all current players and spectator the scoreboard of all players
      return true to stop message loop.
```

where *proccessName*
```
  loop through characters in name MaxNameLength times:
    if character is neither graphic nor blank:
      replace character with _
    else keep character the same
    return the processed name
```

## Testing Plan

### Unit Testing
- Each helper module was tested before integrating with the main client and server using driver files, which passed both valid and invalid/null arguments to the module functions. All functions were implemented with defensive programming to ensure bad variables do not corrupt/crash the program.
- The server and client were tested with print statements to make sure all messages were correctly produced and formatted. The correct format of messages is crucial to communications between the client and server. 
- Valgrind was used to detect and fix memory leaks and errors 

### Integration and System Testing
- Nuggets, as a complete, integrated programs, was tested using these tests below. All errors known through testing were fixed:
1. Tested server and client with invalid/incomplete/insufficient arguments.
2. Tested client with invalid names (longer than 50 characters, all blank "    ")
3. Tested edge cases during game such as 27th player or 2nd spectator trying to join, seeing in obstructed corners, etc...
- Valgrind was used to detect memory errors during integration testing.

### System Testing
For system testing we test the server and client together by starting both modules to run the game. We send logging to output test files to identify errors. We also use observations of the running game to identify issues. If problems arise, we identify if the problem is within the game module or client/server message handling, and we proceed to debug from there.

---
## Limitations
- In game_move_player, we have memory allocation issue with gridPoint_new() that we were not able to figure out.
- There are sometimes issues with using our own map for the game, despite our map following the criteria of a valid map. We assume that there are issues with out text file. Even when we had a single, big rectangular room, the issues with moving a player still persisted.