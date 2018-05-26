/**
 * \file tests.c
 * \brief Unit tests for all functions
 * \author Léo Gauvin
 * \version 0.1
 * \date March, 4th 2017
 */

#include "tests.h"
#include <stdlib.h>

ViewElements *initViewElements()
{
    ViewElements *v = (ViewElements *) calloc(1, sizeof(ViewElements));

    if(!v)
        return NULL;

    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Initialization error from SDL Video: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_WM_SetCaption("SDL windows", NULL);

    v->screen = NULL;
    v->image = NULL;

    return v;
}

void freeViewElements(ViewElements *v)
{
    if(v != NULL)
    {
        if(v->image != NULL)
            SDL_FreeSurface(v->image);
        if(v->screen != NULL)
            SDL_FreeSurface(v->screen);
        free(v);
    }

    SDL_Quit();
}

void initTestData()
{
    system("./tests/init_testdata.sh");
}


/*
All the remaining Valgrind errors seem to be caused by SDL implementation
especially by SDL_Init
*/

void runTests(void)
{
    initTestData();
    testFiles();
    testTile();
    /*testMap();*/
    test_perso();
    test_player();
    testBuildings();
    testList();
    testMapChunk();
    testQTree();
    test_animals();
}



