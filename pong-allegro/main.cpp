#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
 #include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "config.h"
#include "init.h"

bool checkCollision(Ball, Player, Player);
void checkPlayerMovementLimits(Player *);

int main(int argc, int *args[]) {
  srand(time(NULL));
  ALLEGRO_DISPLAY *disp = NULL;
  ALLEGRO_EVENT_QUEUE *queue = NULL;
  ALLEGRO_FONT *font = NULL;
  ALLEGRO_SAMPLE *pong = NULL;
  ALLEGRO_TIMER *fps_timer = NULL;

  // initialization
  init(&disp, &queue, &font, &pong, &fps_timer);
  Ball ball;
  Player player1;
  Player player2;
  player2.x = (kScreenWidth * 0.90) - (player2.width / 2);

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
  // zeroes all the contents of the array
  memset(key, 0, sizeof(key)); 

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
        // exit from the left side
        if (ball.cx < ball.radius) {
          ++player2.score;
          ball = Ball();
        }
        // exit from the right side
        if (ball.cx + ball.radius > kScreenWidth) {
          ++player1.score;
          ball = Ball();
        }
        if (checkCollision(ball, player1, player2)) {
          al_play_sample(pong, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          ball.cx -= ball.dx;
          ball.dx *= -1;
        }
        ball.cy += ball.dy;
        if ((ball.cy < ball.radius) || (ball.cy + ball.radius > kScreenHeight) || (checkCollision(ball, player1, player2))) {
          al_play_sample(pong, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
          ball.cy -= ball.dy;
          ball.dy *= -1;
        }
        // player control logic
        /* switch (key[]) {
          case ALLEGRO_KEY_W:
            player1.y -= player1.dy;
            break;
          case ALLEGRO_KEY_UP:
            player2.y -= player2.dy;
            break;
          case ALLEGRO_KEY_S:
            player1.y -= player1.dy;
            break;
          case ALLEGRO_KEY_DOWN:
            player2.y -= player2.dy;
            break;
        } */
        if (key[ALLEGRO_KEY_W]) player1.y -= player1.dy;
        if (key[ALLEGRO_KEY_UP]) player2.y -= player2.dy;
        if (key[ALLEGRO_KEY_S]) player1.y += player1.dy;
        if (key[ALLEGRO_KEY_DOWN]) player2.y += player2.dy; 
        // player movement limits
        checkPlayerMovementLimits(&player1);
        checkPlayerMovementLimits(&player2);

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
      al_draw_textf(
        font, al_map_rgb_f(1, 1, 1), 
        kScreenWidth / 2, 8, 
        ALLEGRO_ALIGN_CENTER, 
        "%d - SCORES - %d", player1.score, player2.score
      );
      // ball
      al_draw_filled_circle(ball.cx, ball.cy, ball.radius, al_map_rgb_f(1, 1, 1));
      // player 1
      al_draw_filled_rectangle(
        player1.x, player1.y, 
        player1.x + player1.width, player1.y + player1.length, 
        al_map_rgb_f(1, 1, 1)
      );
      // player 2
      al_draw_filled_rectangle(
        player2.x, player2.y, 
        player2.x + player2.width, player2.y + player2.length, 
        al_map_rgb_f(1, 1, 1)
      );
      al_flip_display();
      redraw = false;
    }
  }
  // destroy
  al_destroy_sample(pong);
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(fps_timer);
  al_destroy_event_queue(queue);

  return 0;
}

bool checkCollision(Ball b, Player p1, Player p2) {
  bool p1_collision = true;
  bool p2_collision = true;
  // ball sides (todo: improve this)
  int b_left   = b.cx - b.radius,
      b_right  = b.cx + b.radius,
      b_top    = b.cy - b.radius,
      b_bottom = b.cy + b.radius;
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

void checkPlayerMovementLimits(Player *pl) {
  if (pl->y < 0) 
    pl->y = 0;
  if (pl->y > kScreenHeight - pl->length) 
    pl->y = kScreenHeight - pl->length;
}