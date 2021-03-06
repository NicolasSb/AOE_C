
#ifndef Resource_H
#define Resource_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#define WOOD_AMOUNT 1000
#define STONE_AMOUNT 2000
#define FOOD_AMOUNT 800
#define GOLD_AMOUNT 2000
#define COLLECT_AREA 70
#define WOOD_COLLECT_TIME 0.5   /* collect_time correspond to the number of Resources raised per seconds*/
#define FOOD_COLLECT_TIME 0.5
#define STONE_COLLECT_TIME 0.5
#define GOLD_COLLECT_TIME 0.5
/**
*
*   \file Resource.h
*
*   \brief Headers Resource model fonctions
*
*   \author Deredempt Nicolas
*
*   \date 07/03/2017
*/

enum { FOOD = 0, WOOD = 1, STONE = 2, GOLD = 3};
 /* type of Resources we are dealing with
 each tile may have a Resource type -> NONE = no Resources on a tile.*/

typedef struct Resource Resource;
struct Resource{
    int type;   /*to know what type of Resource we are collecting linked with the enum below*/
    int amount;
    int level;  /*will allow us to increase the speed of the collecting*/
    int collect_time;
    int dist_to_cam;
    SDL_Rect r;
};


Resource *initResource(int type);
void set_position (Resource * Resource_name, int x, int y);
void get_coordo (Resource * Resource_name, int *x, int *y);
int improve_collect_time (Resource * Resource_name, int level);
int decreaseResource(Resource *Resource_name, int collect);
void freeResource(Resource *source);
#endif /* Resource_H*/
