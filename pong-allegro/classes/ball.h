#ifndef BALL_H
#define BALL_H  

#include <allegro5/allegro5.h>

#include "../config.h"

class Ball {
  public:
    Ball() {
      cx = kScreenWidth / 2, 
      cy = kScreenHeight / 2,
      dx = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4,
      dy = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4;
      radius = 10;
    }
    ~Ball() {}

    bool checkCollision(Player p1, Player p2);
    bool exitLeft();
    bool exitRight();
    void moveY();
    void moveX();

    float cx, cy,
          dx, dy;
    int radius;
  private:
};

#endif