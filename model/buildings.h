#ifndef BUILDINGS_H
#define BUILDINGS_H

#define HOUSE_HP 250
#define TOWER_HP 400
#define BARRACKS_HP 400
#define CITY_HALL_HP 600

#include <SDL.h>

/**
*   \file buildings.h
*
*   \author Nicolas Sourbier
*
*   \brief to handle correctly the buildings in the game
*/


typedef struct Building Building;
struct Building
{
    int range;
    int hp;
    int type;
    int action;
    SDL_Rect building_r;
    int team;
    int dist_to_cam;
    void *target;
};

enum {HOUSE, BARRACKS, CITY_HALL, TOWER};
enum {SLEEP, BUILD_PERSO, CONSTRUCT_P};

Building * init_building(int type);
int freeBuilding (Building *b);
void setActionFlag(Building *b,int flag);
void setTargetB(Building *b, void *);
int getActionFlag(Building *b);
int getHP(Building *b);

#endif
