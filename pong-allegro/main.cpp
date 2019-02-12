#include "classes/game.h"

Game *game = NULL;

int main(int argc, int *args[]) {
  
  srand(time(NULL));

  game = new Game();
  game->init();
  game->registerEvents();

  while (game->isRunning()) {
    game->handleEvents();
    game->update();
    game->render();
  }
  game->clean();
  delete game;
  return 0;
}
