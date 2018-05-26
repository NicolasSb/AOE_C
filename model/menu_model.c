#include "menu.h"
#include "../view/view.h"


Menu *menu_initialization(App *a)
{
    Menu *menu = calloc(1, sizeof(Menu));

    /* SDL Variables initialization*/
    menu->screen = NULL;
    menu->background = NULL;
    menu->select_menu = NULL;

    /* Position of the selections options*/
    menu->x_select_menu = LENGTH_SCREEN -(LENGTH_SCREEN*0.27);
    menu->center = HEIGHT_SCREEN/2-(HEIGHT_SELECT_MENU/2);
    menu->y_select_menu[0] = menu->center-40;
    menu->y_select_menu[1] = menu->center;
    menu->y_select_menu[2] = menu->center+40;

    /* Background and transition displaying (Errors need to be handled)*/
    menu->screen = a->screen;
    menu->background = loadImage("media/background.bmp");
    menu->select_menu = loadImage("media/select_menu.bmp");

    return menu;
}

void freeMenu(Menu* menu)
{
    if (menu)
    {
        if(menu->background)
            SDL_FreeSurface(menu->background);
        if(menu->select_menu)
            SDL_FreeSurface(menu->select_menu);
        if(menu->screen)
            SDL_FreeSurface(menu->screen);
        free(menu);
    }
}


