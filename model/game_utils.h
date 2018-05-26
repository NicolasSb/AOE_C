#ifndef PLAYER_H
#define PLAYER_H

#include "game_utils.h"
#include "buildings.h"
#include "ressource.h"
#include "animals.h"
#include "perso.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#define NB_RESOURCE_TYPE 4
#define MAX_PERSO 10
#define MAX_CITYHALL 3
#define MAX_HOUSES 20
#define MAX_BARRACKS 5
#define MAX_TOWER 10
#define MAX_SELECT 20

/**
*
*   \file player.h
*   \brief implementation of the functions depending on more than one type (buildings/perso, Resource/perso, ...)
*   \author Sourbier Nicolas
*   \date
*/

/* to know what is selected by the player*/
typedef struct Select Select;
struct Select {
    void *target;
    int target_type;
};

/* the player have available resources, personages and buildings*/
typedef struct Player Player;
struct Player {
    int team_number;
    int available_resources[NB_RESOURCE_TYPE];
    Perso *selectP[MAX_SELECT];
    int lumbermen;
    int stoneCutter;
    int goldFinder;
    int picker;
    int nbCharacters;
    int nbVillagers;
    int nbHouses;
    int nbCityHall;
    int nbBarracks;
    int nbTower;
    int main_buildingX;
    int main_buildingY;
    char spare_resources;
    Select s;
};

enum { /* this is the type of the player's selection*/
    SELECT_INVALID=0, SELECT_BUILDING, SELECT_RESOURCE, SELECT_ANIMALS
};

/*Players functions*/
Player * initPlayer(int team_nb);
void freePlayer(Player *pl);
int setSelect(Player *p, void * target, int select_type);
int addSelectedP(Player *pl, Perso *p);
void clear_selectP(Player *p);

/*Perso's functions*/
int attack_A(Perso* p);
int attackB(Perso *p);
int collectResources(Perso * p, Resource *r);
Building* buildB(Player *pl, int type);
void constructB(Perso *p);
void moveP(Perso *p, int x, int y);
int targetType(Perso*p);

/*buildings functions*/
Perso * createPerso (Building *b, Player *pl, int type);
void constructP(Building *b);

/*animal's functions*/
int animal_ATT(Animals * ani);
Mouvment follow (Animals * animals_name, Perso* p);
int decrease_life_point (Animals * animals_name, Perso * p);
int character_is_seen (Animals * animals_name, Perso * p);
int is_in_area (Resource * Resource_name, Perso * p);
int follow_animal(Perso* p, Animals * ani);
int setAttacked_by_animal(Perso* p, Animals* attacker);
int move_animal_to_character (Mouvment move, Animals * animals_name);

#endif /* PLAYER_H*/

