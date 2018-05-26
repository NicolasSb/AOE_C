#ifndef PERSO_H
#define PERSO_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <SDL.h>
#include "files.h"


#define VILLAGER_ARMOR 5
#define VILLAGER_ATTACK_RANGE 30
#define VILLAGER_DAMAGE 10
#define VILLAGER_HP 50
#define VILLAGER_RANGE 50

#define SWORDMAN_ARMOR 10
#define SWORDMAN_ATTACK_RANGE 30
#define SWORDMAN_DAMAGE 30
#define SWORDMAN_HP 60
#define SWORDMAN_RANGE 50

#define ARCHER_ARMOR 10
#define ARCHER_ATTACK_RANGE 120
#define ARCHER_DAMAGE 20
#define ARCHER_HP 50
#define ARCHER_RANGE 50

/* I've split the structure in two parts, not to put arrows everywhere in my code but to
/ but to access without too much thinking to the arguments*/
typedef struct Perso_u Perso_u;
struct Perso_u {            /* Skills of a Perso* */
    int hp;                 /* Health points */
    int damage;
    int armor;
    int attack_range;
    int range;
};

typedef struct Target_s Target_s;
struct Target_s{
    int type;
    void *t;
};
typedef struct Perso Perso;
struct Perso{               /* The general skills of a type and the individual data */
    Perso_u skills;
    Target_s t;          /* to catch the target of an action */
    SDL_Rect perso_image;
    int dest_x;
    int dest_y;
    int type;               /* The type of perso (see enum below) */
    int action_flag;        /* To determine what he's got to do */
    int generation_time;
    int team;
    int dist_to_cam;
};

enum {                      /* the types of personages */
    VILLAGER, SWORDMAN, ARCHER
};

enum {
    WAIT, MOVE, ATTACK, COLLECT, BUILD, CONSTRUCT, DO_NOTHING
};

enum target_t{
    TARGET_INVALID=0,
    TARGET_RESSOURCE,
    TARGET_PERSO,
    TARGET_BUILDING,
    TARGET_ANIMALS
};


Perso* initPerso(int type);
void setCoord(Perso* p, int x, int y);
void freePerso(Perso* p);
int attack(Perso* player);
int setAttacked(Perso* player, Perso* attacker);
int follow_p(Perso* player, Perso* enemy);
int move_p(Perso* p, int x, int y);
int chooseMoveCode(Perso* p, int x, int y);
double computeDistance(int x1, int y1, int x2, int y2);
void getCoordinates(Perso* p, int *, int*);
int getTeam(Perso *p);
void setTarget(Perso *p, void* target, int target_Type);
void setActionFlagP(Perso *p, int action_flag);
int getActionFlagP(Perso *p);
void setTeam(Perso *p, int team_number);
void setRect(Perso *p, SDL_Rect r);
#endif
