/**
 *  \file application.c
 *  \brief Declarations of App structure and related functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 5th 2017
 */

#include "application.h"
#include "view_constants.h"

App *initApp(int screen_w, int screen_h, char *appName)
{
	App *app = NULL;
	const SDL_VideoInfo *vid = NULL;
	app = (App *) calloc(1, sizeof(App));

    if(!app)
    {
        fprintf(stderr, "Error when allocating app at line %d, in %s\n",
                __LINE__, __FILE__);
        return NULL;
    }

    app->running = 1;
    app->screen = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Initialization error from SDL Video: %s\n", SDL_GetError());
        return NULL;
    }

    vid = SDL_GetVideoInfo();
    app->best_w = vid->current_w;
    app->best_h = vid->current_h;


    if(appName)
        SDL_WM_SetCaption(appName, NULL);
    else /* Default caption */
        SDL_WM_SetCaption("AOEIV", NULL);

    if(screen_w && screen_h)
        app->screen = SDL_SetVideoMode(screen_w, screen_h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    else
        app->screen = SDL_SetVideoMode(app->best_w, app->best_h, 32,
                                       SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);


    if (app->screen == NULL)
    {
        fprintf(stderr, "Impossible to load video mode: %s\n", SDL_GetError());
        return NULL;
    }

    app->camera.w = app->screen->w;
    app->camera.h = app->screen->h;
    
    app->mousemap = loadImage("media/tile_mask.gif");

    return app;
}

void centerCameraOnPoint(App *app, SDL_Rect point)
{
    app->camera.x = point.x - (app->camera.w / 2);
    app->camera.y = point.y - (app->camera.h / 2);
}

void setSpaces(App *app, Game *g)
{
    MatrixData *md = (MatrixData *) g->m->world_map->data->nd;
    int map_size = md->w;

    /* Settings for world area */
    app->world_area.w = map_size * TILE_WIDTH + app->camera.w;
    app->world_area.h = map_size * TILE_HEIGHT + app->camera.h;

    app->world_area.x = -(app->world_area.w / 2);
    app->world_area.y = - (app->camera.h / 2);

    /* Settings for camera area */
    app->cam_area.x = app->world_area.x + STEP_X;
    app->cam_area.y = app->world_area.y;
    app->cam_area.w = app->world_area.w - app->camera.w;
    app->cam_area.h = app->world_area.h - app->camera.h;
}

void closeApp(App* app)
{
    if(app)
    {
    	SDL_FreeSurface(app->mousemap);
        SDL_FreeSurface(app->screen);
        SDL_Quit();
        free(app);
    }
}


int isFullscreen(App* app)
{
    if(app)
    {
        if(app->screen)
        {
            return (app->screen->flags) & SDL_FULLSCREEN;
        }

    }

    return -1;
}

int toggleFullscreen(App* app, Game *g)
{
    SDL_Surface *scr = NULL;
    int w;
    int h;

    Uint32 flags = app->screen->flags;
    flags ^= SDL_FULLSCREEN;

    w = (isFullscreen(app)) ? SCREEN_WIDTH : app->best_w;
    h = (isFullscreen(app)) ? SCREEN_HEIGHT : app->best_h;

    scr = SDL_SetVideoMode(w, h, 32, flags);

    if(!scr)
    {
        fprintf(stderr, "Error when toggling fullscreen\n");
        return 0;
    }
    else {
        SDL_FreeSurface(app->screen);
        app->screen = scr;

        app->camera.w = scr->w;
        app->camera.h = scr->h;

        setSpaces(app, g);

        return 1;
    }
    
    
}
