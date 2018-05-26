#include "controller_player.h"
#include "../view/view.h"
#include "../view/application.h"
#include "../model/map.h"
#include "../model/game_utils.h"
#include "../model/game.h"
#include "../model/interaction.h"
#include "../view/iso_utils.h"
/**
*
*   \file controller_player.c
*   \brief game controller
*
*   \author Sourbier N.
*
*   \version 1.0
*
*   \date 05/03/2017
*
*   \todo add other functions and possibilities for the next version
*/


static int testValidTile(Game *g, App *a, int x, int y)
{
    SDL_Rect tmp;
    MatrixData *nd = NULL;
    tmp.x = x;
    tmp.y = y;
    tmp.h = tmp.w = 0;

    tmp = mouseMap(a, &tmp);
    nd = (MatrixData*)g->m->world_map->data->nd;
    if (tmp.x<0 || tmp.y<0)
        return 0;
    else if(tmp.x > nd->w || tmp.y > nd->h)
        return 0;

    return 1;
}



void action_listener(Game *g, App *a, Banner *b)
{
    int nbBuilding;
    SDL_Rect select;
    SDL_Event e;
    int isSelecting;
    Element *current;
    int i, mouse_x=10,mouse_y=10, type;
    int banner_mouseX = 0, banner_mouseY = 0;
    void *pointer;
    int actual_time =0, previous_time = 0;
    Player *pl = g->pl[0];
    GameMap *m = g->m;
    SDL_Rect pos_begin;
    pos_begin.x = g->pl[0]->main_buildingX;
    pos_begin.y = g->pl[0]->main_buildingY;
    initResourceList(g, m);
    updateDistToCam(g, g->r);
    updateDistToCam(g, g->b);
    g->r = sortList(g->r);
    g->b = sortList(g->b);
    nbBuilding = g->b->nbElts;
    isSelecting=0;

    if (pl)
    {
        centerCameraOnPoint(a, pos_begin);
        while (a->running)
        {
            actual_time = SDL_GetTicks();
            SDL_PollEvent(&e);
            switch (e.type)
            {
            case SDL_QUIT:
                a->running=0;
                break;
            case SDL_KEYDOWN:
                isSelecting = 0;
                standard_imput(g, pl, a, e);
            case SDL_MOUSEBUTTONDOWN:
                banner_mouseX = e.motion.x;
                banner_mouseY = e.motion.y;
                mouse_x = e.button.x;
                mouse_y = e.button.y;
                switch (e.button.button)
                {
                case SDL_BUTTON_RIGHT:
                    for (i=0; i<MAX_SELECT; i++)
                    {
                        if(pl->selectP[i])
                        {
                            pl->selectP[i]->dest_x = mouse_x + a->camera.x;
                            pl->selectP[i]->dest_y = mouse_y + a->camera.y;
                            if (!(pointer=enemyDetected(g, pl->selectP[i]->dest_x, pl->selectP[i]->dest_y, &type)))
                            {
                                if((pointer=(resourceFound(g,  pl->selectP[i]->dest_x, pl->selectP[i]->dest_y, &type))))
                                {
                                    setActionFlagP(pl->selectP[i], COLLECT);
                                    setTarget(pl->selectP[i], pointer, TARGET_RESSOURCE);
                                }
                                else
                                {
                                    if (testValidTile(g, a, mouse_x, mouse_y))
                                        setActionFlagP(pl->selectP[i], MOVE);
                                }

                            }

                            else
                            {
                                setActionFlagP(pl->selectP[i], ATTACK);
                                if (type == TARGET_PERSO)
                                    setTarget(pl->selectP[i],(Perso* )pointer, type);
                                else if (type == TARGET_BUILDING)
                                    setTarget(pl->selectP[i], (Building*)pointer, type);
                                else if (type == TARGET_ANIMALS)
                                    setTarget(pl->selectP[i], (Animals*)pointer, type);

                            }

                        }
                        else break;
                    }
                    break;
                case SDL_BUTTON_LEFT:
                    if(banner_mouseY < (b->position.y))
                    {
                        isSelecting=1;
                        select.x=mouse_x + a->camera.x;
                        select.y=mouse_y + a->camera.y;
                        select_onePerso(g, select.x, select.y);
                        if (pl->selectP[0]==NULL)
                            select_Something(g, select.x, select.y);
                        setBannerFlag(pl, b);
                        banner_selection(b);
                    }
                    else
                    {
                        banner_control(g, b, banner_mouseX, banner_mouseY, EVENT_CLIC);
                    }
                    break;
                default:
                    break;
                }
            case SDL_MOUSEMOTION:
                banner_mouseX = e.motion.x;
                banner_mouseY = e.motion.y;
                if(banner_mouseY < (b->position.y))
                {
                    if(SDL_BUTTON_LEFT && isSelecting && mouse_x && mouse_y)
                    {
                        select.w=abs(e.motion.x);
                        select.h=abs(e.motion.y);
                        if (mouse_x<=select.w && mouse_y<=select.h)
                            drawRectangleSelection(a, select);
                    }
                }
                else
                {
                    banner_control(g, b, banner_mouseX, banner_mouseY, EVENT_MOVEMENT);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (SDL_BUTTON_LEFT && isSelecting)
                {
                    isSelecting=0;
                    select.w-=mouse_x;
                    select.h-=mouse_y;
                    if (select.h>15 && select.w >15)
                    {
                        select_perso(g, select);
                        setBannerFlag(pl, b);
                        banner_selection(b);
                    }
                }
                break;
            default:
                break;
            }


            if(actual_time-previous_time >30)
            {
                if (g->b->nbElts != nbBuilding)
                {
                    updateDistToCam(g, g->b);

                    nbBuilding=g->b->nbElts;
                }
                /*g->b=sortList(g->b);
                updateDistToCam(g, g->p);
                g->p=sortList(g->p);*/

                blitGroundTiles(a, m);


                current = g->p->first;
                do
                {
                    if (current)
                    {
                        Perso *p=(Perso*)current->pointer;
                        action_perso(g, p);
                        current = current->next;
                    }
                }
                while (current);


                current = g->b->first;
                do
                {
                    if (current)
                    {
                        if(current->pointer)
                            action_Building(g, (Building*)current->pointer);
                        current = current->next;
                    }
                }
                while(current);


                current = g->a->first;
                do
                {
                    if (current != NULL && current->pointer!=NULL)
                    {
                        Animals *ani=(Animals*)current->pointer;
                        action_animal(g, ani);

                        current = current->next;
                    }
                }
                while(current);

                /*Add the resources */
                blitSavage(a, g->p, g->b, g->r, g->a, m);
                banner_blit(b, a, pl);
                previous_time = actual_time;
                updateScreen(a->screen);
            }

            else
            {
                SDL_Delay(30- (actual_time - previous_time));
            }
        }
        a->running =1;
    }
}

void standard_imput(Game *g, Player *pl, App *a, SDL_Event e)
{
    Building *b;
    Perso *p;
    switch(e.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        a->running = 0;
        break;
    case SDLK_UP:
        moveCamera(a, CAM_UP);
        break;
    case SDLK_DOWN:
        moveCamera(a, CAM_DOWN);
        break;
    case SDLK_LEFT:
        moveCamera(a, CAM_LEFT);
        break;
    case SDLK_RIGHT:
        moveCamera(a, CAM_RIGHT);
        break;
    case SDLK_h:
        if(pl->selectP[0])
        {
            b = buildB(pl, HOUSE);
            if (b)
                insert_first(g->b, (void *)b, pl->team_number);
        }
        break;
    case SDLK_b:
        if(pl->selectP[0])
        {
            b = buildB(pl, BARRACKS);
            if (b)
                insert_first(g->b, (void *)b, pl->team_number);
        }
        break;
    case SDLK_c:
        if(pl->selectP[0])
        {
            b = buildB(pl, CITY_HALL);
            if (b)
                insert_first(g->b, (void *)b, pl->team_number);
        }
        break;
    case SDLK_t:
        if(pl->selectP[0])
        {
            b = buildB(pl, TOWER);
            if (b)
                insert_first(g->b, (void *)b, pl->team_number);
        }
        break;
    case SDLK_a:
        if(pl->s.target_type==SELECT_BUILDING)
        {
            b = (Building *)pl->s.target;
            if (b->team == pl->team_number)
            {
                p = createPerso((Building*)pl->s.target, pl,ARCHER);
                if(p)
                    insert_first(g->p, (void *)p, pl->team_number);
            }
        }
        break;
    case SDLK_v:
        if(pl->s.target_type==SELECT_BUILDING)
        {
            b = (Building *)pl->s.target;
            if (b->team == pl->team_number)
            {
                p = createPerso((Building*)pl->s.target, pl, VILLAGER);
                if(p)
                    insert_first(g->p, (void *)p, pl->team_number);
            }
        }
        break;
    case SDLK_s:
        if(pl->s.target_type==SELECT_BUILDING)
        {
            b = (Building *)pl->s.target;
            if (b->team == pl->team_number)
            {
                p = createPerso((Building*)pl->s.target, pl,SWORDMAN);
                if(p)
                    insert_first(g->p, (void *)p, pl->team_number);
            }
        }
        break;
    case SDLK_f:
        toggleFullscreen(a, g);
        break;
    default:
        break;
    }
}


void setBannerFlag(Player *pl, Banner *b)
{
    Perso *p = NULL;
    Animals *a = NULL;
    Building *bu = NULL;
    Resource *r = NULL;
    if (pl)
    {
        if ((p = pl->selectP[0])!=NULL)
        {
            b->banner_selected = B_CHARACTER;
            switch (p->type)
            {
            case ARCHER :
                b->banner_type = B_ARCHER;
                break;
            case SWORDMAN:
                b->banner_type = B_SWORDSMAN;
                break;
            case VILLAGER:
                b->banner_type = B_VILLAGER;
                break;
            default:
                b->banner_type = B_DEFAULT;
                break;
            }
        }
        else if (pl->s.target != NULL)
        {
            switch (pl->s.target_type)
            {
            case SELECT_ANIMALS:
                b->banner_selected = B_ANIMAL;
                a = (Animals*)(pl->s.target);
                switch (a->type)
                {
                case STAG :
                    b->banner_type = B_STAG;
                    break;
                case BOAR:
                    b->banner_type = B_BOAR;
                    break;
                case SHEEP:
                    b->banner_type = B_SHEEP;
                    break;
                case WOLF:
                    b->banner_type = B_WOLF;
                    break;
                default:
                    b->banner_type = B_DEFAULT;
                    break;
                }
                break;
            case SELECT_BUILDING:
                b->banner_selected = B_BUILDING;
                bu = (Building*)(pl->s.target);
                switch (bu->type)
                {
                case CITY_HALL :
                    b->banner_type = B_CITYHALL;
                    break;
                case HOUSE:
                    b->banner_type = B_HOUSE;
                    break;
                case TOWER:
                    b->banner_type = B_TOWER;
                    break;
                case BARRACKS:
                    b->banner_type = B_BARRACK;
                    break;
                default:
                    b->banner_type = B_DEFAULT;
                    break;
                }
                break;
            case SELECT_RESOURCE:
                b->banner_selected = B_RESOURCE;
                r = (Resource*)(pl->s.target);
                switch (r->type)
                {
                case GOLD :
                    b->banner_type = B_GOLD;
                    break;
                case STONE:
                    b->banner_type = B_STONE;
                    break;
                case FOOD:
                    b->banner_type = B_FOOD;
                    break;
                case WOOD:
                    b->banner_type = B_WOOD;
                    break;
                default:
                    b->banner_type = B_DEFAULT;
                    break;
                }
                break;
                break;
            case TARGET_INVALID:
                b->banner_selected = B_BASE;
                break;
            default:
                b->banner_selected = B_BASE;
                break;
            }
        }
        else
        {
            b->banner_selected = B_DEFAULT;
        }
    }
}
