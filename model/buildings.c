
#include <stdio.h>
#include <stdlib.h>
#include "buildings.h"
#include "files.h"

/**
*
*   \file buildings.c
*   \brief Buildings management
*
*   \author Sourbier N.
*
*   \version 1.0
*
*   \date 15/02/2017
*
*   \note Test coverage is 98.04%
*   \todo change some functions' scope to static
*   \todo add tests on the untested functions
*/

Building * init_building(int type)
{
    Building *b = calloc(1, sizeof(Building));
    if (! b){
        return NULL;
    }
    b->action=SLEEP;
    b->building_r.h=120;
    b->building_r.w=120;
    b->building_r.x=0;
    b->building_r.y=100;
    b->dist_to_cam = 0;
    b->target = NULL;
    switch (type){
    case HOUSE:
        b->type=HOUSE;
        b->hp=0;
        b->range=50;
        break;
    case BARRACKS:
        b->type=BARRACKS;
        b->hp=0;
        b->range=60;
        break;
    case CITY_HALL:
        b->type=CITY_HALL;
        b->hp=0;
        b->range=80;
        break;
    case TOWER:
        b->type=TOWER;
        b->hp=0;
        b->range=80;
        break;
    }
    return b;
}


/**
*   \brief used to free a Building
*   \param \a b the Building to free
*/
int freeBuilding (Building *b)
{
    if (b){
        b->target = NULL;
        free(b);
        return 0;
    }
    return -1;
}

/**
*   \brief used to set the action flag of a building
*   \param \a b the Building to set the flag \a flag
*/

void setActionFlag(Building *b, int flag)
{
    if (b!=NULL)
        b->action = flag;
}

/**
*   \brief to get thee action Flag of a building
*   \param \a b the Building to get the action Flag
*/

int getActionFlag(Building *b)
{
    if (b!= NULL)
        return b->action;
    return -1;
}

/**
*   \brief to set the target of a building
*   \param \a b, the building to update the target, \a target
*   \todo tests with cmocka
*/
void setTargetB(Building *b, void *target)
{
    if (b)
        b->target=target;
}

/**
*   \brief to access the hp of a building
*   \param \a b the building
*   \return int -> HP of the building
*/

int getHP(Building *b)
{
    if (b!= NULL)
        return b->hp;
    return -1;
}
