# CS50 Nuggets - SERVER
### Content
This subdirectory contains the following files to implement the server for nuggets. The server handles most game logic and updates the client on the state of the game using messages. 

- `server.c`, which implements the server to exchange messages with the client to run the game. 
- `Makefile` to compile the server.
- `.gitignore` to ignore binary files.
- `README.md`

### Instructions
- To compile the server, simply type the command 'make' in the terminal.
- After successful compilation, to open a nuggets game for remote players to join to join, use the syntax:
```
./server map.txt [seed]
```
- Where map.txt is the name of the file containing a string representation of the game map and the seeds control randomization. The server then initializes a port and print the port number out to the screen and wait for clients to join.