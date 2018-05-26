/**
 *  \file testQTree.c
 *  \brief Unit tests for QTree functions
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date March, 29th 2017
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../../model/qtree.h"
#include "tests.h"


/*
***********************************************
Tests for initQTree function
***********************************************
*/
static int setup_initQTree_Root(void **state)
{
    int *depth = (int *) calloc(1, sizeof(int));

    *depth = 0;

    *state = depth;

    return 0;
}

static int setup_initQTree_Child_Depth2(void **state)
{
    int *depth = (int *) calloc(1, sizeof(int));

    *depth = 2;

    *state = depth;

    return 0;
}


static void test_initQTree(void **state)
{
    int *depth = *state;

    QTree *t = initQTree(*depth);

    assert_non_null(t);
    if(t)
    {
        assert_null(t->children);
        assert_null(t->data);
        assert_int_equal(t->depth, *depth);
    }

    state[0] = t;
    state[1] = depth;
}

static int teardownQTree(void **state)
{
    QTree *t = (QTree *) state[0];
    int *depth = state[1];

    free(depth);
    freeQTree(t);

    return 0;
}

static int testInitQTree(void)
{
    const struct CMUnitTest tests_initQTree[] = {
        cmocka_unit_test_setup_teardown(test_initQTree,
                                        setup_initQTree_Root,
                                        teardownQTree),
        cmocka_unit_test_setup_teardown(test_initQTree,
                                        setup_initQTree_Child_Depth2,
                                        teardownQTree),
    };

    return cmocka_run_group_tests_name("Test initQTree function",
                                       tests_initQTree, NULL, NULL);
}

/*
***********************************************
Tests for buildTree function
***********************************************
*/
static int setup_Depth0(void **state)
{
    QTree *t = NULL;
    int *w = NULL;

    int *depth = (int *) calloc(1, sizeof(int));
    *depth = 0;

    w = (int *) calloc(1, sizeof(int));
    *w = 8;

    state[0] = t;
    state[1] = depth;
    state[2] = w;

    return 0;
}

static int setup_Depth1(void **state)
{
    QTree *t = NULL;
    int *w = NULL;

    int *depth = (int *) calloc(1, sizeof(int));
    *depth = 0;

    w = (int *) calloc(1, sizeof(int));
    *w = 16;

    state[0] = t;
    state[1] = depth;
    state[2] = w;

    return 0;
}

static int setup_Depth2(void **state)
{
    QTree *t = NULL;
    int *w  = NULL;

    int *depth = (int *) calloc(1, sizeof(int));
    *depth = 0;

    w = (int *) calloc(1, sizeof(int));
    *w = 32;

    state[0] = t;
    state[1] = depth;
    state[2] = w;

    return 0;
}

static void test_buildTree_Handles_Depth0(void **state)
{
    QTree *t = state[0];
    int *depth = state[1];
    int *w = state[2];

    t = buildTree(/*t, */*depth, *w);

    assert_non_null(t);
    if(t)
    {
        assert_null(t->children);
        assert_non_null(t->data);
        assert_int_equal(t->depth, *depth);
    }

    /* Impossible to perform test if w is freed in a teardown function ??? */
    free(w);

    state[0] = t;
    state[1] = depth;
}

static void test_buildTree_Handles_Depth1(void **state)
{
    QTree *t = state[0];
    int *depth = state[1];
    int *w = state[2];

    t = buildTree(*depth, *w);

    assert_non_null(t);
    if(t)
    {
        assert_non_null(t->children);
        assert_non_null(t->data);
        assert_int_equal(t->depth, *depth);

        if(t->children)
        {
            int i = 0;
            for(i=0; i < NB_CHILDREN; i++)
            {
                assert_non_null(t->children[i]);

                if(t->children[i])
                    assert_null(t->children[i]->children);
            }
        }
    }

    /* Impossible to perform test if w is freed in a teardown function ??? */
    free(w);

    state[0] = t;
    state[1] = depth;
}

static void test_buildTree_Handles_Depth2(void **state)
{
    QTree *t = state[0];
    int *depth = state[1];
    int *w = state[2];

    t = buildTree(*depth, *w);

    assert_non_null(t);
    if(t)
    {
        assert_non_null(t->children);
        assert_non_null(t->data);
        assert_int_equal(t->depth, *depth);

        if(t->children)
        {
            int i = 0;
            for(i=0; i < NB_CHILDREN; i++)
            {
                assert_non_null(t->children[i]);

                if(t->children[i])
                    assert_non_null(t->children[i]->children);
            }
        }
    }

    /* Impossible to perform test if w is freed in a teardown function ??? */
    free(w);

    state[0] = t;
    state[1] = depth;
}

static int testBuildTree(void)
{
    const struct CMUnitTest tests_buildTree[] = {
        cmocka_unit_test_setup_teardown(test_buildTree_Handles_Depth0,
                                        setup_Depth0,
                                        teardownQTree),
        cmocka_unit_test_setup_teardown(test_buildTree_Handles_Depth1,
                                        setup_Depth1,
                                        teardownQTree),
        cmocka_unit_test_setup_teardown(test_buildTree_Handles_Depth2,
                                        setup_Depth2,
                                        teardownQTree),
    };

    return cmocka_run_group_tests_name("Test buildTree function",
                                       tests_buildTree, NULL, NULL);
}

/*
***********************************************
Tests for isLeaf function
***********************************************
*/
static int setup_isLeaf_Depth0(void **state)
{
    QTree *t = buildTree(0, 8);

    *state = t;

    return 0;
}

static int setup_isLeaf_Depth1(void **state)
{
    QTree *t = buildTree(0, 16);

    *state = t;

    return 0;
}

static void test_isLeaf_Handles_Depth0(void **state)
{
    QTree *t = (QTree *) (*state);

    if(t)
    {
        assert_int_equal(isLeaf(t), 1);
    }
}

static void test_isLeaf_Handles_Depth1(void **state)
{
    QTree *t = (QTree *) (*state);

    if(t)
    {
        assert_int_equal(isLeaf(t), 0);

        if(t->children)
        {
            int i = 0;
            for(i=0; i < NB_CHILDREN; i++)
            {
                assert_int_equal(isLeaf(t->children[i]), 1);
            }
        }
    }
}

static int teardownQTree_isLeaf(void **state)
{
    QTree *t = (QTree *) state[0];
    int *depth = state[1];

    free(depth);
    freeQTree(t);

    return 0;
}

static int testIsLeaf(void)
{
    const struct CMUnitTest tests_isLeaf[] = {
        cmocka_unit_test_setup_teardown(test_isLeaf_Handles_Depth0,
                                        setup_isLeaf_Depth0,
                                        teardownQTree_isLeaf),
        cmocka_unit_test_setup_teardown(test_isLeaf_Handles_Depth1,
                                        setup_isLeaf_Depth1,
                                        teardownQTree_isLeaf),
    };

    return cmocka_run_group_tests_name("Test isLeaf function",
                                       tests_isLeaf, NULL, NULL);
}

/*
***********************************************
Main test function launching all the tests
***********************************************
*/
int testQTree(void)
{
    int val = 0;

    val += testInitQTree();
    val += testBuildTree();
    val += testIsLeaf();

    return val;
}
