/**
 *  \file testMapChunk.c
 *  \brief Unit tests for MapChunk
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 29th 2017
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../model/map_chunk.h"
#include "tests.h"


/*
***********************************************
Tests for initMatrixData function
***********************************************
*/
static void test_initMatrixData(void **state)
{
    MatrixData *md = initMatrixData();

    assert_non_null(md);
    if(md)
    {
        assert_int_equal(md->w, 0);
        assert_int_equal(md->h, 0);
        assert_null(md->ground_layer);
        assert_null(md->collision_map);
    }

    *state = md;
}

static int teardownMatrixData(void **state)
{
    MatrixData *md = (MatrixData *) state[0];
    freeMatrixData(md);

    return 0;
}

static int testInitMatrixData(void)
{
    const struct CMUnitTest tests_initMatrixData[] = {
        cmocka_unit_test_teardown(test_initMatrixData,
                                  teardownMatrixData),
    };

    return cmocka_run_group_tests_name("Test initMatrixData function",
                                       tests_initMatrixData, NULL, NULL);
}

/*
***********************************************
Tests for initEmptyMatrixDataWithParam function
***********************************************
*/

/**
 *  \todo Rewrite setup and possibly the beginning of the test so that Valgrind
            does not complain
 */

static void test_initEmptyMatrixDataWithParams_HandlesGoodArgs(void **state)
{
    int w =5, h = 5;

    MatrixData *md = initEmptyMatrixDataWithParams(w, h);

    assert_non_null(md);
    if(md)
    {
        assert_int_equal(md->w, w);
        assert_int_equal(md->h, h);
        assert_non_null(md->ground_layer);
        assert_non_null(md->collision_map);
    }

    *state = md;
}

static void test_initEmptyMatrixDataWithParams_HandlesBadArgs(void **state)
{
    int w = -1;
    int h = 0;

    MatrixData *md = initEmptyMatrixDataWithParams(w, h);

    assert_null(md);

    *state = md;
}

static int teardownMatrixData_Params(void **state)
{
    MatrixData *md = (MatrixData *) state[0];
    freeMatrixData(md);

    return 0;
}

static int testInitEmptyMatrixDataWithParams(void)
{
    const struct CMUnitTest tests_initEmptyMatrixDataWithParams[] = {
        cmocka_unit_test_teardown(test_initEmptyMatrixDataWithParams_HandlesGoodArgs,
                                        teardownMatrixData_Params),
        cmocka_unit_test_teardown(test_initEmptyMatrixDataWithParams_HandlesBadArgs,
                                        teardownMatrixData_Params),
    };

    return cmocka_run_group_tests_name("Test initEmptyMatrixDataWithParams function",
                                       tests_initEmptyMatrixDataWithParams,
                                       NULL, NULL);
}

/*
***********************************************
Tests for initMapChunkWithParams function
***********************************************
*/
static void initMapChunkWithParams_HandlesRoot(void **state)
{
    int w = 5;
    int h = 5;
    int depth = 0;

    MapChunk *m = initMapChunkWithParams(w, h, depth);

    assert_non_null(m);
    if(m)
    {
        assert_non_null(m->nd);

        if(m->nd)
        {
            MatrixData *md = (MatrixData *) m->nd;
            assert_int_equal(md->w, w);
            assert_int_equal(md->h, h);
            assert_non_null(md->ground_layer);
            assert_non_null(md->collision_map);
        }
    }

    state[0]=m;
    state[1]=&depth;
}

static void initMapChunkWithParams_HandlesChild(void **state)
{
    int w = 5;
    int h = 5;
    int depth = 1;

    MapChunk *m = initMapChunkWithParams(w, h, depth);

    assert_non_null(m);
    if(m)
        assert_non_null(m->nd);

    state[0]=m;
    state[1]=&depth;
}

static void initMapChunkWithParams_HandlesBadArgs(void **state)
{
    int depth = 0;

    MapChunk *m = initMapChunkWithParams(-1, 0, depth);

    assert_null(m);

    state[0] = m;
}

static int teardownMapChunk_WithParams_Root(void **state)
{
    MapChunk *m = (MapChunk *) state[0];

    freeMapChunk(m, 0);

    return 0;
}

static int teardownMapChunk_WithParams_Child(void **state)
{
    MapChunk *m = (MapChunk *) state[0];

    freeMapChunk(m, 1);

    return 0;
}

static int testInitMapChunkWithParams(void)
{
    const struct CMUnitTest tests_initMapChunkWithParams[] = {
        cmocka_unit_test_teardown(initMapChunkWithParams_HandlesRoot,
                                        teardownMapChunk_WithParams_Root),
        cmocka_unit_test_teardown(initMapChunkWithParams_HandlesChild,
                                        teardownMapChunk_WithParams_Child),
        cmocka_unit_test_teardown(initMapChunkWithParams_HandlesBadArgs,
                                        teardownMapChunk_WithParams_Root),
    };

    return cmocka_run_group_tests_name("Test initMapChunkWithParams function",
                                       tests_initMapChunkWithParams,
                                       NULL, NULL);
}


/*
***********************************************
Main test function launching all the tests
***********************************************
*/
int testMapChunk(void)
{
    int val = 0;

    val += testInitMatrixData();
    val += testInitEmptyMatrixDataWithParams();
    val += testInitMapChunkWithParams();

    return val;
}
