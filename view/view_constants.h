/**
 *  \file view_constants.h
 *  \brief Definitions of constants used for the view
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 5th 2017
 */

#ifndef VIEW_CONSTANTS_H_INCLUDED
#define VIEW_CONSTANTS_H_INCLUDED

#define TILE_WIDTH      64
#define TILE_HEIGHT     32

#define NB_COL          8
#define NB_ROW          8

#define NB_TILES        NB_COL * NB_ROW

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

#define STEP_X          TILE_WIDTH / 2
#define STEP_Y          TILE_HEIGHT / 2

#define X_OFFSET        SCREEN_WIDTH / 2 - TILE_WIDTH / 2

#define CAM_DISPLACEMENT    4

#endif /* VIEW_CONSTANTS_H_INCLUDED*/
