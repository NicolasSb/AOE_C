/**
 *  \file tile.h
 *  \brief Definitions for handling tiles and tilesets
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 1st 2017
 */

#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <SDL.h>

/**
 * \struct Tile
 * \brief Ground tile
 */
typedef struct Tile Tile;
struct Tile
{
    SDL_Rect clip;          /*!< Position and size of the tile in the tileset image */
    SDL_Rect collision;     /*!< Position and size of the actual object (for collisions) */
};

/**
 * \struct Tileset
 * \brief Tileset structure used for loading a tileset at the map loading
 */
typedef struct Tileset Tileset;
struct Tileset
{
    SDL_Surface *t;
    int nb_row;
    int nb_col;
    Tile *tiles;
};

enum TlstType {
    TLST_GROUND,
    TLST_PERSO,
    TLST_BUILDING,
    TLST_RESOURCE,
    TLST_ANIMAL,
    TLST_BLUEMASK,
    TLST_REDMASK};

/**
 *  \fn Tileset loadTilesetFromFile(char *filename)
 *  \brief Loads a tileset from a tlst file
 *  \param filename the name of the tlst file
 *  \return the filled tileset
 */
Tileset *loadTilesetFromFile(char *filename);

/**
 *	\fn void freeTileset(Tileset *t)
 *	\brief Frees a tileset
 *	\param t the tileset to free
 */
void freeTileset(Tileset *t);

#endif
