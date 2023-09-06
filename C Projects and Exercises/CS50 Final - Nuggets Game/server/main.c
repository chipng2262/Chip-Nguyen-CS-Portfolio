#include <stdbool.h>
#include <stdio.h>

static const int MaxPlayers = 26;
static const int goldTotal = 250;
static const int GoldMinNumPiles = 10;
static const int GoldMaxNumPiles = 30; 

int main(int argc, char *argv[])
{
  printf("Starting server\n");

  if(argc != 3 && argc != 2){
    fprintf(stderr, "Error: Invalid number of parameters given. Expects .txt, [Seed]");
    exit(1);
  }

  int seed;
  if(argc == 3){
    if(sscanf(argv[2], "%d", &seed) <= 0){
      fprintf(stderr, "Error: expects second value to be a positive integer");
      exit(2);
    }
    if(seed < 0){
      fprintf(stderr, "Error: expects second value to be a positive integer");
      exit(3);
    }
    srand(seed);
  }
  else{
    srand(getpid());
  }

  FILE* fp;
  if((fp = fopen(argv[1], "r")) == NULL){
    fprintf(stderr, "Error: unopenable file");
    exit(4);
  }

  int numGoldPiles = rand() % (GoldMaxNumPiles - GoldMinNumPiles + 1) + GoldMinNumPiles;

  


  bool is_running = true;
  while (is_running) {


  }

  return 0;
}
