#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "game_utils.h"


#define MAX_PLAYER 12

typedef struct Game Game;
struct Game {
    Player* pl[MAX_PLAYER]; /* an array of players*/
    GameMap *m;     /* the map */
    int nbPlayer;
    List *p;
    List *b;
    List *r;
    List *a;
};


Game *initGame(int nbPlayers);
void freeGame(Game *g);
void saveGameBinary(Game *g);
Game *loadGameFromFileBinary();
void emptyLists(Game *g);
void create_listPerso(Game *g);
void create_listBuilding(Game *g);
void * enemyDetected(Game *g, int mouse_x, int mouse_y, int *);
void *resourceFound(Game *g, int mouse_x, int mouse_y, int *type);
int HumanDetected_by_animal(Animals * ani, Game *g);
void updateDistToCam(Game *g, List *l);

void select_perso(Game *g, SDL_Rect r);
void select_onePerso(Game *g, int x, int y);
void select_Something(Game *g, int mouse_x, int mouse_y);

int action_perso(Game *g, Perso *p);
int action_Building(Game *g, Building *b);
int action_animal(Game *g, Animals *ani);
void attackCharacter(Game *g, Perso *p);
void attackAnimals(Game *g, Perso *p);
void AnimalATT_perso(Game *g, Animals * ani);
void attackBuilding(Game *g, Perso *p);
void collectResources_general(Game *g, Player *pl, Perso *p);


/* IA Functions */
void initResourceList(Game *, GameMap *);
void autoCollectResources(Game *g, Player *pl, Perso *p);
void chooseValidCoordinate(Player *, Perso *p);




#endif /* GAME_H*/
