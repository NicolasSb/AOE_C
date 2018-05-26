/**
 * \file testTile.c
 * \brief Unit tests for tile.h functions
 * \author Léo Gauvin
 * \version 0.1
 * \date March, 3rd 2017
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../model/tile.h"
#include "tests.h"

/*
***********************************************
Tests for loadTilesetFromFile
***********************************************
*/
static int setup_loadTilesetFromFile(void **state)
{
    char *filename = "tests/data/ground_tileset.tlst";
    ViewElements *v = initViewElements();

    v->screen = SDL_SetVideoMode(256, 32, 32, SDL_HWSURFACE);

    if(v->screen == NULL)
    {
        fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    state[0] = filename;
    state[1] = v;

    return 0;
}

static int setup_loadTilesetFromFile_NonExistingFile(void **state)
{
    char *filename = "tests/data/bidon";
    ViewElements *v = initViewElements();

    v->screen = SDL_SetVideoMode(256, 32, 32, SDL_HWSURFACE);

    if(v->screen == NULL)
    {
        fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    state[0] = filename;
    state[1] = v;

    return 0;
}

static void test_loadTilesetFromFile(void **state)
{
    char *filename = (char *) (state[0]);
    ViewElements *v = (ViewElements *) (state[1]);

    Tileset *tlst = loadTilesetFromFile(filename);

    SDL_BlitSurface(tlst->t, NULL, v->screen, NULL);
    SDL_Flip(v->screen);

    assert_non_null(tlst);

    if(tlst)
    {
        assert_non_null(tlst->t);
        assert_int_equal(tlst->nb_row, 1);
        assert_int_equal(tlst->nb_col, 4);
        assert_int_equal(tlst->tiles[0].collision.x, 32);
        assert_int_equal(tlst->tiles[0].collision.y, 0);
        assert_int_equal(tlst->tiles[0].collision.w, 2);
        assert_int_equal(tlst->tiles[0].collision.h, 2);
    }

    freeTileset(tlst);
    freeViewElements(v);
}

static void test_loadTilesetFromFile_HandlesNonExistingFile(void **state)
{
    char *filename = (char *) (state[0]);
    ViewElements *v = (ViewElements *) (state[1]);

    Tileset *tlst = loadTilesetFromFile(filename);

    assert_null(tlst);

    freeTileset(tlst);
    freeViewElements(v);
}

static int testloadTilesetFromFile(void)
{
    const struct CMUnitTest tests_LoadTilesetFromFile[] = {
        cmocka_unit_test_setup(test_loadTilesetFromFile,
                               setup_loadTilesetFromFile),
        cmocka_unit_test_setup(test_loadTilesetFromFile_HandlesNonExistingFile,
                               setup_loadTilesetFromFile_NonExistingFile),
    };

    return cmocka_run_group_tests_name("Test loadTilesetFromFile function",
                                       tests_LoadTilesetFromFile, NULL, NULL);
}

/*
***********************************************
Main test function launching all the tests
***********************************************
*/
int testTile(void)
{
    int val = 0;
    val += testloadTilesetFromFile();

    return val;
}
