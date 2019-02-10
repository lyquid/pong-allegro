#ifndef INIT_H
#define INIT_H  

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void init(
  ALLEGRO_DISPLAY     **disp, 
  ALLEGRO_EVENT_QUEUE **queue, 
  ALLEGRO_FONT        **font, 
  ALLEGRO_TIMER       **timer
);

void mustInit(bool test, const char *desc);

#endif
