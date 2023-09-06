# CS50 Nuggets
## Design Spec
### Team Gitters, Spring 2023

This document references the [Requirements Spec](REQUIREMENTS.md) for the Nuggets game. Here we focus on the core subset:
- User interface
- Inputs and outputs
- Functional decomposition into modules
- Major data structures
- Testing plan

Our program consists of 3 main components: the client, the server, and the shared modules with helper functions and data structures to implement the game. 

### Client (player)

The *client* acts in one of two modes:

 1. *spectator*, in which it observes all players in the game against the entire game map. 
 2. *player*, in which the it plays the game by trying to collect as many gold as possible. 

#### User interface
- To initialize the client, the user must use the command:
```
./player hostname port [playername]
```

- The argument `playerName` is optional. If the client wants to join as a player, [playername] must be provided. If it wants to spectate the game, playername must be left blank. If invalid arguments are provided, the program exits with error message. 

### Inputs and output
- **Inputs**: the client takes hostname, port, and [playername] as inputs in the command line, as mentioned above to establish a connection to a server and initialize the mode of a client. Once connected to a server, the client takes keystrokes from players. 
- **Outputs**: the client uses ncurses to display the game, including the map (full or partial depending on play mode), the players and their movements, and the gold status of players.

#### Functional decomposition into modules
We anticipate the modules below for the client. We will also use the `message` module provided to handle receiving and sending messages between the client and server. 

- *main* parses arguments, intializes the connection address, determines player's mode (spectator or player), and loop through messages.
- *intializeScreen* uses ncurses to intialize and display the game to the player.
- *handleInput* handles keystroke inputs from clients and send keystroke messages to server.
- *handleMessage* handles messages sent from the server depending on the type of message.

#### Pseudo code for logic/algorithmic flow
The main client will run as follow:
```
  parse the command line and alidate parameters
  determine client's participation mode send "SPECTATE" if spectactor and "PLAY name" if player.
  loop through messages:
    if receive valid input keystrokes from clients, send keystroke message to server and refresh screen. 
    if receive message from server, parse message and handle message
  stop loop if handleMessage() or handleInpur return true
```
where *initializeScreen*:
```
 initialize the display window of the game
 set the location of cruses in ncurses
 whil the display window is too small to contain the full map:
  prompt player to enlarge screen and press enter after

```

and *handleInput*:
```
 gets the key the client just pressed
 if spectator, only consider key Q for Quit
 if client:
  if key is valid: send message to server
  else: print Unknown keystroke to play screen.
 refresh and return true;
```

and *handleMessage*:
```
 gets and parses message from server
 handles the message and update the player/spectator accordingly.
 
```

#### Major data structure
- The client is a simple program that does not rely on any complicated data structure. The program only defines a struct `gamestate_t` to store information about the game at any time. This 

### Server

#### User Interface
- To compile the server, we use the command below:
```
./server map.txt [seed]
```

### Inputs and output
- **Inputs**: as seen from the command line, the server takes a text file repsresentation of a valid game map and the optional seed controls randomness in generating random numbers useful for the server. 
- **Outputs**: the server will printout error messages to stderr and print the log all the messages exchanged between itself and the client.

#### Functional decomposition into modules
We anticipate the modules below for the server. We will also use the `message` module provided to handle receiving and sending messages between the client and server as well as our own modules stored in `shared` to handle game logic.

- *main* parses arguments, creat a new game, and loop through messages
- *handleMessage* parses messages received from clients and call other processing functions.
- *handleMessagePlay* initializes a new player and sends it all the necessary messages to play the game. 
- *handleMessageSpectate* initializes a new spectator and sends it all the necessary messages to observe the game.
- *handleMessageKey* handles keystrokes and send update messages to the spectator and all curent players of the game.
- *processName* to process the name of a player according to the Requirements Spec.

#### Pseudo code for logic/algorithmic flow
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
  create new player
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

### Major data structures

The major data structures used for server are as follows:

* game - Stores the status of the game.
* grid - Stores a grid data structure.
* gridPoint - Stores an x and y coordinate.
* map - Stores the original and working grids.
* player - Stores player information.

---

---

## Shared module

### Functional decomposition

#### game

This module contains functionality for handling game logic and operations.

#### grid

This module contains functionality for storing and handling the grid struct.

#### gridPoint

This module contains functionality for storing and handling x and y coordinates.

#### map

This module contains functionality for storing and handling the original and working grids.

#### player
This small module contains functionality for storing and handling player state.

---

## Testing Plan

### Unit Testing
- The client and server are tested using command line containing invalid, bad, or extreme inputs in test files. The client and server should exit and print out an error whenever one is encountered.
- Other modules such as `play`, `game`, `grid, and `gridPoint` will also be tested using different invalid/null inputs to test programs' resistance to errors.
- Run valgrind to check for memory issues.

### Integration Testing
- The server and client are tested in conjuction against the test files provided. We use logging and our run-time observations to identify errors. We also connect multiple clients to the server to test connection and full functionality.
- Use valgrind to identify memory issues

### System Testing
- For system testing we test the server and client together by starting both modules to run the game. We send logging to output test files to identify errors. We also use observations of the running game to identify issues. If problems arise, we identify if the problem is within the game module or client/server message handling, and we proceed to debug from there.