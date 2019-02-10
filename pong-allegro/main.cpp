#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "init.h"

typedef struct Player { 
  int length = 150,
      score = 0;
  float x = kScreenWidth * 0.09, 
        y = (kScreenHeight / 2) - (length / 2),
        dx = 0.0, 
        dy = 6.0;
} Player; 

typedef struct Ball { 
  float cx = kScreenWidth / 2, 
        cy = kScreenHeight / 2,
        dx = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4,
        dy = ((((float)rand()) / RAND_MAX) - 0.5) * 2 * 4;
  int radius = 10;
} Ball; 

int main(int argc, int *args[]) {
  srand(time(NULL));
  ALLEGRO_DISPLAY *disp = NULL;
  ALLEGRO_EVENT_QUEUE *queue = NULL;
  ALLEGRO_FONT *font = NULL;
  ALLEGRO_TIMER *fps_timer = NULL;

  // initialization
  init(&disp, &queue, &font, &fps_timer);
  Ball ball;
  Player player1;

  // event listeners
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_timer_event_source(fps_timer));
  ALLEGRO_EVENT event;

  // keyboard stuff
  enum KeyStatus {
    kKeySeen = 1,
    kKeyReleased = 2
  };
  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key)); // zeroes all the contents of the array

  bool redraw = true;
  bool quit = false;

  // main game loop
  al_start_timer(fps_timer);
  while (!quit) {
    al_wait_for_event(queue, &event);

    // event handling switch
    switch (event.type) {
      // the fps event
      case ALLEGRO_EVENT_TIMER:
        // ball movement logic
        ball.cx += ball.dx;
        if ((ball.cx < ball.radius) || (ball.cx + ball.radius > kScreenWidth)) {
          ball.cx -= ball.dx;
          ball.dx *= -1;
        }
        ball.cy += ball.dy;
        if ((ball.cy < ball.radius) || (ball.cy + ball.radius > kScreenHeight)) {
          ball.cy -= ball.dy;
          ball.dy *= -1;
        }
        // player control logic
        if (key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W])
          player1.y -= player1.dy;
        if (key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S])
          player1.y += player1.dy;
        // player movement limits
        if (player1.y < 0) 
          player1.y = 0;
        if (player1.y > kScreenHeight - player1.length) 
          player1.y = kScreenHeight - player1.length;

        // ESC key logic
        if (key[ALLEGRO_KEY_ESCAPE]) quit = true;

        for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
          key[i] &= kKeySeen;

        redraw = true;
        break;
      // look here for an explanation: 
      // https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace%3A-Input
      case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = kKeySeen | kKeyReleased;
        break;
      case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= kKeyReleased;
        break;  
      // X from the window quits
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        quit = true;
        break;
    }
    // draw and update display
    if (redraw && al_is_event_queue_empty(queue)) {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      // score
      al_draw_textf(font, al_map_rgb_f(1, 1, 1), kScreenWidth / 2, 8, ALLEGRO_ALIGN_CENTER, "Score: %d", player1.score);
      // ball
      al_draw_filled_circle(ball.cx, ball.cy, ball.radius, al_map_rgb_f(1, 1, 1));
      // player 1
      al_draw_filled_rectangle(player1.x, player1.y, player1.x + 10, player1.y + player1.length, al_map_rgb_f(1, 1, 1));
      al_flip_display();
      redraw = false;
    }
  }
  // destroy
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(fps_timer);
  al_destroy_event_queue(queue);

  return 0;
}
