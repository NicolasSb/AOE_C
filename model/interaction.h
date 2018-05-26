#ifndef HEADER_INTERACTION
#define HEADER_INTERACTION

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include "../view/application.h"
#include "../model/game_utils.h"

#define TEXT_HEIGHT 20
#define LENGTH_SCREEN 1280
#define HEIGHT_SCREEN 720

#define OPTION_POSITION_Y 12
#define BUTTON_POSITION_X 755

#define OFFSET_Y_BUTTON 25
#define OFFSET_X_OPTION 877
#define NUMBER_OF_BUTTONS 4
#define NUMBER_OF_OPTIONS 6

#define LENGTH_SELECT_BUTTON 110
#define HEIGHT_SELECT_BUTTON 40

#define LENGTH_SELECT_OPTION 60
#define HEIGHT_SELECT_OPTION 60

#define OFFSET_DATA_X 360
#define OFFSET_DATA_Y 25
#define JUMP_DATA_Y 35

#define OFFSET_SKILLS_X 500
#define OFFSET_SKILLS_Y 15
#define JUMP_SKILLS_X 90

typedef struct
{
    SDL_Surface *background;
    SDL_Surface *select_button;
    SDL_Surface *select_option;
    SDL_Rect position;
    double x_select_button;
    double y_select_button[NUMBER_OF_BUTTONS];
    double x_select_option[NUMBER_OF_OPTIONS];
    double y_select_option;
    int button_selected;
    int option_selected;
    int banner_type;
    int banner_selected;
    TTF_Font *police;
} Banner;

enum{B_ARCHER,B_SWORDSMAN,B_VILLAGER,B_CITYHALL,B_HOUSE,B_TOWER,B_BARRACK,B_BOAR,B_SHEEP,B_WOLF,B_STAG,B_GOLD,B_WOOD,B_STONE,B_FOOD,B_DEFAULT};
enum{B_BASE=500,B_CHARACTER,B_ANIMAL,B_RESOURCE,B_BUILDING};
enum{EVENT_MOVEMENT=1000, EVENT_CLIC};
int display_banner(void);
void action_on_banner(Game *g, int type, int nbButton);

Banner *banner_initialization(App *a);
Banner *banner_selection(Banner *banner);

void banner_control(Game *, Banner *banner, int, int, int);
void freeBanner(Banner *b);

void init_ttf(Banner *b);
Banner * banner_blit(Banner *banner, App *a, Player *pl);
void display_game_Data(Banner *banner,App *a,Player *pl);

int buttonHovered(Banner *b, int y);
int optionHovered(Banner *b, int x);

#endif
