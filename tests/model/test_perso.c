
#include "../../model/perso.h"
#include "tests.h"

/* to use Cmocka : */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


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
    p->type=1;
    *state = p;
    return 0;
}

static int setup_2Perso (void **state)
{
    Perso * p= initPerso(VILLAGER);
    Perso * p2 = initPerso(VILLAGER);
    state[0] = p;
    state[1] = p2;
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

static int teardownP2 (void **state)
{
    Perso *p= (Perso*)state[0];
    Perso *p2 = (Perso*)state[1];
    if (*state != NULL && state[1]!=NULL){
        freePerso(p);
        freePerso(p2);
        return 0;
    }
    return -1;
}

static void test_Perso_create_villager()
{
    Perso * p = initPerso(VILLAGER);
    assert_non_null(p);
    freePerso(p);
}

static void test_Perso_create_archer()
{
    Perso * p = initPerso(ARCHER);
    assert_non_null(p);
    freePerso(p);
}

static void test_Perso_create_swordman()
{
    Perso * p = initPerso(SWORDMAN);
    assert_non_null(p);
    freePerso(p);
}

static void test_setcoord_Handles_From_Null_Pointer(void **state)
{
    int x,y;
    int booleen;
    Perso * p = (Perso *) *state;
    setCoord(p, 0, 0);
    getCoordinates(p, &x, &y);
    booleen = (x==0 && y ==0 )? 1 : 0;
    assert_int_equal(1, booleen);
}

static void test_setcoord_Handles_From_itialised_Pointer(void **state)
{
    int x,y;
    int booleen;
    Perso * p = (Perso *) *state;
    setCoord(p, 0, 0);
    getCoordinates(p, &x, &y);
    booleen = (x==0 && y ==0 )? 1:0;
    assert_int_equal(1, booleen);
}


static void test_attack_Handles_initialised_Persos_noHPleft(void **state)
{
    int a;
    Perso * p = (Perso *) state[0];
    Perso * p2 = (Perso *) state[1];
    p->perso_image.x = p2->perso_image.x;
    p->perso_image.y = p2->perso_image.y;
    setTeam(p2, 2);
    p2->skills.hp = -1;
    setTarget(p, (void*) p2, TARGET_PERSO);
    a=attack(p);
    freePerso(p);
    freePerso(p2);
    assert_int_equal(1,a);
}

static void test_attack_Handles_initialised_Persos_Withhp(void **state)
{
    int a;
    Perso * p = (Perso *) state[0];
    Perso * p2 = (Perso *) state[1];
    setTeam(p2, 1);
    setTarget(p, (void *)p2, TARGET_PERSO);
    p->skills.hp = p2->skills.hp = 50;
    a = attack(p);
    assert_int_equal(0,a);
}

static void test_attack_Handles_initialised_Persos(void **state)
{
    int a;
    Perso * p = (Perso *) state[0];
    Perso * p2 = (Perso *) state[1];
    setTarget(p, (void *) p2, TARGET_PERSO);
    setTeam(p2, 1);
    p->perso_image.x=p->perso_image.y=500;
    a=attack(p);
    assert_int_equal(0,a);
}


/*static void test_attack_Handles_initialised_Persos_case_same_team(void **state)
{
    int a;
    Perso * p = (Perso *) state[0];
    Perso * p2 = (Perso *) state[1];
    p->t.t = (void*)p2;
    p->perso_image.x=p->perso_image.y=500;
    a=attack(p);
    assert_int_not_equal(0,a);
}*/


static void test_attack_Handles_uninitialised_Persos(void **state){
    int a;
    Perso * p = (Perso *) *state;
    getTeam(p);
    a = attack(p);

    assert_int_equal(-1, a);
}


static void test_set_attack_Handles_Null_Pointer(void ** state)
{
    Perso * p = (Perso *) *state;
    Perso * p2 = (Perso *) *state+1;
    int  i = setAttacked(p, p2);
    assert_int_not_equal(0, i);
}

static void test_set_attack_Handles_Not_Null_Pointer(void ** state)
{
    Perso * p = (Perso *) *state;
    Perso * p2 = (Perso *) *state+1;
    int res = setAttacked(p, p2);
    assert_int_equal(0,res);
}

static void test_follow_unitialised_perso(void ** state)
{
    Perso * p = (Perso *) *state;
    Perso * p2 = (Perso *) *state+1;
    int res = follow_p(p,p2);
    assert_int_not_equal(0, res);
}

static void test_follow_itialised_perso(void ** state)
{
    Perso * p = (Perso *) state[0];
    Perso * p2 = (Perso *) state[1];
    int res = follow_p(p,p2);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_null(void ** state)
{
    Perso * p = (Perso*) *state;
    int res = move_p(p, 10, 10);
    assert_int_not_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_1(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = p->perso_image.y =2;
    res = move_p(p, 2, 0);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_2(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 2;
    p->perso_image.y = 1;
    res = move_p(p, 6, 5);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_3(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 3;
    p->perso_image.y = 2;
    res = move_p(p, 0, 5);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_4(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 2;
    p->perso_image.y = 3;
    res = move_p(p, 6, 3);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_5(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 3;
    p->perso_image.y = 3;
    res = move_p(p, 0, 0);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_6(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 2;
    p->perso_image.y = 2;
    res = move_p(p, 1, 2);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_7(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 1;
    p->perso_image.y = 2;
    res = move_p(p, 1,6);
    assert_int_equal(0, res);
}

static void test_Perso_move_handles_Perso_not_null_case_code_8(void ** state)
{
    Perso * p = *state;
    int res;
    p->perso_image.x = 1;
    p->perso_image.y = 3;
    res = move_p(p, 4, 0);
    assert_int_equal(0, res);
}

static void test_compute_distance()
{
    int i, j, k, l;
    int res;
    i=j=1;
    l=k=0;
    res = computeDistance(i,j,k,l);
    assert_int_equal(1,res);
}
static void test_getCoordinates_handles_null_pointer(void **state)
{
    int res;
    Perso * p =(Perso*) *state;
    int x, y;
    getCoordinates(p, &x, &y);
    res = (x==0 && y == 0 )? 0 : -1 ;
    assert_int_equal(0,res);
}

static void test_setRect(void **state){
    Perso *p =(Perso*)*state;
    SDL_Rect r;
    r.h=r.w=r.x=r.y=10;
    setRect(p, r);
    assert_int_equal(10, p->perso_image.x);
}

static void test_getCoordinates_handles_not_null_pointer(void **state)
{
    Perso * p = *state;
    int x, y, res;
    getCoordinates(p, &x, &y);
    res = (x==0 && y == 0 )? 0 : -1 ;
    assert_int_equal(0,res);
}

static void test_getAtionflag_Handle_nullPointer(void **state){
    Perso *p = (Perso*) *state;
    int res = getActionFlagP(p);
    assert_int_equal(-1, res);
}


static int test_all_perso()
{
    const struct CMUnitTest test_characters[] ={
        cmocka_unit_test(test_Perso_create_archer),
        cmocka_unit_test(test_Perso_create_swordman),
        cmocka_unit_test(test_Perso_create_villager),
        cmocka_unit_test(test_setcoord_Handles_From_Null_Pointer),
        cmocka_unit_test_setup_teardown(test_setcoord_Handles_From_itialised_Pointer,setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_attack_Handles_initialised_Persos, setup_2Perso, teardownP2),
        cmocka_unit_test_setup(test_attack_Handles_initialised_Persos_noHPleft, setup_2Perso),
        cmocka_unit_test_setup_teardown(test_attack_Handles_initialised_Persos_Withhp, setup_2Perso, teardownP2),
        cmocka_unit_test(test_attack_Handles_uninitialised_Persos),
        cmocka_unit_test(test_set_attack_Handles_Null_Pointer),
        cmocka_unit_test_setup_teardown(test_set_attack_Handles_Not_Null_Pointer,setup_2Perso, teardownP2),
        cmocka_unit_test(test_follow_unitialised_perso),
        cmocka_unit_test_setup_teardown(test_follow_itialised_perso,setup_2Perso, teardownP2),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_1, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_2, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_3, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_4, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_5, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_6, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_7, setup_Perso, teardownP),
        cmocka_unit_test_setup_teardown(test_Perso_move_handles_Perso_not_null_case_code_8, setup_Perso, teardownP),
        cmocka_unit_test(test_Perso_move_handles_Perso_null),
        cmocka_unit_test(test_compute_distance),
        cmocka_unit_test_setup_teardown(test_getCoordinates_handles_not_null_pointer, setup_Perso, teardownP),
        cmocka_unit_test(test_getCoordinates_handles_null_pointer),
        cmocka_unit_test(test_getAtionflag_Handle_nullPointer),
        cmocka_unit_test_setup_teardown(test_setRect, setup_Perso, teardownP)
    };

    return cmocka_run_group_tests(test_characters, NULL, NULL);
}

int test_perso(){
    int val =0;
    val += test_all_perso();

    return val;
}
