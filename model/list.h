
#ifndef LIST_H
#define LIST_H

typedef struct Element Element;
struct Element
{
    void *pointer;
    Element *next;
    int team;
};

typedef struct List List;
struct List
{
    Element *first;
    int nbElts;
    int type;
};

enum {LIST_BUILDING, LIST_PERSO, LIST_RESOURCE, LIST_ANIMALS};

List *initList(int i);

void insert_first(List *List, void *p, int team);
void insert_last(List *l, void *p, int team);

void delete_first(List *List);
void deleteBySearch(List *l, void *p);

void emptyList(List *l);
void freeList(List *l);


List * sortList(List *l);




#endif
