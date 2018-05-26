
#include "../../model/game_utils.h"
#include "../../model/game.h"
#include "tests.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
/**
*   \brief init and teardown for perso
*
*
*/


static int setup_Perso (void **state)
{
    Perso *p = malloc(sizeof(Perso));
    if (!p)
        return -1;
    p->action_flag=MOVE;
    p->perso_image.x=0;
    p->perso_image.y=0;
    p->perso_image.h=60;
    p->perso_image.w=60;
    p->generation_time=55;
    p->skills.armor=5;
    p->skills.attack_range=50;
    p->skills.damage=25;
    p->skills.hp=50;
    p->skills.range=20;
    p->t.t= NULL;
    p->t.type=TARGET_INVALID;
    p->team=0;
    p->type=VILLAGER;
    *state = p;
    return 0;
}
static int teardownP (void **state)
{
    Perso *p=(Perso*)*state;
    if (*state != NULL){
        free (p);
        return 0;
    }
    return -1;
}



/**
*   \brief setup and teardown for a player
*
*
*/


static int setup_Player (void **state)
{
    Player *pl = calloc(1, sizeof(Player));
    int i;
    if (!pl)
        return -1;

    for(i=0; i< NB_RESOURCE_TYPE; i++)
        pl->available_resources[i]=20000;


    pl->team_number=1;
    pl->s.target=NULL;
    pl->s.target_type=SELECT_INVALID;

    *state = pl;

    return 0;
}


static int teardown_Pl (void **state)
{
    Player *p = (Player*)*state;
    if (p){
        freePlayer(p);
        return 0;
    }
    return -1;
}





/* TESTS PERSO FUNCTIONS */
static void test_attackB(void ** state)
{
    Perso *p =(Perso*)*state;
    Building *b = init_building(HOUSE);
    int i, res=0;

    b->team=50;
    p->perso_image.x=b->building_r.x;
    p->perso_image.y=b->building_r.y;
    b->hp = 200;
    setTarget(p, (void*)b, TARGET_BUILDING);
    for(i=0;i<15; i++)
        attackB(p);
    assert_in_range(b->hp, 0, 199);
    b->hp = -1;
    for(i=0;i<5; i++)
        res = attackB(p);
    assert_int_equal(res, 1);
    freeBuilding(b);
    setTarget(p, NULL, TARGET_INVALID);
    res = attackB(p);
    assert_int_equal(res, -1);
}


static void test_constructB_caseNULLPerso(void **state){
    Player *pl = initPlayer(0);
    Perso *p=(Perso*)*state;
    Building *b = buildB(pl, HOUSE);
    constructB(p);
    assert_null(b);
    freePlayer(pl);
}

static void test_constructB_case_nonNULLPerso(void **state){
    Player *pl = initPlayer(0);
    Perso*p=(Perso*)*state;
    Building *b, *b1, *b2, *b3;
    int i=0;
    for(i=0; i<NB_RESOURCE_TYPE; i++)
        pl->available_resources[i]+=10000;
    pl->selectP[0] = p;
    b = buildB(pl, HOUSE);
    for(i=0; i<600; i++)
        constructB(p);
    assert_int_not_equal(0, b->hp);
    freeBuilding(b);

    pl->selectP[0] = p;
    b1 = buildB(pl, BARRACKS);
    for(i=0; i<1000; i++)
        constructB(p);
    assert_int_not_equal(0, b1->hp);
    freeBuilding(b1);

    pl->selectP[0] = p;
    b2 = buildB(pl, TOWER);
    for(i=0; i<1000; i++)
        constructB(p);
    assert_int_not_equal(0, b2->hp);
    freeBuilding(b2);

    pl->selectP[0] = p;
    b3 = buildB(pl, CITY_HALL);
    for(i=0; i<1400; i++)
        constructB(p);
    assert_int_not_equal(0, b3->hp);
    freeBuilding(b3);

    freePlayer(pl);
}


static void test_BuildB_case_NULLPerso(){
    Player *pl = initPlayer(0);
    Building *b = buildB(pl, BARRACKS);
    assert_null(b);
    freePlayer(pl);
}


static void test_BuildB_case_nonNULLPerso(void **state){
    Player *pl = initPlayer(0);
    Perso *p = (void*)*state;
    Building *b, *b2;
    int i=0;
    pl->selectP[0]=p;
    b = buildB(pl, BARRACKS);
    assert_non_null(b);
    freeBuilding(b);
    for(i=0; i<NB_RESOURCE_TYPE; i++)
        pl->available_resources[i]=0;
    b2 = buildB(pl, 0);
    assert_ptr_equal(NULL, b2);
    freePlayer(pl);
}

static void test_collectResources(void ** state)
{
    Perso *p = (void*)*state;
    Resource *r=initResource(WOOD);
    int res=0, i;
    for(i=0; i<200; i++)
        res +=collectResources(p,r);
    assert_int_not_equal(0, res);
    freeResource(r);
    r= NULL;
    res = collectResources(p,r);
    assert_int_equal(0, res);
}

static void test_moveP(void **state)
{
    Perso *p =(void*)*state;
    int x = p->perso_image.x+1;
    moveP(p, x, p->perso_image.y);
    assert_int_equal(p->perso_image.x, x);
}

static void test_targetType_case_NULLPerso(void **state){
    Perso*p = (Perso*)*state;
    int a = targetType(p);
    assert_int_equal(TARGET_INVALID, a);
}

static void test_targetType_case_nonNULLPerso(void **state){
    int a;
    Perso*p = (Perso*)*state;
    Building* b= init_building(CITY_HALL);
    setTarget(p, b, TARGET_BUILDING);
    a = targetType(p);
    assert_int_not_equal(TARGET_INVALID, a);
    freeBuilding(b);
}


/*Tests on buildings functions */

static void test_constructP(void ** state)
{
    Player *pl = (void*)*state;
    Building *b = init_building(HOUSE);
    Building *b1 = init_building(BARRACKS);
    Perso *p, *p1, *p2;
    int i, res=0;
    p = createPerso(b, pl, VILLAGER);
    assert_non_null(p);
    for(i=0; i<30; i++)
    {
        constructP(b);
        res +=p->skills.hp;
    }
    assert_int_not_equal(0, p->skills.hp);
    res = 0;
    freePerso(p);

    p1 = createPerso(b1, pl, ARCHER);
    assert_non_null(p1);
    for(i=0; i<30; i++)
    {
        constructP(b1);
        res +=p1->skills.hp;
    }
    assert_int_not_equal(0, p1->skills.hp);
    res = 0;
    freePerso(p1);

    p2 = createPerso(b1, pl, SWORDMAN);
    assert_non_null(p2);
    for(i=0; i<60; i++)
    {
        constructP(b1);
        res +=p2->skills.hp;
    }
    assert_int_not_equal(0, p2->skills.hp);
    res = 0;
    freePerso(p2);

    freeBuilding(b);
    freeBuilding(b1);
}

/* TESTS ON PLAYER'S Functions */



static void test_Player_init(){
    Player *p = initPlayer(1);
    assert_non_null(p);
    freePlayer(p);
}

static void Test_setSelect_Handles_nullPlayer(void ** state){
    Player *pl = (Player*)*state;
    Perso *p = initPerso(VILLAGER);
    int res = setSelect(pl, (void*) p, VILLAGER);
    assert_int_equal(-1, res);
    freePerso(p);
}

static void Test_setSelect_Handles_NONnullPlayer(void ** state){
    Player *pl = (Player*)*state;
    Perso *p = initPerso(VILLAGER);
    int res = setSelect(pl, (void*) p, VILLAGER);
    assert_int_equal(0,res);

    freePerso(p);
}


static void test_selectPerso_case_nonNULLPlayer(void **state){
    Player *pl = (Player*) *state;
    int res;
    Perso *p =initPerso(VILLAGER);
    setCoord(p, 50,50);
    /*select_onePerso(g, pl, 50, 50);*/
    res = pl->s.target==p ? 1: 0;
    assert_int_equal(res, 0);
    freePerso(p);
}

static void test_selectPerso_case_NULLPlayer(void **state){
    Player *pl = (Player*) *state;
    Perso *p =initPerso(VILLAGER);
    setCoord(p, 50,50);
    /*select_onePerso(g, pl, 50, 50);*/
    assert_null(pl);
    freePerso(p);
}

static void test_addSelectedP(void** state)
{
    int res;
    Player *pl = (Player *)*state;
    Perso *p = initPerso(VILLAGER);
    p->team = pl->team_number;
    clear_selectP(pl);
    res = addSelectedP(pl, p);
    assert_int_equal(0, res);
    freePerso(p);
    p = NULL;
    clear_selectP(pl);
    res = addSelectedP(pl, p);
    assert_int_equal(-1, res);
}

static void test_clear_selectP(void **state)
{
    Player *pl = (void *) *state;
    Perso* p = initPerso(VILLAGER);
    setSelect(pl, (void*) p, TARGET_PERSO);
    addSelectedP(pl, p);
    clear_selectP(pl);
    assert_null(pl->selectP[0]);
    assert_null(pl->s.target);
}



/*
    other functions

Mouvment follow (Animals * animals_name, Perso* p);
int decrease_life_point (Animals * animals_name, Perso * p);
int character_is_seen (Animals * animals_name, Perso * p);
int is_in_area (Ressource * ressource_name, Perso * p);

*/

static int test_all_player()
{
    const struct CMUnitTest test_game_utils[] ={
        cmocka_unit_test(test_Player_init),
        cmocka_unit_test(Test_setSelect_Handles_nullPlayer),
        cmocka_unit_test_setup_teardown(Test_setSelect_Handles_NONnullPlayer, setup_Player, teardown_Pl),
        cmocka_unit_test(test_selectPerso_case_NULLPlayer),
        cmocka_unit_test_setup_teardown(test_selectPerso_case_nonNULLPlayer, setup_Player, teardown_Pl),
        cmocka_unit_test_setup_teardown(test_addSelectedP, setup_Player, teardown_Pl),
        cmocka_unit_test_setup_teardown(test_clear_selectP, setup_Player, teardown_Pl)
    };

    return cmocka_run_group_tests(test_game_utils, NULL, NULL);
}


static int test_all_perso_functions()
{
    const struct CMUnitTest test_perso_functions[] ={
        cmocka_unit_test_setup_teardown(test_attackB, setup_Perso,teardownP),
        cmocka_unit_test(test_BuildB_case_NULLPerso),
        cmocka_unit_test_setup_teardown(test_BuildB_case_nonNULLPerso, setup_Perso, teardownP),
        cmocka_unit_test(test_constructB_caseNULLPerso),
        cmocka_unit_test_setup_teardown(test_constructB_case_nonNULLPerso,setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_collectResources,setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_moveP,setup_Perso, teardownP),
        cmocka_unit_test(test_targetType_case_NULLPerso),
        cmocka_unit_test_setup_teardown(test_targetType_case_nonNULLPerso, setup_Perso, teardownP),
    };

    return cmocka_run_group_tests(test_perso_functions, NULL, NULL);
}



static int test_all_buildings_functions()
{
    const struct CMUnitTest test_buildings_functions[] ={
        cmocka_unit_test_setup_teardown(test_constructP,setup_Player, teardown_Pl)
    };

    return cmocka_run_group_tests(test_buildings_functions, NULL, NULL);
}


int test_player(){
    int val =0;
    val += test_all_player();
    val += test_all_perso_functions();
    val += test_all_buildings_functions();
    return val;
}

