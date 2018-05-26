/**
 * \file TestMap.c
 * \brief Unit tests for map.h functions
 * \author Léo Gauvin
 * \version 0.1
 * \date March, 4th 2017
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../model/map.h"
#include "tests.h"


/*
***********************************************
Tests for initEmptyMap function
***********************************************
*/
static void test_initEmptyMap()
{
    GameMap *m = initEmptyMap();

    assert_non_null(m);
    assert_null(m->map_name);
    assert_int_equal(m->nb_tilesets, 0);
    assert_null(m->tlst_filename);
    assert_null(m->tlst);
    assert_null(m->tlst_offset);
    assert_null(m->world_map);

    freeMap(m);
}

static int testInitEmptyMap(void)
{
    const struct CMUnitTest tests_InitEmptyMap[] = {
        cmocka_unit_test(test_initEmptyMap),
    };

    return cmocka_run_group_tests_name("Test initEmptyMap function", tests_InitEmptyMap, NULL, NULL);
}

/*
***********************************************
Tests for initEmptyMapWithParams function
***********************************************
*/
static int setup_initEmptyMapWithParams(void **state)
{
    int *nb_tlst = (int *) calloc(1, sizeof(int));
    char **tlst_filename = (char **) calloc(*nb_tlst, sizeof(char *));
    char *map_filename = "tests/data/1.map";
    *nb_tlst = 1;
    tlst_filename[0] = "tests/data/ground_tileset.tlst";

    state[0] = map_filename;
    state[1] = nb_tlst;
    state[2] = tlst_filename;

    return 0;
}

static void test_initEmptyMapWithParams(void **state)
{
    int w;
    GameMap *m = NULL;
    char *map_filename = (char *)state[0];
    int *nb_tlst = (int *) state[1];
    char **tlst_filename = (char **)state[2];

    ViewElements *v = initViewElements();

    v->screen = SDL_SetVideoMode(256, 32, 32, SDL_HWSURFACE);

    if(v->screen == NULL)
    {
        fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    w = 8;

    m = initEmptyMapWithParams(map_filename, *nb_tlst, tlst_filename, w);

    assert_non_null(m);
    if(m)
    {
        assert_string_equal(m->map_name, state[0]);
        assert_int_equal(m->nb_tilesets, *nb_tlst);
        assert_non_null(m->tlst);
        assert_non_null(m->tlst_filename);
        if(m->tlst_filename)
        {
            int i = 0;
            for(i = 0; i < *nb_tlst; i++)
            {
                assert_string_equal(m->tlst_filename[i], tlst_filename[i]);
            }
        }
        assert_non_null(m->tlst_offset);

        assert_non_null(m->world_map);
        if(m->world_map)
        {
            MatrixData *md = (MatrixData *) m->world_map->data->nd;

            assert_int_equal(md->w, w);
        }

    }

    /* it seems that Valgrind does not like when I deallocate the map in
        the teardown function or use the freeMap function but only for this test
        so I free everything by hand here. However, freeing m at the end got
        Valgrind say that I had not freed anything at all
    */
    if(m)
        freeMap(m);
    if(tlst_filename)
        free(tlst_filename);
    if(nb_tlst)
        free(nb_tlst);
    if(v)
        freeViewElements(v);

}

static int teardown(void **state)
{
    GameMap *m = (GameMap *) state[0];
    freeMap(m);

    return 0;
}

static int testInitEmptyMapWithParams(void)
{
    const struct CMUnitTest tests_initEmptyMapWithParams[] = {
        cmocka_unit_test_setup(test_initEmptyMapWithParams,
                               setup_initEmptyMapWithParams),
    };

    return cmocka_run_group_tests_name("Test initEmptyMapWithParams function",
                                       tests_initEmptyMapWithParams, NULL, NULL);
}

/*
***********************************************
Tests for loadFromTextFile
***********************************************
*/
static int setup_loadFromFile_Handles_ExistingFile(void **state)
{
    char *filename = "tests/data/copy.map";
    GameMap *m = (GameMap *) calloc(1, sizeof(GameMap));

    state[0] = m;
    state[1] = filename;

    return 0;
}

static int setup_loadFromFile_Handles_NonExistingFile(void **state)
{
    char *filename = "tests/data/bidon";
    GameMap *m = (GameMap *) calloc(1, sizeof(GameMap));

    state[0] = m;
    state[1] = filename;

    return 0;
}

static void test_loadFromTextFile_Handles_ExistingFile(void **state)
{
    GameMap *m = (GameMap *)state[0];
    char *filename = (char *)state[1];

    m = loadFromTextFile(m, filename);

    assert_non_null(m);
    displayMap(m);
}

static void test_loadFromTextFile_Handles_NonExistingFile(void **state)
{
    GameMap *m = (GameMap *)state[0];
    char *filename = (char *)state[1];

    m = loadFromTextFile(m, filename);

    assert_null(m);
}

static int testLoadFromTextFile(void)
{
    const struct CMUnitTest tests_loadFromTextFile[] = {
        cmocka_unit_test_setup_teardown(test_loadFromTextFile_Handles_ExistingFile,
                                        setup_loadFromFile_Handles_ExistingFile,
                                        teardown),
        cmocka_unit_test_setup_teardown(test_loadFromTextFile_Handles_NonExistingFile,
                                        setup_loadFromFile_Handles_NonExistingFile,
                                        teardown),
    };

    return cmocka_run_group_tests_name("Test loadFromTextFile function",
                                       tests_loadFromTextFile, NULL, NULL);
}

/*
***********************************************
Tests for saveToFile
***********************************************
*/

static int setup_saveToFile_Handles_ExistingFile(void **state)
{
    GameMap *m = NULL;
    char *tlst_filename = "tests/data/ground_tileset.tlst";
    int nb_tlst = 1;
    int w = 8;

    char *filename = "tests/data/saved_map.map";

    m = initEmptyMapWithParams(filename, nb_tlst, &tlst_filename, w);

    if(m == NULL)
        return 0;

    state[0] = m;
    state[1] = filename;

    return 0;
}

static int setup_saveToFile_Handles_NonExistingFile(void **state)
{
    GameMap *m = NULL;
    char *tlst_filename = "tests/data/ground_tileset.tlst";
    int nb_tlst = 1;
    int w = 8;

    char *filename = "tests/data/wrong";

    m = initEmptyMapWithParams(filename, nb_tlst, &tlst_filename, w);

    if(m == NULL)
        return 0;

    state[0] = m;
    state[1] = filename;

    return 0;
}

static void test_saveToTextFile_HandlesExistingFile(void **state)
{
    GameMap *m = (GameMap *)state[0];
    char *filename = (char *)state[1];

    assert_int_equal(0, saveToTextFile(m, filename));
}

static void test_saveToTextFile_HandlesNonExistingFile(void **state)
{
    GameMap *m = (GameMap *)state[0];
    char *filename = (char *)state[1];

    assert_int_equal(0, saveToTextFile(m, filename));
}

static int teardownSave(void **state)
{
    GameMap *m = (GameMap *) state[0];

    if(m)
    {
        freeMap(m);
    }

    return 0;
}

static int testSaveToTextFile(void)
{
    const struct CMUnitTest tests_saveToTextFile[] = {
        cmocka_unit_test_setup_teardown(test_saveToTextFile_HandlesExistingFile,
                               setup_saveToFile_Handles_ExistingFile,
                               teardownSave),
        cmocka_unit_test_setup_teardown(test_saveToTextFile_HandlesNonExistingFile,
                               setup_saveToFile_Handles_NonExistingFile,
                               teardownSave),
    };

    return cmocka_run_group_tests_name("Test saveToTextFile function",
                                       tests_saveToTextFile, NULL, NULL);
}

/*
***********************************************
Main test function launching all the tests
***********************************************
*/
int testMap(void)
{
    int val = 0;

    val += testInitEmptyMap();
    val += testInitEmptyMapWithParams();
    val += testLoadFromTextFile();
    val += testSaveToTextFile();

    return val;
}

