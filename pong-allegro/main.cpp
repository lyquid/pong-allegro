#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "init.h"

typedef struct Player { 
  int length = 150;
  float x = kScreenWidth * 0.09, 
        y = (kScreenHeight / 2) - (length / 2);
  float dx, dy;
  int score = 0;
} Player; 

typedef struct Ball { 
  float cx = kScreenWidth / 2, 
        cy = kScreenHeight / 2;
  float dcx, dcy;
  int radius = 10;
} Ball; 

int main(int argc, int *args[]) {
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

  bool redraw = true;
  bool quit = false;

  // main game loop
  al_start_timer(fps_timer);
  while (!quit) {
    al_wait_for_event(queue, &event);

    // event handling
    switch (event.type) {
      // the fps event
      case ALLEGRO_EVENT_TIMER:
        redraw = true;
        break;
      // any key or X'd quits
      case ALLEGRO_EVENT_KEY_DOWN:
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        quit = true;
        break;
    }
    // draw and update display
    if (redraw && al_is_event_queue_empty(queue)) {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_textf(font, al_map_rgb_f(1, 1, 1), kScreenWidth / 2, 8, ALLEGRO_ALIGN_CENTER, "Score: %d", player1.score);
      al_draw_filled_circle(ball.cx, ball.cy, ball.radius, al_map_rgb_f(1, 1, 1));
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
