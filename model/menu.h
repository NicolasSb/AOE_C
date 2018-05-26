#ifndef HEADER
#define HEADER

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "../view/application.h"

#define LENGTH_SCREEN 1280
#define HEIGHT_SCREEN 720

#define LENGTH_SELECT_MENU 250
#define HEIGHT_SELECT_MENU 35

#define NUMBER_OF_CHOICES 3

typedef struct
{
    SDL_Surface *screen;
    SDL_Surface *background;
    SDL_Surface *select_menu;
    SDL_Rect position;
    SDL_Event event;
    double x_select_menu;
    double center;
    double y_select_menu[NUMBER_OF_CHOICES];
    int choice;
} Menu;

enum {NEW_GAME, LOAD, QUIT};

Menu *menu_blit(Menu *menu);
Menu *menu_initialization(App *a);

void freeMenu(Menu*);
#endif /* HEADER*/


