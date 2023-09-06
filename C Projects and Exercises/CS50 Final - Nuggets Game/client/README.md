# CS50 Nuggets - Client
### Content
This subdirectory contains the following files to implement the client for nuggets. The client is responsible for communicating with the server and displaying the game to the players/spectator's screen.

- `client.c`, which implements the client to exchange messages with the server to run the game. 
- `Makefile` to compile the client.
- `.gitignore` to ignore binary files.
- `README.md`.

### Instructions
- To compile the client, simply type the command 'make' in the terminal.
- After successful compilation, to establish a connection to the server to join a nuggets game, use the syntax:
```
./client hostname port [playername]
```
- Where hostname is the name of the host, port is the port number provided by the server.
- If the client wants to join as a player, a `playername` has to be provided. `playername` must be shorter than 50 characters. If `playername` is not provided, the client joins as a spectator.
- The client can only connect to the server if all arguments are correct and valid. Refer to 'REQUIREMENTS.md` and `DESIGN.md`for more details.