
#include "../../model/buildings.h"
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"
#include "../../model/files.h"

/* to use Cmocka : */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


static int setup_Building (void **state)
{
    Building *b = calloc(1, sizeof (Building));
    if (!b)
        return -1;
    b->action = 0;
    b->building_r.x=0;
    b->building_r.y=0;
    b->building_r.h=50;
    b->building_r.w=50;
    b->hp=500;
    b->range=50;
    b->target=NULL;
    b->type=HOUSE;
    b->team = 0;
    *state = b;
    return 0;
}



static int teardown (void **state)
{
    Building *b = (Building *) (*state);

    if(b)
    {
        b->target=NULL;
        free(b);
    }

    return 0;
}

void test_Building_initiation_Case_HOUSE(void **state){
    Building *b = init_building(HOUSE);
    assert_non_null(b);

    *state = b;
}

void test_Building_initiation_Case_BARRACKS(void **state){
    Building *b = init_building(BARRACKS);
    assert_non_null(b);

    *state = b;
}

void test_Building_initiation_Case_CITYHALL(void **state){
    Building *b = init_building(CITY_HALL);
    assert_non_null(b);

    *state = b;
}

void test_Building_initiation_Case_TOWER(void **state){
    Building *b = init_building(TOWER);
    assert_non_null(b);

    *state = b;
}

void test_freeBuilding_Handles_initialisedBuilding (void **state)
{
    Building *b = *state;
    int res = freeBuilding(b);
    assert_int_not_equal(-1, res);
}

void test_freeBuilding_Handles_uninitialisedBuilding (void **state)
{
    Building *b = *state;
    int res = freeBuilding(b);
    assert_int_equal(-1, res);
}

void test_setActionFlag_Handles_initialisedBuilding(void ** state)
{
    int res;
    Building *b = *state;
    setActionFlag(b, CONSTRUCT_P);
    res = getActionFlag(b) == CONSTRUCT_P ? 1:0;
    assert_int_equal(1, res);
}


void test_setActionFlag_Handles_uninitialisedBuilding(void ** state)
{
    int res;
    Building *b = *state;
    setActionFlag(b, CONSTRUCT_P);
    res = getActionFlag(b) == CONSTRUCT_P ? 1:0;
    assert_int_equal(0, res);
}

int test_all_buildings()
{
    const struct CMUnitTest test_Building[] ={
        cmocka_unit_test_teardown(test_Building_initiation_Case_BARRACKS,
                                  teardown),
        cmocka_unit_test_teardown(test_Building_initiation_Case_CITYHALL,
                         teardown),
        cmocka_unit_test_teardown(test_Building_initiation_Case_HOUSE,
                                  teardown),
        cmocka_unit_test_teardown(test_Building_initiation_Case_TOWER,
                                  teardown),
        cmocka_unit_test_setup(test_freeBuilding_Handles_initialisedBuilding, setup_Building),
        cmocka_unit_test(test_freeBuilding_Handles_uninitialisedBuilding),
        cmocka_unit_test_setup_teardown(test_setActionFlag_Handles_initialisedBuilding, setup_Building, teardown),
        cmocka_unit_test(test_setActionFlag_Handles_uninitialisedBuilding)
    };

    return cmocka_run_group_tests(test_Building, NULL, NULL);
}


int testBuildings(){
    int val =0;
    val += test_all_buildings();

    return val;
}
