/**
 *  \file application.h
 *  \brief Definitions of App structure and related functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 5th 2017
 */

#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <SDL.h>
#include "../model/game.h"

/**
 *  \struct App
 *  \brief This structure facilitates the call to view functions from the controller
*/
typedef struct App App;
struct App
{
    SDL_Surface *screen;    /*!< Screen used for displaying */
    SDL_Rect    camera;     /*!< Area viewed by the camera */
    SDL_Rect    cam_area;   /*!< Area in which the camera can move */
    SDL_Rect    world_area; /*!< Area representing the world */
    int running;            /*!< Boolean set to 0 when we want to end the program */

    int best_w;
    int best_h;
    
    SDL_Surface *mousemap;
};

void centerCameraOnPoint(App *app, SDL_Rect point);

/**
 *  \fn App *initApp(int screen_w, int screen_h, char *appName)
 *  \brief Inits an App structure and loads SDL
 *  \param screen_w the width of the screen
 *  \param screen_h the height of the screen
 *  \param appName the name used for the window caption (can be set to NULL for default)
 *  \return a pointer on the App structure
*/
App *initApp(int screen_w, int screen_h, char *appName);

/**
 *	\fn void setSpaces(App *app, Game *g)
 *	\brief Sets areas and camera after the game is loaded
 *	\param app the main application
 *	\param g the loaded game
 */
void setSpaces(App *app, Game *g);

/**
 *  \fn void closeApp(App* app)
 *  \brief Frees an app structure and quit the SDL
 *  \param app the app to free
*/
void closeApp(App* app);

int isFullscreen(App* app);

int toggleFullscreen(App* app, Game *g);

#endif
