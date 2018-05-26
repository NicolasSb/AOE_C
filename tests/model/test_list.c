#include "../../model/list.h"
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"
#include "../../model/perso.h"
/* to use Cmocka :*/
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>



static int setup_List (void **state)
{
    List *l = calloc(1, sizeof(List));
    if (!l)
        return -1;
    l->type = LIST_PERSO;
    l->nbElts = 0;
    *state = l;

    return 0;
}


static int teardown_l (void **state)
{
    List *l = (List*)*state;
    while(l->first){
        delete_first(l);
    }
    free(l);
    return 0;
}

static void test_List_init_AND_freeList(){
    List *p = initList(1);
    assert_non_null(p);
    freeList(p);
}

static void test_insertFirst_InitialisedList(void **state)
{
    List *p = (List *) *state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    insert_first(p, pointer,1);
    insert_first(p, pointer2,1);
    assert_int_equal(p->nbElts, 2);
}

static void test_insertFirst_UninitialisedList(void **state)
{
    List *p = (List *) *state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    insert_first(p, pointer,1);
    insert_first(p, pointer2,1);
    assert_null(p);
    freePerso(pointer2);
    freePerso(pointer);
}

static void test_insertLast_InitialisedList(void **state)
{
    List *p = (List *) *state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    Perso *pointer3 = initPerso(VILLAGER);
    insert_last(p, pointer,1);
    insert_last(p, pointer2,1);
    insert_last(p, pointer3,1);
    assert_int_equal(p->nbElts, 3);
}

static void test_insertlast_UninitialisedList(void **state)
{
    List *p = (List *) *state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    insert_last(p, pointer,1);
    insert_last(p, pointer2,1);
    assert_null(p);
    freePerso(pointer2);
    freePerso(pointer);
}

static void test_removeBySearch_initialisedList(void **state)
{
    List *l = (List*)*state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    insert_first(l, pointer2,1);
    insert_first(l, pointer,1);
    deleteBySearch(l, (void *)pointer2);
    deleteBySearch(l, (void *)pointer);
    assert_int_equal(0, l->nbElts);
}


static void test_removeBySearch_UninitialisedList(void **state)
{
    List *l = (List*)*state;
    Perso *pointer = initPerso(VILLAGER);
    Perso *pointer2 = initPerso(VILLAGER);
    insert_first(l, pointer2,1);
    insert_first(l, pointer,1);
    deleteBySearch(l, (void *)pointer2);
    assert_null(l);
    freePerso(pointer2);
    freePerso(pointer);
}

static int test_all_lists()
{
    const struct CMUnitTest test_list[] ={
        cmocka_unit_test(test_List_init_AND_freeList),
        cmocka_unit_test_setup_teardown(test_insertFirst_InitialisedList, setup_List, teardown_l),
        cmocka_unit_test(test_insertFirst_UninitialisedList),
        cmocka_unit_test_setup_teardown(test_insertLast_InitialisedList, setup_List, teardown_l),
        cmocka_unit_test(test_insertlast_UninitialisedList),
        cmocka_unit_test_setup_teardown(test_removeBySearch_initialisedList, setup_List, teardown_l),
        cmocka_unit_test(test_removeBySearch_UninitialisedList)

    };

    return cmocka_run_group_tests(test_list, NULL, NULL);
}

int testList(){
    int val =0;
    val += test_all_lists();

    return val;
}
