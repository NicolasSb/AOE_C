#include "game_utils.h"
#include "buildings.h"
#include "ressource.h"
#include "perso.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/**
*
*   \file player.c
*   \brief Player management and interactions
*
*   \author Sourbier N.
*
*   \version 1.0
*
*   \date 04/03/2017
*
*   \todo change some functions' scope to static
*/

/**
*   \brief the initialization of a player
*
*   at the beginning of the game, a player has 5 Villagers and a city hall
*   he also has 1000 available units of each resources
*
*/

Player * initPlayer(int team_nb)
{
    Player *pl = calloc(1, sizeof(Player));
    int i;
    if (!pl)
        return NULL;
    for (i=0; i< NB_RESOURCE_TYPE; i++)
        pl->available_resources[i]=1000;
    pl->team_number=team_nb;
    pl->s.target=NULL;
    pl->s.target_type=SELECT_INVALID;
    pl->goldFinder=0;
    pl->lumbermen=0;
    pl->picker=0;
    pl->stoneCutter=0;
    pl->nbBarracks=0;
    pl->nbCityHall=1;
    pl->nbHouses=0;
    pl->nbTower=0;
    pl->nbCharacters=5;
    pl->nbVillagers=5;
    pl->spare_resources=0;
    pl->main_buildingX=0;
    pl->main_buildingY=0;
    return pl;
}


/**
*   \brief to collect resources with a Personage
*
*   \param \a p, the personage that will collect the resource \a r
*
*/

int collectResources(Perso * p, Resource *r)
{

    static int nb_calls;
    int count = 0;

    if ((p != NULL) && (r != NULL) && (p->type == VILLAGER))
    {
        if((is_in_area(r,p)) )
        {
            if (r->amount <= 0 )
                return -1;
            nb_calls +=1;
            if (nb_calls >90 && nb_calls<95 )
            {
                count =1+r->level*r->collect_time;
                decreaseResource(r, count);
                return count;
            }
            else if (nb_calls >95)
            {
                nb_calls = 0;
            }

            return count;
        }
        else
        {
            moveP(p, r->r.x, r->r.y);
        }
    }
    return count;
}

/**
*   \brief allow a personage to attack an animal
*   \todo test
*/

int attack_A(Perso* p)
{
    static int nbCall;
    int attack_distance;
    Animals * ani;
    if (p != NULL && p->t.t !=NULL)
    {
        ++nbCall;
        ani = (Animals*) p->t.t;
        attack_distance  = computeDistance(p->perso_image.x, p->perso_image.y, ani->r.x, ani->r.y);
        if ((ani->type == WOLF || ani->type == BOAR)&& ani->view_range>attack_distance)
            ani->target = (void*)p;
        if(p->skills.attack_range > attack_distance && nbCall%50==0)
        {
            if (ani->life_point <=0)
            {
                setActionFlagP(p, WAIT);
                return 1;
            }
            else if(ani->life_point >0)
            {
                ani->life_point-=abs(p->skills.damage)/2;
                setActionFlagP(p, ATTACK);
            }
            nbCall=1;
        }
        else
        {
            follow_animal(p, ani);
        }
        return 0;
    }
    return -1;
}

/**
*   \brief allow an animal to attack a character
*   \todo test
*/

int animal_ATT(Animals * ani)
{
    static int nbCall;
    int attack_distance;
    Mouvment move;
    Perso *p= NULL;
    if (ani != NULL && ani->target !=NULL)
    {
        ++nbCall;
        p = (Perso*) ani->target;
        attack_distance  = computeDistance(ani->r.x + ani->r.w/2, ani->r.y + ani->r.h/2,p->perso_image.x, p->perso_image.y);
        if(attack_distance < ATTACK_RANGE_ANI)
        {
            if (p->skills.hp <=0)
            {
                return 1;
            }
            else if(p->skills.hp >0 && ani->life_point >0 && nbCall%60 == 0)
            {
                setAttacked_by_animal(p, ani);
                p->skills.hp-=abs(ani->dommage - p->skills.armor)/2;
                nbCall = 1;
            }
        }
        else
        {
            move = follow (ani,p);
            move_animal_to_character(move, ani);
        }
        return 0;
    }
    return -1;
}


/**
*   \brief allow a personage to attack a building
*
*   \note the target of the building has to be set first
*   \note the first version does not allow us to use that part of the code
*   \todo tests
*/

int attackB(Perso *p)
{
    static int time_off=1;
    int attack_distance, enemy_team_number, p_team_number;
    Building * enemy;
    if (p != NULL && p->t.t !=NULL)
    {
        enemy = (Building*) p->t.t;
        enemy_team_number = enemy->team;
        p_team_number = getTeam(p);
        attack_distance  = computeDistance(p->perso_image.x, p->perso_image.y, enemy->building_r.x, enemy->building_r.y);
        if(p->skills.attack_range > attack_distance && enemy_team_number != p_team_number && (time_off%5)==0)
        {
            if (enemy->hp <=0 )
            {
                setActionFlagP(p, WAIT);
                return 1;
            }
            else if(p->skills.hp >0 && enemy->hp >0)
            {
                enemy->hp-=(p->skills.damage)/10;
            }
            time_off = 1;
        }
        else
        {
            move_p(p, enemy->building_r.x, enemy->building_r.y);
            ++time_off;
        }

        return 0;
    }
    return -1;
}


/*For the next functions, there is a distinction between build/create and construct
Build/create => initializes
Construct =>  gives hp until the hp is full
note that this implementation is for now based on the hp and that is not correct*/




/**
*   \brief to enable the construction of one building depending on the type
*/
static int constructEna (Player *p, int type)
{
    if (p)
    {
        switch (type)
        {
        case CITY_HALL:
            if (p->available_resources[GOLD] > 500 && p->available_resources[WOOD]>600 &&
                    p->available_resources[STONE]> 400 && p->nbCityHall < MAX_CITYHALL)
            {
                p->available_resources[GOLD] -= 500;
                p->available_resources[WOOD] -= 600;
                p->available_resources[STONE]-= 400;
                p->nbCityHall +=1;
                return 1;
            }
            break;
        case HOUSE:
            if (p->available_resources[WOOD]>200 && p->available_resources[STONE]> 100 &&
                    p->nbCityHall < MAX_HOUSES)
            {
                p->available_resources[WOOD] -= 200;
                p->available_resources[STONE]-= 100;
                p->nbHouses +=1;
                return 1;
            }
            break;
        case BARRACKS:
            if (p->available_resources[GOLD] > 100 && p->available_resources[WOOD]>250 &&
                    p->available_resources[STONE]> 200 && p->nbCityHall < MAX_BARRACKS)
            {
                p->available_resources[GOLD] -= 100;
                p->available_resources[WOOD] -= 250;
                p->available_resources[STONE]-= 200;
                p->nbBarracks +=1;
                return 1;
            }
            break;
        case TOWER:
            if (p->available_resources[GOLD] > 300 && p->available_resources[WOOD]>100 &&
                    p->available_resources[STONE]> 300 && p->nbCityHall < MAX_TOWER)
            {
                p->available_resources[GOLD] -= 300;
                p->available_resources[WOOD] -= 100;
                p->available_resources[STONE]-= 300;
                p->nbTower +=1;
                return 1;
            }
            break;
        }
    }
    return 0;
}


/**
*   \brief allows a personage to build a building
*   \todo move the perso if you are not close enough to build
*
*   \param \a p the personage that create a building depending on his \a type
*/

Building * buildB (Player *pl, int type)
{
    Perso *p = pl->selectP[0];
    int i=0;
    if (p!=NULL && pl!=NULL)
    {
        if(p->dest_x == p->perso_image.x && p->dest_y == p->perso_image.y)
        {
            if(constructEna(pl, type)==1)
            {
                Building *b = init_building(type);
                setActionFlagP(p, CONSTRUCT);
                setTarget(p, (void*)(b), TARGET_BUILDING);
                b->building_r.x=p->perso_image.x-5;
                b->building_r.y=p->perso_image.y-5;
                b->team = pl->team_number;
                if (pl->selectP[i])
                {
                    do
                    {
                        setActionFlagP(pl->selectP[i], CONSTRUCT);
                        setTarget(pl->selectP[i], (void*)(b), TARGET_BUILDING);
                        ++i;
                    }
                    while(pl->selectP[i]);

                }
                return b;
            }
            else
            {
                if (pl->selectP[i])
                {
                    do
                    {
                        setActionFlagP(pl->selectP[i], WAIT);
                        setTarget(pl->selectP[i], NULL, TARGET_INVALID);
                        ++i;
                    }
                    while(pl->selectP[i]);

                }
            }
        }
        else
        {
            moveP(p,p->dest_x, p->dest_y);
        }
    }
    return NULL;
}



/**
*   \brief to enable the construction of one building depending on the type
*/
static int constructCharacterEna (Player *p, Building *b, int type)
{
    if (p && b->action != CONSTRUCT_P)
    {
        switch (type)
        {
        case VILLAGER:
            if (p->available_resources[WOOD]>50 && p->available_resources[FOOD]> 100 &&
                    p->nbCharacters < MAX_PERSO + 4*p->nbHouses && b->type != TOWER && b->type != BARRACKS)
            {
                p->available_resources[WOOD] -= 50;
                p->available_resources[FOOD]-= 100;
                p->nbVillagers +=1;
                p->nbCharacters+=1;
                return 1;
            }

            break;
        case SWORDMAN:
            if (p->available_resources[WOOD]>75 && p->available_resources[FOOD]> 150 &&
                    p->nbCharacters < MAX_PERSO + 4*p->nbHouses && b->type == BARRACKS)
            {
                p->available_resources[WOOD] -= 75;
                p->available_resources[FOOD]-= 150;
                p->nbCharacters +=1;
                return 1;
            }

            break;
        case ARCHER:
            if (p->available_resources[WOOD]>100 && p->available_resources[STONE]> 125 &&
                    p->nbCharacters < MAX_PERSO + 4*p->nbHouses && b->type == BARRACKS)
            {
                p->available_resources[WOOD] -= 100;
                p->available_resources[FOOD] -= 125;
                p->nbCharacters +=1;
                return 1;
            }
            break;
        }
    }
    return 0;
}


/**
*   \brief allows a building to create a personage
*
*   \param \a b the building that create a personage depending on his \a type
*/

Perso * createPerso (Building *b, Player *pl,  int type)
{
    if (b!=NULL && pl!=NULL)
    {
        if (constructCharacterEna(pl,b,type)==1)
        {
            Perso *p = initPerso(type);
            setActionFlagP(p, DO_NOTHING);
            p->team = pl->team_number;
            setCoord(p, b->building_r.x+50, b->building_r.y+60);
            setActionFlag(b, CONSTRUCT_P);
            setTargetB(b, (void*) p);
            return p;
        }
    }
    return NULL;
}


/**
*   \brief free the player
*/
void freePlayer(Player *pl)
{
    if(pl)
    {

        pl->s.target=NULL;

        free(pl);
    }
}

/**
*   \brief gives hp to a personage
*
*   \note this function is only used by a building
*
*   \param \a b, the building that constructs the personage
*/

void constructP(Building *b)
{
    static int time_off=1;
    if (b!= NULL && b->target != NULL && (time_off%2)==0 )
    {
        Perso *p = (Perso *) b->target;
        switch (p->type)
        {
        case VILLAGER:
            if (p->skills.hp < VILLAGER_HP)
                p->skills.hp +=2;
            else
            {
                p->dest_x = b->building_r.x + b->building_r.w/2;
                p->dest_y = b->building_r.y + b->building_r.h/2+15;
                setActionFlagP(p, MOVE);
                setTargetB(b, NULL);
                setActionFlag(b, SLEEP);
            }
            break;
        case ARCHER:
            if (p->skills.hp < ARCHER_HP)
                p->skills.hp +=2;
            else
            {
                p->dest_x = b->building_r.x + b->building_r.w/2;
                p->dest_y = b->building_r.y + b->building_r.h/2+15;
                setActionFlag(b, MOVE);
                setTargetB(b, NULL);
                setActionFlag(b, SLEEP);
            }
            break;
        case SWORDMAN:
            if (p->skills.hp < SWORDMAN_HP)
                p->skills.hp +=2;
            else
            {
                p->dest_x = b->building_r.x + b->building_r.w/2;
                p->dest_y = b->building_r.y + b->building_r.h/2+15;
                setActionFlag(b, MOVE);
                setTargetB(b, NULL);
                setActionFlag(b, SLEEP);
            }
            break;
        }
        time_off=1;
    }
    ++time_off;
}


/**
*   \brief gives hp to a Building
*
*   \note this function is only used by a personage
*   \note this will later allow a personage to repair buildings
*
*   \param \a p, the personage that constructs the building
*/

void constructB(Perso *p)
{
    static int time_off=1;
    if (p!= NULL && p->t.t != NULL && p->type==VILLAGER && (time_off%3)==0)
    {
        Building *b = (Building *) p->t.t;
        switch (b->type)
        {
        case HOUSE:
            if (getHP(b) < HOUSE_HP)
            {
                b->hp +=1;
            }
            else
            {
                setActionFlagP(p, MOVE);
                setTarget(p, NULL, TARGET_INVALID);
            }
            break;
        case CITY_HALL:
            if (getHP(b) < CITY_HALL_HP)
                b->hp +=1;
            else
            {
                setActionFlagP(p, MOVE);
                setTarget(p, NULL, TARGET_INVALID);
            }
            break;
        case BARRACKS:
            if (getHP(b)< BARRACKS_HP)
                b->hp +=1;
            else
            {
                setActionFlagP(p, MOVE);
                setTarget(p, NULL, TARGET_INVALID);
            }
            break;
        case TOWER:
            if (getHP(b)< TOWER_HP)
                b->hp +=1;
            else
            {
                setActionFlagP(p, MOVE);
                setTarget(p, NULL, TARGET_INVALID);
            }
            break;
        }
        time_off =1;
    }
    ++time_off;
}

/**
*   \brief to move a personage
*
*   \note this only calls for the perso.c function
*
*/


void moveP(Perso *p, int x, int y)
{
    move_p(p, x, y);
}


/**
*   \brief to update the player's selection
*
*
*/

int setSelect(Player *p, void * target, int select_type)
{
    if (p!= NULL)
    {
        p->s.target = target;
        p->s.target_type=select_type;
        return 0;
    }
    return -1;
}


/**
*   \brief to add a  character to the player's selection
*
*/
int addSelectedP(Player *pl, Perso *p)
{
    int i;
    if (p && pl)
    {
        for (i=0; i<MAX_SELECT; i++)
        {
            if (pl->selectP[i]==NULL && pl->team_number==p->team)
            {
                pl->selectP[i]=p;
                return 0;
            }
        }
    }
    return-1;
}



/**
*   \brief to access the type of the player's selection
*
*   \return the type of the selection
*
*/

int targetType (Perso *p)
{

    if(p!= NULL)
    {
        return p->t.type;
    }
    return TARGET_INVALID;
}

/**
*   \brief to clear the player's selection
*
*/
void clear_selectP(Player *p)
{
    int i;
    if (p)
    {
        for (i=0; i<MAX_SELECT; i++)
            if(p->selectP[i]!=NULL)
                p->selectP[i]=NULL;
            else
                break;
        if (p->s.target)
        {
            p->s.target=NULL;
            p->s.target_type=TARGET_INVALID;
        }
    }
}


/**
*   \brief determine the mouvment that an animal have to do to follow a character
*
*   different mouvments : left, right, above, bellow, left_above, left_bellow, right_above, right_bellow
*
*   \return the right mouvment
*/

Mouvment follow (Animals * animals_name, Perso* p)
{
    int temps_min = 0;
    int shortest_dist = 100000;
    Mouvment move;


    if ((temps_min = computeDistance ((animals_name->r.x)-1, animals_name->r.y, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = LEFT;
    }
    if ((temps_min = computeDistance ((animals_name->r.x)+1, animals_name->r.y, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = RIGHT;
    }
    if ((temps_min = computeDistance (animals_name->r.x, (animals_name->r.y)-1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = ABOVE;
    }
    if ((temps_min = computeDistance (animals_name->r.x, (animals_name->r.y)+1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = BELLOW;
    }
    if ((temps_min = computeDistance ((animals_name->r.x)+1, (animals_name->r.y)+1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = RIGHT_BELLOW;
    }
    if ((temps_min = computeDistance ((animals_name->r.x)+1, (animals_name->r.y)-1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = RIGHT_ABOVE;
    }
    if ((temps_min = computeDistance ((animals_name->r.x)-1, (animals_name->r.y)+1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = LEFT_BELLOW;
    }
    if ((temps_min = computeDistance ((animals_name->r.x)-1, (animals_name->r.y)-1, p->perso_image.x, p->perso_image.y)) < shortest_dist)
    {
        shortest_dist = temps_min;
        move = LEFT_ABOVE;
    }

    return move;

}


/**
*   \brief used to decrease life point of an animal after a character attack
*
*   \return 1 whether everything is OK
*/


int decrease_life_point (Animals * animals_name, Perso * p)
{
    if (animals_name != NULL)
    {
        animals_name->life_point -= p->skills.damage;
        return 1;
    }
    return 0;
}

/**
*   \brief determine whether or not the character is seen by the animal
*
*   \return 1 whether the character is seen
*/

int character_is_seen (Animals * animals_name, Perso * p)
{
    int center_tile_x = (animals_name->r.x + animals_name->r.w)/2;
    int center_tile_y = (animals_name->r.y + animals_name->r.h)/2;

    if ((abs(p->perso_image.x - center_tile_x) <= animals_name->view_range) && (abs(p->perso_image.y - center_tile_y) <= animals_name->view_range))
        return 1;

    else
        return 0;
}


/**
*   \brief determine whether or not the character is located in the collecting area
*
*   \return 1 whether the character is in area
*/

int is_in_area (Resource * r, Perso * p)
{
    if(r!=NULL && p!=NULL)
    {
        int range = p->skills.range;
        int center_tile_x = r->r.x + r->r.w/2;
        int center_tile_y = r->r.y + r->r.h/2;

        if (abs(p->perso_image.x - center_tile_x) < range/3 + r->r.w/2 &&
                abs(p->perso_image.y - center_tile_y) < range/3 + r->r.h/2)
            return 1;
    }
    return 0;
}

/**
*   \brief move the animal according to the movement determined by the function follow
*
*   \return 1 whether everything is ok
*/
int move_animal_to_character (Mouvment move, Animals * animals_name)
{
    switch (move)
    {
    case LEFT :
        move_left(animals_name);
        return 1;
        break;
    case RIGHT :
        move_right(animals_name);
        return 1;
        break;
    case ABOVE :
        move_above(animals_name);
        return 1;
        break;
    case BELLOW :
        move_bellow(animals_name);
        return 1;
        break;
    case LEFT_BELLOW :
        move_left_bellow(animals_name);
        return 1;
        break;
    case LEFT_ABOVE :
        move_left_above(animals_name);
        return 1;
        break;
    case RIGHT_BELLOW :
        move_right_bellow(animals_name);
        return 1;
        break;
    case RIGHT_ABOVE :
        move_right_above(animals_name);
        return 1;
        break;
    }
    return 0;
}

/**
*   \brief allows to follow an animal
*
*   \return 0 whether everything is ok
*/

int follow_animal(Perso* p, Animals * ani)
{
    if(p!=NULL && ani != NULL)
    {
        int x_ani;
        int y_ani;
        get_coordo_animals (ani, &x_ani, &y_ani);
        move_p(p, x_ani, y_ani);
        return 0;
    }
    return -1;
}

/**
*   \brief allows to change the target of a character while he is attacked by an animal
*
*   \return 0 whether everything is ok
*/

int setAttacked_by_animal(Perso* p, Animals* attacker)
{
    if( p !=NULL && attacker != NULL)
    {
        setTarget(p, (void*) attacker, TARGET_ANIMALS);
        setActionFlagP(p, ATTACK);
        return 0;
    }
    return -1;
}


