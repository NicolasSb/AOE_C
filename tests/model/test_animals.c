#include <stdio.h>
#include <stdlib.h>
#include "tests.h"
#include "../../model/animals.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


static int setup_animals (void**state)
{
    Animals * ani = initWolf();
    if (ani == NULL)
        return -1;
    *state = ani;
        return 0;
}

static int teardown (void**state)
{
    free(*state);
    return 0;
}

static void test_init_wolf ()
{
    Animals * ani = initWolf();
    assert_non_null(ani);
}

static void test_init_boar ()
{
    Animals * ani = initBoar();
    assert_non_null(ani);
}

static void test_init_sheep ()
{
    Animals * ani = initSheep();
    assert_non_null(ani);
}

static void test_init_stag ()
{
    Animals * ani = initStag();
    assert_non_null(ani);
}

static void test_change_coordinates (void **state)
{
    Animals * ani = (Animals *)(state[0]);
    int x = 65;
    int y = 75;
    change_coordinates (ani, x, y);
    assert_int_equal(ani->r.x,65);
    assert_int_equal(ani->r.y,75);
}

static void test_get_coordo_animals (void ** state)
{
    Animals * ani = (Animals *)(state[0]);
    int x = 65;
    int y = 75;
    int x_test = 0;
    int y_test = 0;
    change_coordinates(ani,x,y);
    get_coordo_animals(ani, &x_test, &y_test);
    assert_int_equal(x_test,65);
    assert_int_equal(y_test,75);
}

static void move_left_above_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_left_above (ani);
   assert_int_equal(test,1);
}

static void move_left_above_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_left_above (ani);
   assert_int_equal(test,0);
}

static void move_right_above_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_right_above (ani);
   assert_int_equal(test,1);
}

static void move_right_above_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_right_above (ani);
   assert_int_equal(test,0);
}


static void move_left_bellow_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_left_bellow (ani);
   assert_int_equal(test,1);
}

static void move_left_bellow_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_left_bellow (ani);
   assert_int_equal(test,0);
}

static void move_right_bellow_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_right_bellow (ani);
   assert_int_equal(test,1);
}

static void move_right_bellow_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_right_bellow (ani);
   assert_int_equal(test,0);
}

static void move_right_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_right(ani);
   assert_int_equal(test,1);
}

static void move_right_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_right(ani);
   assert_int_equal(test,0);
}

static void move_left_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_left(ani);
   assert_int_equal(test,1);
}

static void move_left_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_left(ani);
   assert_int_equal(test,0);
}

static void move_above_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_above(ani);
   assert_int_equal(test,1);
}

static void move_above_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test;
   test = move_above(ani);
   assert_int_equal(test,0);
}

static void move_bellow_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test=0;
   ani->r.x = 50;
   ani->r.y = 50;
   test = move_bellow(ani);
   assert_int_equal(test,1);
}

static void move_bellow_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test=0;
   test = move_bellow(ani);
   assert_int_equal(test,0);
}

static void is_attacking_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test = 0;
   test = is_attacking (ani);
   assert_int_equal(test,1);
}

static void is_attacking_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test = 0;
   test = is_attacking (ani);
   assert_int_equal(test,0);
}

static void enrage_test (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test = 0;
   test = enrage (ani);
   assert_int_equal(test,1);
}

static void enrage_test_null (void ** state)
{
   Animals * ani = (Animals *)(state[0]);
   int test = 0;
   test = enrage (ani);
   assert_int_equal(test,0);
   freeAnimals(ani);
}


static int test_all_animals()
{
    const struct CMUnitTest test_ani[] ={
        cmocka_unit_test(test_init_wolf),
        cmocka_unit_test(test_init_boar),
        cmocka_unit_test(test_init_sheep),
        cmocka_unit_test(test_init_stag),
        cmocka_unit_test_setup_teardown(test_change_coordinates, setup_animals, teardown),
        cmocka_unit_test_setup_teardown(test_get_coordo_animals, setup_animals, teardown),
        cmocka_unit_test_setup_teardown(move_left_above_test, setup_animals, teardown),
        cmocka_unit_test(move_left_above_test_null),
        cmocka_unit_test_setup_teardown(move_right_above_test, setup_animals, teardown),
        cmocka_unit_test(move_right_above_test_null),
        cmocka_unit_test_setup_teardown(move_left_bellow_test, setup_animals, teardown),
        cmocka_unit_test(move_left_bellow_test_null),
        cmocka_unit_test_setup_teardown(move_right_bellow_test, setup_animals, teardown),
        cmocka_unit_test(move_right_bellow_test_null),
        cmocka_unit_test_setup_teardown(move_right_test, setup_animals, teardown),
        cmocka_unit_test(move_right_test_null),
        cmocka_unit_test_setup_teardown(move_left_test, setup_animals, teardown),
        cmocka_unit_test(move_left_test_null),
        cmocka_unit_test_setup_teardown(move_above_test, setup_animals, teardown),
        cmocka_unit_test(move_above_test_null),
        cmocka_unit_test_setup_teardown(move_bellow_test, setup_animals, teardown),
        cmocka_unit_test(move_bellow_test_null),
        cmocka_unit_test_setup_teardown(is_attacking_test, setup_animals, teardown),
        cmocka_unit_test(is_attacking_test_null),
        cmocka_unit_test_setup(enrage_test, setup_animals),
        cmocka_unit_test(enrage_test_null),
        };
        return cmocka_run_group_tests(test_ani, NULL, NULL);
}


int test_animals(){
    int val =0;
    val += test_all_animals();

    return val;
}


