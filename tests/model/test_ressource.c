#include <stdio.h>
#include <stdlib.h>
#include "test_ressource.h"
#include "../../model/ressource.h"


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static int setup_ressource (void**state)
{
    Resource * ress = initResource(WOOD);
    if (ress == NULL)
        return -1;
    *state = ress;
        return 0;
}

static int teardown (void**state)
{
    free(*state);
    return 0;
}

static void initWood_test ()
{
    Resource * ress = initResource(WOOD);
    assert_non_null(ress);
}

static void initStone_test ()
{
    Resource * ress = initResource(STONE);
    assert_non_null(ress);
}

static void initFood_test ()
{
    Resource * ress = initResource(FOOD);
    assert_non_null(ress);
}

static void initGold_test ()
{
    Resource * ress = initResource(GOLD);
    assert_non_null(ress);
}

static void set_position_test (void ** state)
{
    Resource * ress =(Resource *)(state[0]);
    int x = 44;
    int y = 55;
    set_position (ress, x, y);
    assert_int_equal(ress->r.x, 44);
    assert_int_equal(ress->r.y, 55);
}

static void get_coordo_test(void ** state)
{
    Resource * ress =(Resource *)(state[0]);
    int x = 44;
    int y = 55;
    int x_test = 0;
    int y_test = 0;
    set_position (ress, x, y);
    get_coordo(ress, &x_test, &y_test);
    assert_int_equal(x_test, 44);
    assert_int_equal(y_test, 55);
}

static void improve_collect_time_test (void ** state)
{
    Resource * ress =(Resource *)(state[0]);
    int test;
    int level = 2;
    ress->collect_time = 2;
    test = improve_collect_time(ress, level);
    assert_int_equal(ress->level,2);
    assert_int_equal(ress->collect_time, 4);
    assert_int_equal(test, 1);
}

static void improve_collect_time_test_null (void ** state)
{
    Resource * ress =(Resource *)(state[0]);
    int test;
    int level = 2;
    test = improve_collect_time(ress, level);
    assert_int_equal(test, 0);
}

static void decreaseRessource_test (void ** state)
{
    Resource * ress = (Resource *)(state[0]);
    int test =1;
    ress->amount = 40;
    test = decreaseResource(ress,1);
    assert_int_equal(ress->amount, 39);
    assert_int_equal(test, 1);
}

static void decreaseRessource_test_null (void ** state)
{
    Resource * ress = (Resource *)(state[0]);
    int test = 1;
    test = decreaseResource(ress,1);
    assert_int_equal(test, 0);
}

int test_all_ressource()
{
    const struct CMUnitTest test_perso[] ={
        cmocka_unit_test(initWood_test),
        cmocka_unit_test(initStone_test),
        cmocka_unit_test(initFood_test),
        cmocka_unit_test(initGold_test),
        cmocka_unit_test_setup_teardown(set_position_test, setup_ressource, teardown),
        cmocka_unit_test_setup_teardown(get_coordo_test, setup_ressource, teardown),
        cmocka_unit_test_setup_teardown(improve_collect_time_test, setup_ressource, teardown),
        cmocka_unit_test(improve_collect_time_test_null),
        cmocka_unit_test_setup_teardown(decreaseRessource_test, setup_ressource, teardown),
        cmocka_unit_test(decreaseRessource_test_null),
        /*cmocka_unit_test_setup(freeAnimals_test, setup_animals),*/
        };
        return cmocka_run_group_tests(test_perso, NULL, NULL);
}
