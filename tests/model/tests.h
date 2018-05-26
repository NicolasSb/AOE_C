/**
 *  \file test.h
 *  \brief Definitions of all the test functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 1st 2017
 */

#ifndef TESTS_H_INCLUDED
#define TESTS_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

/**
 *  \fn void initTestData()
 *  \brief Launches a bash script which makes sure files created during previous
            tests are removed from the data directory
*/
void initTestData();

/**
 *  \fn void runTests(void)
 *  \brief Straightforward function which launches each test series individually
*/
void runTests(void);

/*
**********************************************************************
				DECLARATIONS OF TEST FUNCTIONS
Each function corresponds to one C file to testFiles
**********************************************************************
*/

int testFiles(void);
int testMap(void);
int testTile(void);
int test_perso(void);
int test_player(void);
int testBuildings(void);
int testList(void);
int testMapChunk(void);
int testQTree(void);
int test_animals(void);

/* This structure and its related functions are here just for the tests which need to have a SDL_context*/

/**
 * \struct ViewElements
 * \brief Basic screen + one image for tests purpose
 */
typedef struct ViewElements {
    SDL_Surface *screen;    /*!< the screen used during the game */
    SDL_Surface *image;     /*!< Surface for loading an image */
} ViewElements;

/**
 *  \fn ViewElements *initViewElements()
 *  \brief Initializes an empty ViewElements structure and initializes SDL
 *  \return A pointer on a ViewElements structure
*/
ViewElements *initViewElements();

/**
 *  \fn void freeViewElements(ViewElements *v)
 *  \brief Frees a ViewElements structure pointer and stop SDL
 *  \param v the pointer to free
*/
void freeViewElements(ViewElements *v);

#endif
