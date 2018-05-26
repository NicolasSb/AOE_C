#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_ttf.h>
#include <time.h>
#include "controller/controller_player.h"

#include "view/view_constants.h"
#include "view/view.h"
#include "view/application.h"
#include "model/map.h"
#include "model/game.h"
#include "model/files.h"
#include "model/interaction.h"
#include "model/menu.h"

/*#define TEST_MODE*/

#ifndef TEST_MODE


int main(void)
{
    Menu *menu;
    int cont = 1;
    App *app = initApp(0, 0, NULL);
    Game *g = NULL;
    Banner *b = banner_initialization(app);
    init_ttf(b);
    menu = menu_initialization(app);

    srand(time(NULL));

    menu->choice = 0;

    while (cont)
    {
        menu_blit(menu);

        SDL_WaitEvent(&menu->event);

        switch(menu->event.type)
        {
        /* Keyboard selection management*/
        case SDL_QUIT:
            cont = 0;

            break;
        case SDL_KEYDOWN:
            switch(menu->event.key.keysym.sym)
            {
            case SDLK_UP:
                menu->choice = (menu->choice + 2) % 3;
                break;
            case SDLK_DOWN:
                menu->choice = (menu->choice + 1) % 3;
                break;
            case SDLK_ESCAPE:
                cont = 0;

                break;
            case SDLK_RETURN:
                switch(menu->choice)
                {
                case NEW_GAME:
                    g= initGame(2);
                    setSpaces(app, g);
                    action_listener(g, app, b);
                    saveGameBinary(g);
                    freeGame(g);
                    break;

                case LOAD:
                    if (fileExists("game.save"))
                    {
                        g = loadGameFromFileBinary();
                    }
                    else
                        g= initGame(2);

                    setSpaces(app, g);
                    action_listener(g, app, b);
                    saveGameBinary(g);
                    freeGame(g);
                    break;

                case QUIT:
                    cont = 0;

                    break;

                default:
                    break;
                }
                break;

            default:
                return -1;
                break;
            }
            break;

        /* Mouse selection management*/
        case SDL_MOUSEBUTTONUP:
            if (menu->event.button.button == SDL_BUTTON_LEFT && menu->event.button.x > menu->x_select_menu && menu->event.button.x < menu->x_select_menu + LENGTH_SELECT_MENU)
            {
                if(menu->event.button.y > menu->y_select_menu[NEW_GAME] && menu->event.button.y < menu->y_select_menu[LOAD])
                {
                    g= initGame(2);
                    setSpaces(app, g);
                    action_listener(g, app, b);
                    saveGameBinary(g);
                    freeGame(g);
                }
                else if(menu->event.button.y > menu->y_select_menu[LOAD] && menu->event.button.y < menu->y_select_menu[QUIT])
                {
                    if (fileExists("game.save"))
                    {
                        g = loadGameFromFileBinary();
                    }
                    else
                        g= initGame(2);
                    setSpaces(app, g);
                    action_listener(g, app, b);
                    saveGameBinary(g);
                    freeGame(g);
                }
                else if(menu->event.button.y > menu->y_select_menu[QUIT] && menu->event.button.y < menu->y_select_menu[QUIT]+HEIGHT_SELECT_MENU)
                {
                    cont = 0;
                }
            }
            break;

        case SDL_MOUSEMOTION:
            if(menu->event.motion.x > menu->x_select_menu && menu->event.motion.x < menu->x_select_menu + LENGTH_SELECT_MENU)
            {
                if(menu->event.motion.y > menu->y_select_menu[NEW_GAME] && menu->event.motion.y < menu->y_select_menu[LOAD]) menu->choice = NEW_GAME;
                else if(menu->event.motion.y > menu->y_select_menu[LOAD] && menu->event.motion.y < menu->y_select_menu[QUIT]) menu->choice = LOAD;
                else if(menu->event.motion.y > menu->y_select_menu[QUIT] && menu->event.motion.y < menu->y_select_menu[QUIT]+HEIGHT_SELECT_MENU) menu->choice = QUIT;
            }
            break;
        }
        SDL_Flip(menu->screen);
    }
    freeBanner(b);
    TTF_Quit();
    freeMenu(menu);
    closeApp(app);
    return 0;
}



#endif

#ifdef TEST_MODE

#include "tests/model/tests.h"

int main()
{
    runTests();

    return 0;
}

#endif
