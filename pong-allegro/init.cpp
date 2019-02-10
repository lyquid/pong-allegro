#include "config.h"
#include "init.h"

void init(
  ALLEGRO_DISPLAY **disp, 
  ALLEGRO_EVENT_QUEUE **queue, 
  ALLEGRO_FONT **font, 
  ALLEGRO_TIMER **timer
) {
  mustInit(al_init(), "allegro5");
  mustInit(al_init_primitives_addon(), "primitives addon");
  mustInit(al_install_keyboard(), "keyboard");

  *timer = al_create_timer(1.0 / kFPS);
  mustInit(*timer, "timer");

  *queue = al_create_event_queue();
  mustInit(*queue, "queue");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
  *disp = al_create_display(kScreenWidth, kScreenHeight);
  mustInit(*disp, "display");

  *font = al_create_builtin_font();
  mustInit(*font, "font");
}

void mustInit(bool test, const char *desc) {
  if (test) return;

  printf("couldn't initialize %s\n", desc);
  exit(1);
}
