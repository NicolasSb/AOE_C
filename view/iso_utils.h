/**
 *  \file iso_utils.h
 *  \brief Definitions of utilities for iso coordinates
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date April, 5th 2017
 */

#ifndef ISO_UTILS_H_INCLUDED
#define ISO_UTILS_H_INCLUDED

#include <SDL.h>
#include "application.h"
#include "view_constants.h"

/**
 *  \enum IsoDir
 *  \brief Enumeration to represent the possible directions in iso coordinates
 */
enum IsoDir {
    ISO_N,
    ISO_NE,
    ISO_E,
    ISO_SE,
    ISO_S,
    ISO_SW,
    ISO_W,
    ISO_NW
};
typedef enum IsoDir IsoDir;

/**
 *  \enum MouseMapPos
 *  \brief Enumeration to represent the possible position of the mouse on the mouse map image
 */
enum MouseMapPos {
    MM_CENTER,
    MM_NE,
    MM_SE,
    MM_SW,
    MM_NW
};
typedef enum MouseMapPos MMPos;

/**
 *  \fn SDL_Rect tilePlotter(SDL_Rect *pos_map)
 *  \brief Computes the corresponding world position for a matrix position
 *  \param pos_map SDL_Rect containing matrix positions
 *  \return A SDL_Rect containing world position
 */
SDL_Rect tilePlotter(SDL_Rect *pos_map);

/**
 *  \fn SDL_Rect tileWalker(SDL_Rect start, IsoDir dir)
 *  \brief Gives the resulting tile position after a one-tile move in direction dir
 *  \param start SDL_Rect containing starting position
 *  \param dir the direction of the movement
 *  \return the resulting tile position after the movement
 */
SDL_Rect tileWalker(SDL_Rect start, IsoDir dir);

/**
 *  \fn SDL_Rect mouseMap(App *app, SDL_Rect *mouse_pos)
 *  \brief Returns the matrix position on which the mouse is pointing
 *  \param app the main application
 *  \param mouse_pos a SDL_Rect containing the mouse position on the screen
 *  \return a SDL_Rect containing the matrix position at the given mouse position
 */
SDL_Rect mouseMap(App *app, SDL_Rect *mouse_pos);

#endif
