#include "classes/game.h"

Game *game = NULL;

int main(int argc, char *argv[]) {
  
  srand(time(NULL));

  game = new Game();
  game->init();

  while (game->isRunning()) {
    game->handleEvents();
    game->update();
    game->render();
  }
  game->clean();
  delete game;
  return 0;
}
