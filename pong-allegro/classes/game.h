#ifndef GAME_H
#define GAME_H  

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "../config.h"

class Game {
  public:
    Game() {
      player2.x = (kScreenWidth * 0.90) - (player2.width / 2);
      // zeroes all the contents of the key array
      memset(key, 0, sizeof(key)); 
    }
    ~Game() {}

    void clean();
    void handleEvents();
    void init();
    bool isRunning();
    void registerEvents();
    void render(); 

  private:

    bool checkCollision(Ball, Player, Player);
    void checkPlayerMovementLimits(Player *);
    void mustInit(bool test, const char *desc);

    ALLEGRO_DISPLAY *disp = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_SAMPLE *pong = NULL;
    ALLEGRO_TIMER *fps_timer = NULL;
    bool redraw = true;
    bool running = false;
    Ball ball;
    Player player1;
    Player player2;
    // keyboard stuff
    unsigned char key[ALLEGRO_KEY_MAX];
    enum KeyStatus {
      kKeySeen = 1,
      kKeyReleased = 2
    }; 
};

#endif