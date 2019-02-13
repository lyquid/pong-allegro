#include "game.h"

void Game::checkPlayerMovementLimits(Player *pl) {
  if (pl->y < 0) 
    pl->y = 0;
  if (pl->y > kScreenHeight - pl->length) 
    pl->y = kScreenHeight - pl->length;
}

void Game::clean() {
  al_destroy_sample(pong);
  al_destroy_font(font);
  al_destroy_display(disp);
  al_destroy_timer(fps_timer);
  al_destroy_event_queue(queue);
}

void Game::handleEvents() {
  al_wait_for_event(queue, &event);
  // event handling switch
  switch (event.type) {
    // the fps event
    case ALLEGRO_EVENT_TIMER:
      // player control logic
      if (key[ALLEGRO_KEY_W]) player1.y -= player1.dy;
      if (key[ALLEGRO_KEY_UP]) player2.y -= player2.dy;
      if (key[ALLEGRO_KEY_S]) player1.y += player1.dy;
      if (key[ALLEGRO_KEY_DOWN]) player2.y += player2.dy; 
      // player movement limits
      checkPlayerMovementLimits(&player1);
      checkPlayerMovementLimits(&player2);
      // ESC key logic
      if (key[ALLEGRO_KEY_ESCAPE]) running = false;

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
      running = false;
      break;
  }
}

void Game::init() {
  mustInit(al_init(), "allegro5");
  mustInit(al_install_audio(), "audio addon");
  mustInit(al_init_acodec_addon(), "audio codec addon");
  mustInit(al_init_primitives_addon(), "primitives addon");
  mustInit(al_install_keyboard(), "keyboard");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
  disp = al_create_display(kScreenWidth, kScreenHeight);
  mustInit(disp, "display");

  queue = al_create_event_queue();
  mustInit(queue, "queue");

  font = al_create_builtin_font();
  mustInit(font, "font");

  mustInit(al_reserve_samples(1), "reserve samples");
  pong = al_load_sample("assets/pong.wav");
  mustInit(pong, "pong sound");

  fps_timer = al_create_timer(1.0 / kFPS);
  mustInit(fps_timer, "timer");
  al_start_timer(fps_timer);

  running = true;
}

void Game::mustInit(bool test, const char *desc) {
  if (test) return;

  printf("couldn't initialize %s\n", desc);
  exit(1);
}

bool Game::isRunning() {
  return running; 
} 

void Game::registerEvents() {
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_timer_event_source(fps_timer));
}

void Game::render() {
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

void Game::update() {
  ball.moveX();
  if(ball.exitLeft()) {
    ++player2.score;
    ball = Ball();
  }
  if(ball.exitRight()) {
    ++player1.score;
    ball = Ball();
  }
  if (ball.checkCollision(player1, player2)) {
    al_play_sample(pong, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    ball.cx -= ball.dx;
    ball.dx *= -1;
  }

  ball.moveY();
  if ((ball.cy < ball.radius) || (ball.cy + ball.radius > kScreenHeight) || (ball.checkCollision(player1, player2))) {
    al_play_sample(pong, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    ball.cy -= ball.dy;
    ball.dy *= -1;
  }
}