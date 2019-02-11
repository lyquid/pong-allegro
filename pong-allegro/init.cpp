#include "config.h"
#include "init.h"

void init(
  ALLEGRO_DISPLAY **disp, 
  ALLEGRO_EVENT_QUEUE **queue, 
  ALLEGRO_FONT **font,
  ALLEGRO_SAMPLE **pong, 
  ALLEGRO_TIMER **timer
) {
  mustInit(al_init(), "allegro5");
  mustInit(al_init_primitives_addon(), "primitives addon");
  mustInit(al_install_audio(), "audio addon");
  mustInit(al_init_acodec_addon(), "audio codec addon");
  mustInit(al_install_keyboard(), "keyboard");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
  *disp = al_create_display(kScreenWidth, kScreenHeight);
  mustInit(*disp, "display");

  *queue = al_create_event_queue();
  mustInit(*queue, "queue");

  *font = al_create_builtin_font();
  mustInit(*font, "font");

  mustInit(al_reserve_samples(1), "reserve samples");
  *pong = al_load_sample("assets/pong.wav");
  mustInit(*pong, "pong sound");

  *timer = al_create_timer(1.0 / kFPS);
  mustInit(*timer, "timer");
}

void mustInit(bool test, const char *desc) {
  if (test) return;

  printf("couldn't initialize %s\n", desc);
  exit(1);
}
