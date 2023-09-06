/**
 * client.c - c file for nuggets project
 *
 * Nuggets server module implements the game server. 
 * The client communicates back and forth with the server based on the user inputted keys. 
 * 
 * 
 * Gitters, Spring 2023
 */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "message.h"

enum modes {
  PLAYER,
  SPECTATOR
};

typedef struct gamestate 
{
  enum modes mode;
  int nrows;
  int ncols;
  char alias;
  char* port;
  } gamestate_t;

static gamestate_t gamestate;

/**************** function prototypes ****************/
static void initializeScreen();
static bool handleInput(void* arg);
static bool handleMessage(void* arg, const addr_t from, const char* message);

/**************** main ****************/
 /**
  * Function parses arguments, initialize game, and start message loop
 */
int main(int argc, char *argv[])
{
  // initialize message module
  if (message_init(NULL) == 0) {
    return 2;
  }

  // parse and validate arguments
  const char* program = argv[0];
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "usage: %s hostname port [playername]\n", program);
    return 3;
  }

  char* serverHost = argv[1];
  char* serverPort = argv[2];
  addr_t server;

  // try to initialize the address
  if (!message_setAddr(serverHost, serverPort, &server)) {
    fprintf(stderr, "Can't form address from %s %s\n", serverHost, serverPort);
    return 4;
  }

  gamestate.port = serverPort;

  if (argc == 4) { // if player provides a name, send PLAY message
    char* playerName = argv[3];
    gamestate.mode = PLAYER;

    char* command = "PLAY";
    char* message = malloc((sizeof(char)) * (strlen(command) + strlen(playerName)+2));
    sprintf(message, "%s %s", command, playerName);
    message_send(server, message);
    free(message);
  }

  else {
    gamestate.mode = SPECTATOR;
    message_send(server, "SPECTATE");
  }

  // loop through messages
  bool ok = message_loop(&server, 0, NULL, handleInput, handleMessage);
  message_done();

  return ok? 0 : 1;
}

/**************** initializeScreen ****************/
 /**
  * function to intialize screen using ncurses
 */
static void 
initializeScreen() {
  initscr(); 
  cbreak(); 
  noecho(); 
  curs_set(0);

  int x_upleft, y_upleft; //up left
  int x_bottomright, y_bottomright; //bottom right
  getbegyx(stdscr, y_upleft, x_upleft);
  getmaxyx(stdscr, y_bottomright, x_bottomright);

  // get current dimension of window
  int height = y_bottomright - y_upleft;
  int width = x_bottomright - x_upleft;

  do {
    if (width < (gamestate.ncols + 1) || height < (gamestate.nrows + 1)) {
      printw("Window is too small. Enlarge and PRESS ENTER to continue playing. Repeat until map appears.\n");
    }
    else break;

    int key;
    key = getch();
    // check if player has enlarged screen and clicked enter
    while (width < (gamestate.ncols + 1) || height < (gamestate.nrows + 1) || key != 10) {
      getbegyx(stdscr, y_upleft, x_upleft);
      getmaxyx(stdscr, y_bottomright, x_bottomright);
      height = y_bottomright - y_upleft;
      width = x_bottomright - x_upleft;
      key = getch();
    }
  } while (width < (gamestate.ncols + 1) || height < (gamestate.nrows + 1));

  refresh();
}

/**************** handleInput ****************/
 /**
  * Function to handle keystroke inputs from the client
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop
 */
static bool 
handleInput(void* arg)
{
  addr_t* serverp = arg;

  if (serverp == NULL) {
    fprintf(stderr, "handleInput called with arg=NULL");
    return true;
  }

  if (!message_isAddr(*serverp)) {
    fprintf(stderr, "handleInput called without a correspondent.");
    return true;
  }

  // handle keystrokes
  int key = getch();

  if (gamestate.mode == PLAYER) { // keystrokes for players
    switch(key) {
      case 'h': message_send(*serverp, "KEY h"); refresh(); break;
      case 'l': message_send(*serverp, "KEY l"); refresh(); break;
      case 'j': message_send(*serverp, "KEY j"); refresh(); break;
      case 'k': message_send(*serverp, "KEY k"); refresh(); break;
      case 'y': message_send(*serverp, "KEY y"); refresh(); break;
      case 'u': message_send(*serverp, "KEY u"); refresh(); break;
      case 'b': message_send(*serverp, "KEY b"); refresh(); break;
      case 'n': message_send(*serverp, "KEY n"); refresh(); break;
      case 'H': message_send(*serverp, "KEY H"); refresh(); break;
      case 'L': message_send(*serverp, "KEY L"); refresh(); break;
      case 'J': message_send(*serverp, "KEY J"); refresh(); break;
      case 'K': message_send(*serverp, "KEY K"); refresh(); break;
      case 'Y': message_send(*serverp, "KEY Y"); refresh(); break;
      case 'U': message_send(*serverp, "KEY U"); refresh(); break;
      case 'B': message_send(*serverp, "KEY B"); refresh(); break;
      case 'N': message_send(*serverp, "KEY N"); refresh(); break;
      case 'Q': message_send(*serverp, "KEY Q"); refresh(); break;
      // if other keys, print to player's screen
      default: mvprintw(0, 49, "Unknown keystroke"); refresh();
    }
  }
  else {
    switch(key) {
      case 'Q': message_send(*serverp, "KEY Q"); refresh(); break;
    }
  }
  refresh();
  return false;
}

/**************** handleMessage ****************/
 /**
  * Function to handle incoming messages from the client
  * Callter provides:
  *   a valid address to the message sender and the message
  * Function returns:
  *   false if the message loop should continue
  *   true if the message loop should stop
 */
static bool 
handleMessage(void* arg, const addr_t from, const char* message) {
  if (message == NULL) return false;

  // hand OK message
  if (strncmp(message, "OK ", strlen("OK ")) == 0) {
    const char* content = message + strlen("OK ");
    char alias;
    sscanf(content, "%c", &alias);
    gamestate.alias = alias;
    return false;
  }
  
  // handle GRID message
  if (strncmp(message, "GRID ", strlen("GRID ")) == 0) {
    const char* content = message + strlen("GRID ");

    int rows, cols;
    sscanf(content, "%d %d", &rows, &cols);
    gamestate.nrows = rows;
    gamestate.ncols = cols;

    initializeScreen();
    refresh();
    return false;
  }

  // handle GOLD message
  if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
    const char* content = message + strlen("GOLD ");
    int justCollected, purseTotal, goldRemaining;
    sscanf(content, "%d %d %d", &justCollected, &purseTotal, &goldRemaining);

    refresh();
    move(0,0);
    clrtoeol();
    refresh();

    if (gamestate.mode == PLAYER) {
      // print gold info of a player
      if (justCollected == 0) {
        mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed).", gamestate.alias, purseTotal, goldRemaining);
        refresh();
      }
      else {
        mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed). GOLD received: %d", gamestate.alias, purseTotal, goldRemaining, justCollected);
        refresh();
      }
    }
    // print info for a spectator
    else {
      mvprintw(0,0, "Spectator: %d nuggets unclaimed. Play at plank %s", goldRemaining, gamestate.port);
    }
    refresh();
    return false;
  }

  // handle DISPLAY message
  if (strncmp(message, "DISPLAY\n", strlen("DISPLAY\n")) == 0) {
    const char* content = message + strlen("DISPLAY\n");

    // print the map starting from line 1
    mvprintw(1, 0, "%s", content);
    refresh();
    return false;
   }

   // handle QUIT message
  if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    const char* content = message + strlen("QUIT ");
    endwin(); // exit curses
    printf("%s\n", content);
    return true; // return true to stop loop
  }

  // handle ERROR message
  if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
    const char* content = message + strlen("ERROR ");
    refresh();
    // print message to stderr
    fprintf(stderr, "%s", content);
    return false;
  }
  return false;
}