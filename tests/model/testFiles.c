/**
 * \file testFiles.c
 * \brief Unit tests for files.h functions
 * \author Léo Gauvin
 * \version 0.1
 * \date March, 2th 2017
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../model/files.h"
#include "tests.h"


/*
***********************************************
Tests for openFile
***********************************************
*/
static int setup_openFile_Reading(void **state)
{
    char *filename = "tests/data/test";
    char *mode = "r";

    state[0] = filename;
    state[1] = mode;

    return 0;
}

static int setup_openFile_Writing(void **state)
{
    char *filename = "tests/data/test";
    char *mode = "w";

    state[0] = filename;
    state[1] = mode;

    return 0;
}

static int setup_openFile_NonExistingFile(void **state)
{
    char *filename = "test/data/wrong";
    char *mode = "r";

    state[0] = filename;
    state[1] = mode;

    return 0;
}

static void test_openFile_Handles_Mode(void **state)
{
    char *filename = (char *) (state[0]);
    char *mode = (char *) (state[1]);


    assert_non_null(openFile(filename, mode));
}

static void test_openFile_Handles_NonExistingFile(void **state)
{
    char *filename = (char *) (state[0]);
    char *mode = (char *) (state[1]);


    assert_null(openFile(filename, mode));
}


static int testOpenFile(void)
{
    const struct CMUnitTest tests_OpenFile[] = {
        cmocka_unit_test_setup(test_openFile_Handles_Mode, setup_openFile_Reading),
        cmocka_unit_test_setup(test_openFile_Handles_Mode, setup_openFile_Writing),
        cmocka_unit_test_setup(test_openFile_Handles_NonExistingFile,
                                setup_openFile_NonExistingFile),
    };

    return cmocka_run_group_tests_name("Test openFile function", tests_OpenFile, NULL, NULL);
}

/*
***********************************************
Tests for loadImage
***********************************************
*/
static int setup_loadImage(void **state)
{
    char *filename = "tests/data/tileset.png";
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

static int setup_loadImage_NonExistingFile(void **state)
{
    char *filename = "tests/data/bidon.png";
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

static void test_loadImage_Handles_ExistingFile(void ** state)
{
    char *filename = (char *) (state[0]);
    ViewElements *v = (ViewElements *) (state[1]);

    v->image = loadImage(filename);

    SDL_BlitSurface(v->image, NULL, v->screen, NULL);
    SDL_Flip(v->screen);

    assert_non_null(v->image);

    freeViewElements(v);
}

static void test_loadImage_Handles_NonExistingFile(void ** state)
{
    char *filename = (char *) (state[0]);
    ViewElements *v = (ViewElements *) (state[1]);

    v->image = loadImage(filename);

    assert_null(loadImage(filename));

    freeViewElements(v);
}

static int testLoadImage(void)
{
    const struct CMUnitTest tests_files[] = {
        cmocka_unit_test_setup(test_loadImage_Handles_ExistingFile,
                                        setup_loadImage),
        cmocka_unit_test_setup(test_loadImage_Handles_NonExistingFile,
                                        setup_loadImage_NonExistingFile),
    };

    return cmocka_run_group_tests_name("Test loadImage function", tests_files, NULL, NULL);
}

/*
***********************************************
Main test function launching all the tests
***********************************************
*/
int testFiles(void)
{
    int val = 0;

    val += testOpenFile();
    val += testLoadImage();

    return val;
}

