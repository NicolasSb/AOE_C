#include "interaction.h"

Banner *banner_initialization(App *a)
{

    Banner *b = calloc (1,sizeof(Banner));
    int i = 0;

    b->police = NULL;

    b->button_selected = 0;
    b->option_selected = 0;
    b->banner_type = B_DEFAULT;
    b->banner_selected = B_BASE;

    b->select_button = loadImage("media/select_button.bmp");
    b->select_option = loadImage("media/select_option.bmp");



    b = banner_selection(b);

    b->position.x = (a->camera.w - b->background->w)/2;
    b->position.y = a->camera.h - b->background->h;

    b->y_select_option = b->position.y + OPTION_POSITION_Y;
    for (i=0; i< NUMBER_OF_OPTIONS; i++)
    {
        b->x_select_option[i] =b->position.x + OFFSET_X_OPTION + 8*i + LENGTH_SELECT_OPTION * i;
    }
    b->x_select_button = b->position.x + BUTTON_POSITION_X;
    for (i=0; i< NUMBER_OF_BUTTONS; i++)
    {
        b->y_select_button[i] = b->position.y + OFFSET_Y_BUTTON + 22*i + HEIGHT_SELECT_BUTTON * i;
    }
    return b;
}


void freeBanner(Banner *b)
{
    if(b)
    {
        if(b->background)
            SDL_FreeSurface(b->background);
        if(b->select_button)
            SDL_FreeSurface(b->select_button);
        if(b->select_option)
            SDL_FreeSurface(b->select_option);
        if(b->police)
            TTF_CloseFont(b->police);
        free(b);
    }
}

Banner * banner_selection(Banner *banner)
{
    if(banner->background)
        SDL_FreeSurface(banner->background);

    banner->background = NULL;

    switch(banner->banner_type)
    {
    case B_ARCHER :
        banner->background = loadImage("media/archer.bmp");
        banner->banner_selected = B_CHARACTER;
        break;
    case B_SWORDSMAN :
        banner->background = loadImage("media/swordsman.bmp");
        banner->banner_selected = B_CHARACTER;
        break;
    case B_VILLAGER :
        banner->background = loadImage("media/villager.bmp");
        banner->banner_selected = B_CHARACTER;
        break;
    case B_CITYHALL :
        banner->background = loadImage("media/cityhall.bmp");
        banner->banner_selected = B_BUILDING;
        break;
    case B_HOUSE :
        banner->background = loadImage("media/house.bmp");
        banner->banner_selected = B_BUILDING;
        break;
    case B_TOWER :
        banner->background = loadImage("media/towers.bmp");
        banner->banner_selected = B_BUILDING;
        break;
    case B_BARRACK :
        banner->background = loadImage("media/barracks.bmp");
        banner->banner_selected = B_BUILDING;
        break;
    case B_SHEEP :
        banner->background = loadImage("media/sheep.bmp");
        banner->banner_selected = B_ANIMAL;
        break;
    case B_STAG :
        banner->background = loadImage("media/stag.bmp");
        banner->banner_selected = B_ANIMAL;
        break;
    case B_BOAR :
        banner->background = loadImage("media/boar.bmp");
        banner->banner_selected = B_ANIMAL;
        break;
    case B_WOLF :
        banner->background = loadImage("media/wolf.bmp");
        banner->banner_selected = B_ANIMAL;
        break;
    case B_STONE :
        banner->background = loadImage("media/stone.bmp");
        banner->banner_selected = B_RESOURCE;
        break;
    case B_GOLD :
        banner->background = loadImage("media/gold.bmp");
        banner->banner_selected = B_RESOURCE;
        break;
    case B_WOOD :
        banner->background = loadImage("media/wood.bmp");
        banner->banner_selected = B_RESOURCE;
        break;
    case B_FOOD:
        banner->background = loadImage("media/food.bmp");
        banner->banner_selected = B_RESOURCE;
        break;
    case B_DEFAULT:
        banner->background = loadImage("media/base.bmp");
        banner->banner_selected = B_BASE;
        break;
    default :
        banner->background = loadImage("media/base.bmp");
        banner->banner_selected = B_BASE;
        break;
    }

    return banner;
}


int buttonHovered(Banner *b, int y)
{
    int i;
    static int nbCall;
    if (nbCall%30== 1)
    {
        nbCall = 0;
        for (i=0; i<NUMBER_OF_BUTTONS-1; i++)
        {
            if (b->y_select_button[i+1]>y)
            {
                return i;
            }
        }
        if(b->y_select_button[i] < y)
        {
            return NUMBER_OF_BUTTONS-1;
        }
        return 0;
    }
    ++nbCall;
    return b->button_selected;
}

int optionHovered(Banner *b, int x)
{
    int i;
    static int nbCall;
    if(nbCall%30 == 1)
    {
        nbCall = 0;
        for (i=0; i<NUMBER_OF_OPTIONS-1; i++)
        {
            if (b->x_select_option[i+1]>x)
            {
                return i;
            }
        }
        if(b->x_select_option[i] < x)
        {
            return NUMBER_OF_OPTIONS-1;
        }
        return 0;
    }
    ++nbCall;
    return b->option_selected;
}


void action_on_banner(Game *g, int type, int nbButton)
{
    static int nbCall=0;
    Perso *p = NULL;
    Building *b = NULL;
    Player *pl = NULL;
    if (g)
    {
        nbCall ++;
        pl = g->pl[0];
        switch (type)
        {
        case B_VILLAGER:
            p = g->pl[0]->selectP[0];
            if (p->action_flag == SLEEP || p->action_flag == MOVE)
            {
                switch (nbButton)
                {
                case 0:
                    b = buildB(pl, CITY_HALL);
                    if (b)
                        insert_first(g->b, (void *)b, pl->team_number);
                    break;
                case 1:
                    b = buildB(pl, HOUSE);
                    if (b)
                        insert_first(g->b, (void *)b, pl->team_number);
                    break;
                case 2:
                    b = buildB(pl, BARRACKS);
                    if (b)
                        insert_first(g->b, (void *)b, pl->team_number);
                    break;
                case 3 :
                    b = buildB(pl, TOWER);
                    if (b)
                        insert_first(g->b, (void *)b, pl->team_number);
                    break;
                default:
                    break;
                }
            }
            break;
        case B_BARRACK:
            switch (nbButton)
            {
            case 0:
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
            case 1:
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
            default:
                break;
            }
            break;
        case B_HOUSE:
            switch (nbButton)
            {
            case 0:
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
            default:
                break;
            }
            break;
        case B_CITYHALL:
            switch (nbButton)
            {
            case 0:
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
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

