/**
 *  \file iso_utils.h
 *  \brief Implementation of utilities for iso coordinates
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date April, 5th 2017
 */

#include "view_constants.h"
#include "iso_utils.h"
#include "application.h"
#include "../model/files.h"

#include <stdio.h>
#include <math.h>

/**
 *  \fn static Uint32 getPixel_32(SDL_Surface *surface, int x, int y)
 *  \brief Returns the pixel at a given position. This function is given on SDL website.
 *  \param surface the surface on which to retrieve the pixel
 *  \param x the x position of the pixel
 *  \param y the y position of the pixel
 *  \return a Uint32 containing the pixel components
 */
static Uint32 getPixel_32(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address of the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;
    }
}

/**
 *  \fn static MMPos mousePosFineTuning(SDL_Surface *tile_mask, Uint32 pix)
 *  \brief Returns the position of the mouse on the mouse map image given the pixel color
 *  \param tile_mask the surface containing the mouse map image
 *  \param pix the pixel which color allows us to tell where is the mouse
 *  \return an integer corresponding to the area on which the mouse is
 */
static MMPos mousePosFineTuning(SDL_Surface *tile_mask, Uint32 pix)
{
    Uint8 r, g, b;

    SDL_GetRGB(pix, tile_mask->format, &r, &g, &b);

    if(r==0xFF && g==0 && b==0)
        return MM_NW;
    else if(r==0 && g==0xFF && b==0)
        return MM_SW;
    else if(r==0 && g==0 && b==0xFF)
        return MM_SE;
    else if(r==0xFF && g==0xFF && b==0)
        return MM_NE;
    else
        return MM_CENTER;
}

SDL_Rect tilePlotter(SDL_Rect *pos_map)
{
    SDL_Rect pos_world;
    pos_world.x = (pos_map->x - pos_map->y)* STEP_X;
    pos_world.y = (pos_map->x + pos_map->y) * STEP_Y;
    pos_world.h = TILE_HEIGHT;
    pos_world.w = TILE_WIDTH;

    return pos_world;
}

SDL_Rect tileWalker(SDL_Rect start, IsoDir dir)
{
    switch(dir)
    {
        case ISO_N:
        {
            start.x--;
            start.y--;
        }
        break;

        case ISO_NE:
        {
            start.y--;
        }
        break;

        case ISO_E:
        {
            start.x++;
            start.y--;
        }
        break;

        case ISO_SE:
        {
            start.x++;
        }
        break;

        case ISO_S:
        {
            start.x++;
            start.y++;
        }
        break;

        case ISO_SW:
        {
            start.y++;
        }
        break;

        case ISO_W:
        {
            start.x--;
            start.y++;
        }
        break;

        case ISO_NW:
        {
            start.x--;
        }
        break;
    }

    return start;
}

SDL_Rect mouseMap(App *app, SDL_Rect *mouse_pos)
{
    SDL_Rect mouse_map_coarse;
    SDL_Rect map_pos;
    SDL_Rect mouse_map_fine;
    Uint32 pix;
    /* Screen to world conversion */
    mouse_pos->x += app->camera.x;
    mouse_pos->y += app->camera.y;

    /* Determine coarse position */

    mouse_map_coarse.x = floor(mouse_pos->x / TILE_WIDTH);
    mouse_map_coarse.y = floor(mouse_pos->y / TILE_HEIGHT);

    if(mouse_pos->x < 0)
        mouse_map_coarse.x--;
    if(mouse_pos->y < 0)
        mouse_map_coarse.y--;

    map_pos.x = mouse_map_coarse.x + mouse_map_coarse.y;
    map_pos.y = mouse_map_coarse.y - mouse_map_coarse.x;

    /* Determine fine position */
    mouse_map_fine.x = mouse_pos->x % TILE_WIDTH;
    mouse_map_fine.y = mouse_pos->y % TILE_HEIGHT;

    pix = getPixel_32(app->mousemap, mouse_map_fine.x, mouse_map_fine.y);

    /* Fine tuning with mouse map image */
    switch(mousePosFineTuning(app->mousemap, pix))
    {
        case MM_CENTER:
            break;

        case MM_NE:
            map_pos = tileWalker(map_pos, ISO_NE);
            break;

        case MM_SE:
            map_pos = tileWalker(map_pos, ISO_SE);
            break;

        case MM_SW:
            map_pos = tileWalker(map_pos, ISO_SW);
            break;

        case MM_NW:
            map_pos = tileWalker(map_pos, ISO_NW);
            break;
    }

    return map_pos;
}
