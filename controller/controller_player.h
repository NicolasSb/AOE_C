#ifndef CONTROLL_P
#define CONTROLL_P

#include "../view/application.h"
#include "../model/map.h"
#include "../model/game_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "../model/game.h"
#include <SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include "../model/interaction.h"

/* this is temporary, the map should not be there in the next version*/
void action_listener(Game *g, App *a, Banner *b);
void standard_imput(Game *g, Player *p, App *a, SDL_Event e);


void setBannerFlag(Player *p, Banner *b);

#endif
