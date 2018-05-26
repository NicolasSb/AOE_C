#include <SDL/SDL_ttf.h>
#include "../model/interaction.h"
#include "../model/game_utils.h"


void init_ttf(Banner *b)
{
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Initialization error from  SDL TTF_Init : %s\n", TTF_GetError());
    }

    b->police = TTF_OpenFont("media/CloisterBlack.ttf", TEXT_HEIGHT);
    if (b->police == NULL)
    {
        fprintf(stderr, "Police can not be charged : %s\n", SDL_GetError());
    }

}



Banner *banner_blit(Banner *banner, App *a, Player *pl)
{
    SDL_Rect tmpRect;
    tmpRect.x = 0;
    tmpRect.y = 0;
    tmpRect.w = 0;
    tmpRect.h = 0;

    SDL_SetAlpha(banner->select_button, SDL_SRCALPHA, 80);
    SDL_SetAlpha(banner->select_option, SDL_SRCALPHA, 80);

    banner->position.x = (a->camera.w- banner->background->w)/2;
    banner->position.y =  a->camera.h- banner->background->h;
    SDL_BlitSurface(banner->background, NULL, a->screen, &(banner->position));

    if(banner->button_selected>=4)
        banner->button_selected=0;

    tmpRect.w = LENGTH_SELECT_BUTTON;
    tmpRect.h = HEIGHT_SELECT_BUTTON;
    tmpRect.x = banner->x_select_button;
    tmpRect.y = banner->y_select_button[banner->button_selected];
    SDL_BlitSurface(banner->select_button, NULL, a->screen, &(tmpRect));

    if(banner->option_selected>=6)
        banner->option_selected=0;

    tmpRect.w = LENGTH_SELECT_OPTION;
    tmpRect.h = HEIGHT_SELECT_OPTION;
    tmpRect.x = banner->x_select_option[banner->option_selected];
    tmpRect.y = banner->y_select_option;
    SDL_BlitSurface(banner->select_option, NULL, a->screen, &(tmpRect));

    display_game_Data(banner, a, pl);

    return banner;
}


void display_game_Data(Banner *b,App *a,Player *pl)
{
    int i, tmpInt;
    char tmp[50];
    SDL_Color whiteColor= {255, 255, 255, 0};
    SDL_Rect tmpRect;
    SDL_Surface *txt = NULL;
    Resource *r = NULL;
    Building *bu = NULL;
    Animals *an = NULL;
    Perso *p = NULL;

    sprintf(tmp, "%d / %d ", pl->nbCharacters, MAX_PERSO + 4*pl->nbHouses);
    txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
    tmpRect.x= b->position.x + OFFSET_DATA_X;
    tmpRect.y = b->position.y + OFFSET_DATA_Y;
    SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
    if(txt)
    {
        SDL_FreeSurface(txt);
        txt = NULL;
    }
    for(i = 0; i<NB_RESOURCE_TYPE; i++)
    {
        sprintf(tmp, "%d", pl->available_resources[i]);
        txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
        tmpRect.y +=  JUMP_DATA_Y;
        SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
        if(txt)
        {
            SDL_FreeSurface(txt);
            txt = NULL;
        }
    }

    switch(b->banner_selected)
    {
    case B_CHARACTER:
        tmpRect.x = b->position.x + OFFSET_SKILLS_X;
        tmpRect.y = b->position.y + OFFSET_SKILLS_Y;
        p = pl->selectP[0];
        if(p)
        {
            sprintf(tmp, "%d", p->skills.hp);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }

            sprintf(tmp, "%d", p->skills.damage);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            tmpRect.x +=  JUMP_SKILLS_X;
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }

            sprintf(tmp, "%d", p->skills.armor);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            tmpRect.x +=  JUMP_SKILLS_X;
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }
        }
        break;
    case B_ANIMAL:
        tmpRect.x = b->position.x + OFFSET_SKILLS_X;
        tmpRect.y = b->position.y + OFFSET_SKILLS_Y;
        an = (Animals*)pl->s.target;
        if(an)
        {
            sprintf(tmp, "%d", an->life_point);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }

            sprintf(tmp, "%d", an->dommage);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            tmpRect.x +=  JUMP_SKILLS_X;
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }
        }
        break;
    case B_BUILDING:
        tmpRect.x = b->position.x + OFFSET_SKILLS_X;
        tmpRect.y = b->position.y + OFFSET_SKILLS_Y;
        bu = (Building *) pl->s.target;
        if (bu)
        {
            sprintf(tmp, "%d", bu->hp);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }
        }
        break;
    case B_RESOURCE:
        r = (Resource *) pl->s.target;
        if(r)
        {
            switch (r->type)
            {
            case STONE:
                tmpInt = STONE_AMOUNT;
                break;
            case FOOD:
                tmpInt = FOOD_AMOUNT;
                break;
            case WOOD:
                tmpInt = WOOD_AMOUNT;
                break;
            case GOLD:
                tmpInt = GOLD_AMOUNT;
                break;
            }
            tmpRect.x = b->position.x + OFFSET_SKILLS_X;
            tmpRect.y = b->position.y + OFFSET_SKILLS_Y;

            sprintf(tmp, "%d / %d", r->amount, tmpInt);
            txt = TTF_RenderText_Solid(b->police,tmp, whiteColor);
            SDL_BlitSurface(txt, NULL, a->screen, &tmpRect);
            if(txt)
            {
                SDL_FreeSurface(txt);
                txt = NULL;
            }
        }
        break;
    default:
        break;
    }
    if(txt)
    {
        SDL_FreeSurface(txt);
    }
}
