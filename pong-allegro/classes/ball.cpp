#include "ball.h"

bool Ball::checkCollision(Player p1, Player p2) {
  bool p1_collision = true;
  bool p2_collision = true;
  // ball sides (todo: improve this)
  int b_left   = cx - radius,
      b_right  = cx + radius,
      b_top    = cy - radius,
      b_bottom = cy + radius;
  // player1 sides
  int p1_left   = p1.x,
      p1_right  = p1.x + p1.width,
      p1_top    = p1.y,
      p1_bottom = p1.y + p1.length;
  // player2 sides
  int p2_left   = p2.x,
      p2_right  = p2.x + p2.width,
      p2_top    = p2.y,
      p2_bottom = p2.y + p2.length;
  // collision check with player1
  if ( b_left >= p1_right 
    || b_right <= p1_left
    || b_top >= p1_bottom
    || b_bottom <= p1_top )
      p1_collision = false;
  // collision check with player2
  if ( b_left >= p2_right 
    || b_right <= p2_left
    || b_top >= p2_bottom
    || b_bottom <= p2_top )
      p2_collision = false;
  
  return (p1_collision || p2_collision) ? true : false;
}

bool Ball::exitBottom() {
  return (cy + radius > kScreenHeight) ? true : false;
}

bool Ball::exitLeft() {
  return (cx < radius) ? true : false;
}

bool Ball::exitRight() {
  return (cx + radius > kScreenWidth) ? true: false;
}

bool Ball::exitTop() {
  return (cy < radius) ? true : false;
}

void Ball::invertX() {
  cx -= dx;
  dx *= -1;
}

void Ball::invertY() {
  cy -= dy;
  dy *= -1;
}

void Ball::moveX() {
  cx += dx;
}

void Ball::moveY() {
  cy += dy;
}
