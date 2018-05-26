/**
 *  \file tile.h
 *  \brief Declarations for Tileset and Tile functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 1st 2017
 */

#include "tile.h"
#include "files.h"

static void readCollisionInfo(Tileset *t, FILE *f)
{
    int i = 0, j = 0;

    int px = 0, py = 0, w = 0, h = 0;

    for(i=0; i < t->nb_row; i++)
    {
        for(j=0; j < t->nb_col; j++)
        {
            int index_cur = i*t->nb_col + j;
            fscanf(f, "%d %d %d %d\n", &px, &py, &w, &h);

            t->tiles[index_cur].collision.x = px;
            t->tiles[index_cur].collision.y = py;
            t->tiles[index_cur].collision.w = w;
            t->tiles[index_cur].collision.h = h;
        }
    }
}

Tileset *loadTilesetFromFile(char *filename)
{
    FILE *f = openFile(filename, "r");
    Tile tile;
    Tileset *tlst;
    char tileset_file[MAX_FILENAME];
    int tile_w = 0, tile_h = 0;
    int i, j;

    if(!f)
		return NULL;

    tlst = (Tileset *) calloc(1, sizeof(Tileset));
    if(!tlst)
    {
		fprintf(stderr, "Error when allocating tileset at line %d, in %s\n",
                __LINE__, __FILE__);
        return NULL;
    }

    fscanf(f, "%s\n", tileset_file);

	tlst->t = loadImage(tileset_file);

    fscanf(f, "%d %d\n", &(tlst->nb_row), &(tlst->nb_col));

    tlst->tiles = (Tile *) calloc(tlst->nb_row * tlst->nb_col, sizeof(Tile));

    if(!tlst->tiles)
    {
    	fprintf(stderr, "Error when allocating tile array at line %d, in %s\n",
                __LINE__, __FILE__);
        return NULL;
    }


    for(i=0; i < tlst->nb_row; i++)
    {
        fscanf(f, "%d %d\n", &tile_w, &tile_h);

        tile.clip.w = tile_w;
        tile.clip.h = tile_h;

        for(j=0; j < tlst->nb_col; j++)
        {
            tile.clip.x = j*tile_w;
            tile.clip.y = i*tile_h;

            tlst->tiles[i*tlst->nb_col + j] = tile;
        }

        fscanf(f, "\n");
    }

    readCollisionInfo(tlst, f);

    return tlst;
}

void freeTileset(Tileset *tlst)
{
	if(tlst)
	{
	    if(tlst->tiles)
            free(tlst->tiles);
        if(tlst->t)
            SDL_FreeSurface(tlst->t);
		free(tlst);
	}
}
