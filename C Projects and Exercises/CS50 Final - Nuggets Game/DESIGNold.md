# CS50 Nuggets
## Design Spec
### Team Gitters, Spring, 2023

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes client, server, and shared modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

The *client* acts in one of two modes:

 1. *spectator*, the passive spectator mode described in the requirements spec.
 2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

Clients playg the game with keyboard inputs, unless they are 

### Inputs and outputs

#### Inputs
The client takes in keystrokes to move the character within and between rooms.

#### Outputs
Displays the map on the client side by printing it to stdout.

### Major data structures

> A language-independent description of the major data structure(s) in this program.
> Mention, but do not describe, any libcs50 data structures you plan to use.

---

## Server
### User interface

See the requirements spec for the command-line interface.
There is no interaction with the user.

> You may not need much more.

### Inputs and outputs

> Briefly describe the inputs (map file) and outputs (to terminal).
> If you write to log files, or log to stderr, describe that here.
> Command-line arguments are not 'input'.


### Functional decomposition into modules

- Client
    - Holds a connection to the server
    - Sends payloads of keyboard input to the server
    - Receives payloads of game state from the server
    - Updates visibility each frame
- Server
    - Holds connections to each client
    - Receives payloads from clients
    - Controls game state
    - Sends state in a payload to clients
- Shared
    - Functionality and type definitions shared between clients and servers
    - Structs: payloads, game state

### Pseudo code for logic/algorithmic flow

### Server pseudocode
The server will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initialize_game() to set up data structures
	initialize the 'message' module
	print the port number on which we wait
	call message_loop(), to await clients and handle game logic
	call game_over() to inform all clients the game has ended
	clean up


#### message_loop()
This function, given in the message module, will use a callback that parses and handles the inputs from the client.

#### initialize_game()
This function will initialize all server state, including the game state and the buffer for client connection handles.

#### game_over()
This function will stop game logic and send a message with type `SERVER_GAME_OVER` to all clients, which will end the game.

### Client pseudocode
The client will run as follows:

	execute from a command line per the requirement spec
	parse the command line, validate parameters
	call initialize_player() to set up data structures
    establish a connection to the server
    loop
        send input to the server
        break when client wishes to disconnect
    clean up

#### initialize_player()
This function will initialize all client state

#### establish_connection()
This function will make a connection handle to the server and receive initial state, including the map

### Major data structures

#### Game Map

Contained in a string representing the 2D array of grid points in the map

#### Payload Packet

Contains a flag representing the type of message sent and then the message itself, represented as a string

#### Connections list

Contained in an array of `addr_t` structs

## Client module

## Server module

## Shared module
This module contains functionality and types used by both clients and servers.

This includes:

- Payload information
    - Message type flags
    - Message data structures
- Map structs
    - Representation of the map, with helper functions

### Functional decomposition

> List each of the main functions implemented by this module, with a phrase or sentence description of each.

### Pseudo code for logic/algorithmic flow

> For any non-trivial function, add a level-4 #### header and provide tab-indented pseudocode.
> This pseudocode should be independent of the programming language.

### Major data structures

> Describe each major data structure in this module: what information does it represent, how does it represent the data, and what are its members.
> This description should be independent of the programming language.
