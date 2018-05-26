#include <stdio.h>
#include <stdlib.h>
#include "animals.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/**
*
*   \file animals.c
*
*   \brief Animals model fonctions
*
*   \author Deredempt Nicolas
*
*   \date 07/03/2017
*/

/**
*   \brief the initialisation of a wolf
*
*   see file animals.h to know how to complete the fields of the structure
*
*   \return the initialized animal
*/

Animals *initWolf()
{
    Animals * animals_name = malloc(sizeof(Animals));
    if (animals_name==NULL)
        return NULL;

    animals_name->type = WOLF;
    animals_name->life_point = WOLF_LIFE_POINT;
    animals_name->attack = 0;
    animals_name->dommage = WOLF_DOMMAGE;
    animals_name->speed = WOLF_SPEED;
    animals_name->view_range = ANI_VIEW_RANGE;
    animals_name->target=NULL;
    animals_name->r.h=30;
    animals_name->r.w=30;
    animals_name->r.x=0;
    animals_name->r.y=0;
    animals_name->dist_to_cam = 0;
    animals_name->direction = 0;
    return animals_name;
}

/**
*   \brief the initialisation of a boar
*
*   see file animals.h to know how to complete the fields of the structure
*
*   \return the initialized animal
*/

Animals *initBoar()
{
    Animals * animals_name = malloc(sizeof(Animals));

    if (animals_name==NULL)
        return NULL;

    animals_name->type = BOAR;
    animals_name->life_point = BOAR_LIFE_POINT;
    animals_name->attack = 0;                       /*Two states : 0 isn't attacking, 1 is attacking*/
    animals_name->dommage = BOAR_DOMMAGE;
    animals_name->speed = BOAR_SPEED;
    animals_name->view_range = ANI_VIEW_RANGE;
    animals_name->target=NULL;
    animals_name->r.h=30;
    animals_name->r.w=30;
    animals_name->r.x=0;
    animals_name->r.y=0;
    animals_name->dist_to_cam = 0;
    animals_name->direction = 0;
    return animals_name;
}

/**
*   \brief the initialisation of a sheep
*
*   see file animals.h to know how to complete the fields of the structure
*
*   \return the initialized animal
*/

Animals *initSheep()
{
    Animals * animals_name = malloc(sizeof(Animals));
    if (animals_name==NULL)
        return NULL;

    animals_name->type = SHEEP;
    animals_name->life_point = SHEEP_LIFE_POINT;
    animals_name->attack = 0;
    animals_name->dommage = SHEEP_DOMMAGE;
    animals_name->speed = SHEEP_SPEED;
    animals_name->view_range = ANI_VIEW_RANGE;
    animals_name->target=NULL;
    animals_name->r.h=30;
    animals_name->r.w=30;
    animals_name->r.x=0;
    animals_name->r.y=0;
    animals_name->dist_to_cam = 0;
    animals_name->direction = 0;
    return animals_name;
}

/**
*   \brief the initialisation of a stag
*
*   see file animals.h to know how to complete the fields of the structure
*
*   \return the initialized animal
*/

Animals *initStag()
{
    Animals * animals_name = malloc(sizeof(Animals));
    if (animals_name==NULL)
        return NULL;

    animals_name->type = STAG;
    animals_name->life_point = STAG_LIFE_POINT;
    animals_name->attack = 0;
    animals_name->dommage = STAG_DOMMAGE;
    animals_name->speed = STAG_SPEED;
    animals_name->view_range = ANI_VIEW_RANGE;
    animals_name->target=NULL;
    animals_name->r.h=30;
    animals_name->r.w=30;
    animals_name->r.x=0;
    animals_name->r.y=0;
    animals_name->dist_to_cam = 0;
    animals_name->direction = 0;
    return animals_name;
}

/**
*   \brief give a position to an animal
*/

void change_coordinates (Animals * animals_name, int x, int y)
{
    if (animals_name != NULL)
    {
        animals_name->r.x = x;
        animals_name->r.y = y;
    }
}

/**
*   \brief get the position of an animal
*/

void get_coordo_animals (Animals * animals_name, int *x, int *y)
{
    if (animals_name != NULL)
    {
        *x = animals_name->r.x;
        *y = animals_name->r.y;
    }
}

/**
*   \brief animal moves diagonally (one step left and one step above)
*
*   \return 1 whether everything is OK
*/


int move_left_above (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.x -= animals_name->speed;
        animals_name->r.y -= animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves diagonally (one step right and one step above)
*
*   \return 1 whether everything is OK
*/

int move_right_above (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.x += animals_name->speed;
        animals_name->r.y -= animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves diagonally (one step left and one step bellow)
*
*   \return 1 whether everything is OK
*/

int move_left_bellow (Animals * animals_name)
{
    if(animals_name != NULL)
    {
        animals_name->r.x -= animals_name->speed;
        animals_name->r.y += animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves diagonally (one step right and one step bellow)
*
*   \return 1 whether everything is OK
*/

int move_right_bellow (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.x += animals_name->speed;
        animals_name->r.y += animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves left
*
*   \return 1 whether everything is OK
*/

int move_left (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.x -= animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves right
*
*   \return 1 whether everything is OK
*/

int move_right (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.x += animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves above
*
*   \return 1 whether everything is OK
*/

int move_above (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.y -= animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief animal moves bellow
*
*   \return 1 whether everything is OK
*/

int move_bellow (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->r.y += animals_name->speed;
        return 1;
    }
    return 0;
}

/**
*   \brief determine whether or not the animal is attacking
*
*   \return 1 whether everything is OK
*/

int is_attacking (Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->attack = 1;
        return 1;
    }
    return 0;
}

/**
*   \brief whenever a wolf sees a character or a boar/stag is attacked, its speed increase
*
*   \return 1 whether everything is OK
*/

int enrage (Animals * animals_name) /* animals have a basic speed which increase when the animal is attacked (BOAR) or when it see a character (WOLF)*/
{
    if (animals_name != NULL)
    {
        animals_name->speed *= 2;
        return 1;
    }
    return 0;
}

/**
*   \brief free animal
*/

void freeAnimals(Animals * animals_name)
{
    if (animals_name != NULL)
    {
        animals_name->target = NULL;
        free(animals_name);
    }
}
