
#ifndef ANIMALS_H
#define ANIMALS_H

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#define WOLF_DOMMAGE 10
#define BOAR_DOMMAGE 7
#define SHEEP_DOMMAGE 0
#define STAG_DOMMAGE 0

#define WOLF_LIFE_POINT 30
#define BOAR_LIFE_POINT 30
#define SHEEP_LIFE_POINT 5
#define STAG_LIFE_POINT 20

#define ATTACK_RANGE_ANI 25
#define ANI_VIEW_RANGE 150

#define WOLF_SPEED 2   /*pixels per second*/
#define BOAR_SPEED 1
#define SHEEP_SPEED 1
#define STAG_SPEED 2


/**
*
*   \file animals.h
*
*   \brief Headers animals model fonctions
*
*   \author Deredempt Nicolas
*
*   \date 07/03/2017
*/


typedef struct Animals Animals;

struct Animals{
    int type;        /*to know what type of animals is standing in front of us linked with the enum below*/
    int life_point;
    int attack;     /*to know whether the animal is attacking or not*/
    int dommage;
    int speed;
    int view_range;
    int dist_to_cam;
    int direction;
    void *target;
    SDL_Rect r;
    SDL_Surface *s;
};

enum { WOLF, BOAR, SHEEP, STAG}; /* type of ressources we are dealing with
 each tile may have a ressource type -> NONE = no ressources on a tile.1000000,*/
typedef enum {LEFT, RIGHT, ABOVE, BELLOW, LEFT_BELLOW, LEFT_ABOVE, RIGHT_BELLOW, RIGHT_ABOVE} Mouvment;


Animals *initWolf();
Animals *initBoar();
Animals *initSheep();
Animals *initStag();


void change_coordinates (Animals * animals_name, int x, int y);
void get_coordo_animals (Animals * animals_name, int *x, int *y);

int move_left_above (Animals * animals_name);
int move_right_above (Animals * animals_name);
int move_left_bellow (Animals * animals_name);
int move_right_bellow (Animals * animals_name);
int move_left (Animals * animals_name);
int move_right (Animals * animals_name);
int move_above (Animals * animals_name);
int move_bellow (Animals * animals_name);

int is_attacking (Animals * animals_name);
int enrage (Animals * animals_name);

void freeAnimals(Animals * animals_name);

#endif
