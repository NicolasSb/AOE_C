#include <stdio.h>
#include <stdlib.h>
#include "ressource.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
/**
*
*   \file ressource.c
*
*   \brief Resource model functions
*
*   \author Deredempt Nicolas / Sourbier Nicolas
*
*   \date 07/03/2017
*/


/**
*   \brief the initialization of a resource depending on a type
*
*   see file ressource.h to know how to complete the fields of the structure
*
*   \return the initialized resource
*/
Resource *initResource(int type)
{
    Resource *r = calloc(1, sizeof(Resource));
    if (r==NULL)
        return NULL;
    r->level=1;                         /* 3 levels implemented to improve collect_time*/
    r->r.x = 5;
    r->r.y = 5;
    r->r.h = 100;
    r->r.w = 100;
    r->dist_to_cam = 2000000;
    switch (type)
    {
    case WOOD:
        r->type = WOOD;
        r->amount= WOOD_AMOUNT;
        r->collect_time=WOOD_COLLECT_TIME;
        break;
    case STONE:
        r->type = STONE;
        r->amount=STONE_AMOUNT;
        r->collect_time=STONE_COLLECT_TIME;
        break;
    case GOLD:
        r->type = GOLD;
        r->amount=GOLD_AMOUNT;
        r->collect_time=GOLD_COLLECT_TIME;
        break;
    case FOOD:
        r->type = FOOD;
        r->amount=FOOD_AMOUNT;
        r->collect_time=FOOD_COLLECT_TIME;
        break;
    }
    return r;

}




/**
*   \brief used to set a position to a ressource
*/
void set_position (Resource * resource_name, int x, int y)
{
    if(resource_name != NULL)
    {
        resource_name->r.x = x;
        resource_name->r.y = y;
    }
}
/**
*   \brief used to get the position to a ressource
*/
void get_coordo (Resource * resource_name, int *x, int *y)
{
    if (resource_name != NULL)
    {
    *x = resource_name->r.x;
    *y = resource_name->r.y;
    }
}
/**
*   \brief improve the collect time based on the level of improvment
*
*   3 levels of collect time will be implemented
*
*   \return 1 whether everything is ok
*/
int improve_collect_time (Resource * resource_name, int level)
{
    if(resource_name != NULL)
    {
    resource_name->level = level;
    resource_name->collect_time *= level;
    return 1;
    }
return 0;
}
/**
*   \brief decrease the amount of a ressource
*
*   \return 1 whether everything is ok
*/
int decreaseResource(Resource *resource_name, int collect)
{
    if (resource_name != NULL)
    {
    resource_name->amount -= collect;
    if (resource_name->amount ==0)
    	return -1;
    return 1;
    }
return 0;
}


/**
*   \brief free a resource
*/
void freeResource(Resource *r)
{
    if(r)
        free(r);
}
