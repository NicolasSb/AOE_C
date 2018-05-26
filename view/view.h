/**
 *  \file view.h
 *  \brief Definitions of view functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 5th 2017
 */

#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "../model/map.h"
#include "application.h"

#include <SDL.h>


enum {CAM_UP, CAM_DOWN, CAM_LEFT, CAM_RIGHT};

/**
 *  \fn void moveCamera(App *app, int dir)
 *  \brief Moves the camera (ie the view) in some direction
 *  \param app A pointer on the application
 *  \param dir The direction in which the view is moved
 */
void moveCamera(App *app, int dir);

/**
 *  \fn void blitGroundTiles(SDL_Surface *screen, GameMap *m)
 *  \brief Blits all the ground tiles at once
 *  \param app the main application
 *  \param m the map from which to take the tiles
*/
void blitGroundTiles(App *app, GameMap *m);

/**
 *  \fn void blitSecondLayer(App *app, List *p, List *b, GameMap *m)
 *  \brief Blits all the objects of the second layer
 *  \param app the main application
 *  \param p the list of perso
 *  \param b the list of buildings
 *  \param m the world map
 *  \todo Add the ressources
 */
void blitSecondLayer(App *app, List *p, List *b, GameMap *m);

/**
 *  \fn void updateScreen(SDL_Surface *screen)
 *  \brief Updates the screen
 *  \param screen the screen to update
*/
void updateScreen(SDL_Surface *screen);

/**
*   \fn void drawRectangleSelection (App *app, SDL_Rect r)
*   \brief draw a zone that "select" characters
*   \param app the main application
*   \param r the rectangle representing the selection
*/
void drawRectangleSelection (App *app, SDL_Rect r);


void blitSavage(App *app, List *lp, List *lb, List *lr, List *la, GameMap *m);
#endif
