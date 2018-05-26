#include "../model/menu.h"

Menu *menu_blit(Menu *menu)
{
    SDL_Rect tmpR;
    const SDL_VideoInfo *vid;
    SDL_SetAlpha(menu->select_menu, SDL_SRCALPHA, 80); /* Transition's opacity*/

    SDL_WM_SetCaption("AOE - Chicken Warfare", NULL); /* Name of the Window*/

    vid = SDL_GetVideoInfo();
    tmpR.w = vid->current_w;
    tmpR.h = vid->current_h;
    tmpR.x = tmpR.y = 0;


    SDL_FillRect(menu->screen,NULL, SDL_MapRGB(menu->screen->format, 255,255,255));
    SDL_BlitSurface(menu->screen, NULL, menu->screen, &tmpR);

    menu->position.x = 0;
    menu->position.y = 0;
    SDL_BlitSurface(menu->background, NULL, menu->screen, &(menu->position));

    menu->position.x = menu->x_select_menu;
    menu->position.y = menu->y_select_menu[menu->choice];
    SDL_BlitSurface(menu->select_menu, NULL, menu->screen, &(menu->position));

    return menu;
}

