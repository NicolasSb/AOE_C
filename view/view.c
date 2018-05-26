/**
 *  \file view.c
 *  \brief Declarations of view functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 5th 2017
 */

#include "view.h"

#include "../model/map.h"
#include "../model/tile.h"
#include "view_constants.h"
#include "iso_utils.h"

void moveCamera(App *app, int dir)
{
    switch(dir)
    {
    case CAM_UP:
        if((app->camera.y - CAM_DISPLACEMENT ) >= app->cam_area.y)
            app->camera.y -= CAM_DISPLACEMENT;
        break;
    case CAM_DOWN:
        if((app->camera.y + CAM_DISPLACEMENT ) <= (app->cam_area.y + app->cam_area.h))
            app->camera.y += CAM_DISPLACEMENT;
        break;
    case CAM_LEFT:
        if((app->camera.x - CAM_DISPLACEMENT ) >= app->cam_area.x)
            app->camera.x -= CAM_DISPLACEMENT;
        break;
    case CAM_RIGHT:
        if((app->camera.x + CAM_DISPLACEMENT ) <= (app->cam_area.x + app->cam_area.w))
            app->camera.x += CAM_DISPLACEMENT;
        break;
    }
}

static void blitTile(App *app, Tileset *tlst, int tile_val, int x, int y)
{
    SDL_Rect tile_clip = tlst->tiles[tile_val].clip;
    SDL_Rect screen_pos;

    SDL_Rect matrix_coord;
    matrix_coord.x = x;
    matrix_coord.y = y;

    /* We need to convert from iso coords to screen coords*/
    screen_pos = tilePlotter(&matrix_coord);
    screen_pos.x -= app->camera.x;
    screen_pos.y -= app->camera.y;

    SDL_BlitSurface(tlst->t, &tile_clip, app->screen, &screen_pos);
}

void blitGroundTiles(App *app, GameMap *m)
{
    int i =0, j = 0;
    SDL_Rect top_left_corner;
    SDL_Rect top_right_corner;
    SDL_Rect bottom_left_corner;
    SDL_Rect bottom_right_corner;

    MatrixData *md = (MatrixData *) m->world_map->data->nd;

    int x_min = 0, x_max = 0;
    int y_min = 0, y_max = 0;

    top_left_corner.x = top_left_corner.y = 0;
    top_left_corner.h = top_left_corner.w = 1;

    top_right_corner.x = app->camera.w;
    top_right_corner.y = 0;
    top_right_corner.h = 1;
    top_right_corner.w = 1;

    bottom_left_corner.x = 0;
    bottom_left_corner.y = app->camera.h;
    bottom_left_corner.h = 1;
    bottom_left_corner.w = 1;

    bottom_right_corner.x = app->camera.w;
    bottom_right_corner.y = app->camera.h;
    bottom_right_corner.h = 1;
    bottom_right_corner.w = 1;

    top_left_corner = mouseMap(app, &top_left_corner);
    top_right_corner = mouseMap(app, &top_right_corner);
    bottom_left_corner = mouseMap(app, &bottom_left_corner);
    bottom_right_corner = mouseMap(app, &bottom_right_corner);

    x_min = (top_left_corner.x - 1 < 0) ? 0 : (top_left_corner.x - 1 < 0);
    x_max = (bottom_right_corner.x + 1 > md->w) ? md->w : bottom_right_corner.x + 1;
    y_min = (top_right_corner.y - 1 < 0) ? 0 : top_right_corner.y - 1;
    y_max = (bottom_left_corner.y + 1 > md->h) ? md->h : bottom_left_corner.y + 1;

    SDL_FillRect(app->screen, NULL, SDL_MapRGB(app->screen->format, 0, 0, 0));

    for(i=y_min; i < y_max; i++)
    {
        for(j=x_min; j < x_max; j++)
        {
            blitTile(app, m->tlst[TLST_GROUND], md->ground_layer[i][j], j, i);
        }
    }
}

static void blitPerso(App *app, Perso *p, GameMap *m)
{
    if(p)
    {
        SDL_Rect r=p->perso_image;

        r.x-=p->perso_image.w/2;
        r.y-=p->perso_image.h-5;

        r.x -= app->camera.x;
        r.y -= app->camera.y;

        if(r.x >= -100 && r.x <= (app->camera.w + 100)
                && r.y >= -100 && r.y <= (app->camera.h + 100))
        {
            SDL_Rect p_clip = m->tlst[TLST_PERSO]->tiles[p->type].clip;
            SDL_BlitSurface(m->tlst[TLST_PERSO]->t, &p_clip, app->screen, &(r));
        }
    }
}

static void blitMask(App *app, Perso *p, GameMap *m)
{
    if(p)
    {
        SDL_Rect r=p->perso_image;
        SDL_Rect mask_clip;
        r.x-=p->perso_image.w/2;
        r.y-=p->perso_image.h-5;

        r.x -= app->camera.x;
        r.y -= app->camera.y;


        if(r.x >= -100 && r.x <= (app->camera.w + 100)
                && r.y >= -100 && r.y <= (app->camera.h + 100))
        {

            mask_clip = m->tlst[TLST_BLUEMASK]->tiles[0].clip;
            if (p->team == 0)
            {
                SDL_BlitSurface(m->tlst[TLST_BLUEMASK]->t, &mask_clip, app->screen, &r);
            }
            else
            {
                SDL_BlitSurface(m->tlst[TLST_REDMASK]->t, &mask_clip, app->screen, &r);
            }
        }
    }
}


static void blitBuilding(App *app, Building *b, GameMap *m)
{
    SDL_Rect r = b->building_r;

    r.x -= app->camera.x;
    r.y -= app->camera.y;

    if(r.x >= -100 && r.x <= (app->camera.w + 100)
            && r.y >= -100 && r.y <= (app->camera.h + 100))
    {

        SDL_Rect b_clip = m->tlst[TLST_BUILDING]->tiles[b->type].clip;

        r.y -= b_clip.h * (2.0/3.0);

        SDL_BlitSurface(m->tlst[TLST_BUILDING]->t, &b_clip, app->screen, &r);
    }
}

static void blitRessource(App *app, Resource *rsc, GameMap *m)
{
    SDL_Rect r = rsc->r;

    r.x -= app->camera.x;
    r.y -= app->camera.y;

    if(r.x >= -100 && r.x <= (app->camera.w + 100)
            && r.y >= -100 && r.y <= (app->camera.h + 100))
    {
        SDL_Rect r_clip = m->tlst[TLST_RESOURCE]->tiles[rsc->type].clip;
        SDL_BlitSurface(m->tlst[TLST_RESOURCE]->t, &r_clip, app->screen, &r);
    }
}

static void blitAnimal(App *app, Animals *a, GameMap *m)
{
    SDL_Rect r = a->r;

    r.x -= app->camera.x;
    r.y -= app->camera.y;

    if(r.x >= -100 && r.x <= (app->camera.w + 100)
            && r.y >= -100 && r.y <= (app->camera.h + 100))
    {
        SDL_Rect r_clip = m->tlst[TLST_ANIMAL]->tiles[a->type].clip;

        SDL_BlitSurface(m->tlst[TLST_ANIMAL]->t, &r_clip, app->screen, &r);
    }

}

void blitSavage(App *app, List *lp, List *lb, List *lr,List * la,  GameMap *m)
{
    Element *current = lr->first;
    Perso *p = NULL;
    Building *b = NULL;
    Resource *r=NULL;
    Animals *a = NULL;

    do
    {
        if(current)
        {
            r=current->pointer;
            if (r)
                blitRessource(app,r, m);
            current=current->next;
        }
    }
    while(current);


    current=lb->first;
    do
    {
        if(current)
        {
            b=current->pointer;
            if(b)
                blitBuilding(app,b, m);
            current=current->next;
        }
    }
    while(current);

    current = la->first;
    do
    {
        if(current)
        {
            a= current->pointer;
            if(a)
                blitAnimal(app, a, m);
            current = current->next;
        }
    }
    while(current);


    current=lp->first;
    do
    {
        if(current)
        {
            p=current->pointer;
            if(p && m)
            {
                blitMask(app, p, m);
                blitPerso(app, p, m);
            }

            current=current->next;
        }
    }
    while(current);
}

void blitSecondLayer(App *app, List *p, List *b, GameMap *m)
{
    Element *cur_p = p->first;
    Element *cur_b = b->first;

    Perso *per = NULL;
    Building *build = NULL;

    do
    {
        if(cur_p)
            per = (Perso *) cur_p->pointer;
        else
            per = NULL;
        if(cur_b)
            build = (Building *) cur_b->pointer;
        else
            build = NULL;

        if(build == NULL)
        {
            blitPerso(app, per, m);
            cur_p = cur_p->next;
            continue;
        }
        if(per == NULL)
        {
            blitBuilding(app, build, m);
            cur_b = cur_b->next;
            continue;
        }

        if(build->dist_to_cam >= per->dist_to_cam)
        {
            blitBuilding(app, build, m);
            cur_b = cur_b->next;
        }
        else
        {
            blitPerso(app, per, m);
            cur_p = cur_p->next;
        }

    }
    while(cur_p || cur_b);
}

void updateScreen(SDL_Surface *screen)
{
    SDL_Flip(screen);
}


void drawRectangleSelection (App *app, SDL_Rect r)
{
    SDL_Rect rectTOP, rectBottom, rectLeft, rectRight;
    SDL_Surface *s =NULL;
    rectTOP.x=r.x - app->camera.x;
    rectTOP.y=r.y - app->camera.y;
    rectTOP.h=1;
    rectTOP.w=r.w;

    rectBottom=rectTOP;
    rectBottom.y=r.h-1;

    rectLeft.x=r.x - app->camera.x;
    rectLeft.y=r.y - app->camera.y;
    rectLeft.h=r.h;
    rectLeft.w=1;

    rectRight=rectLeft;
    rectRight.x=r.w-1;

    s = SDL_CreateRGBSurface(0, abs(r.w), abs(r.h), 32, 0,0,0, 255);
    SDL_FillRect(s, &rectTOP, SDL_MapRGB(s->format, 255, 255, 255));
    SDL_FillRect(s, &rectBottom, SDL_MapRGB(s->format, 255, 255, 255));
    SDL_FillRect(s, &rectLeft, SDL_MapRGB(s->format, 255, 255, 255));
    SDL_FillRect(s, &rectRight, SDL_MapRGB(s->format, 255, 255, 255));

    SDL_BlitSurface(s, &rectBottom, app->screen, &(rectBottom));
    SDL_BlitSurface(s, &rectTOP, app->screen, &(rectTOP));
    SDL_BlitSurface(s, &rectLeft, app->screen, &(rectLeft));
    SDL_BlitSurface(s, &rectRight, app->screen, &(rectRight));
    SDL_Flip(app->screen);
    SDL_FreeSurface(s);
}
