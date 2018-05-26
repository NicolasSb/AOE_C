#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "perso.h"
#include "buildings.h"
#include "ressource.h"
#include "animals.h"

/**
*   \file   list.c
*   \brief  implementation of lists that will be used for characters and buildings
*   \author Nicolas Sourbier
*   \date   25/03/2017
*   \note   test coverage with GCOV is 80 %
*/



/**
*   \brief to initialize a list
*   \param \a type the type of the list ( should be LIST_PERSO or LIST_BUILDING)
*/

List *initList(int type)
{
    List *l = calloc(1, sizeof(List));

    if (l == NULL)
    {
        return NULL;
    }
    l->type = type;
    l->nbElts=0;
    return l;
}



/**
*   \brief to insert an element at the first position
*   \param \a l the list we want to insert the \a p element
*/
void insert_first(List *l, void *p, int team)
{
    Element *new_element = calloc(1, sizeof(Element));
    if (l != NULL && new_element != NULL){
        new_element->pointer = p;
        new_element->next = l->first;
        new_element->team = team;
        l->first = new_element;

        ++(l->nbElts);
    }
    else
    {
        free(new_element);
    }
}


/**
*   \brief to insert an element at the last position
*   \param \a l the list we want to insert the \a p element
*/
void insert_last(List *l, void *p, int team)
{
    if (l && !(l->first)){
        insert_first(l, p, team);
    }
    else if (l && l->first){
        Element *new_element = calloc(1, sizeof(Element));
        Element *current = l->first;
        while(current->next){
            current=current->next;
        }
        new_element->pointer = p;
        new_element->next=NULL;
        new_element->team = team;
        current->next=new_element;
        ++(l->nbElts);
    }
}


/**
*   \brief to free the list AND it's elements
*   \param \a l the list we want to free
*/
void freeList(List *l){
    if (l){
        while(l->first){
            delete_first(l);
        }
        free(l);
    }
}

/**
*   \brief to delete the first element
*/
void delete_first(List *l)
{

    if (l->first != NULL)
    {
        Element *toDel = l->first;
        l->first = l->first->next;
        toDel->next=NULL;
        switch (l->type)
        {
        case LIST_PERSO:
            if(toDel->pointer)
                freePerso((Perso*)toDel->pointer);
            break;
        case LIST_BUILDING:
            if(toDel->pointer)
                freeBuilding((Building*)toDel->pointer);
            break;
        case LIST_RESOURCE:
            if(toDel->pointer)
                freeResource((Resource*)toDel->pointer);
            break;
        case LIST_ANIMALS:
            if(toDel->pointer)
                freeAnimals((Animals*)toDel->pointer);
            break;
        }
        free(toDel);
        --(l->nbElts);
    }
}


/**
*   \brief to delete an element by searching it in the list
*   \param \a l the list where we want to delete the \a p element
*   \note If the element is not found, nothing will happen
*/
void deleteBySearch(List *l, void *p)
{
    if(l){
        Element *previous = l->first;
        Element *current =l->first;
        if(l->first->pointer==p){
            delete_first(l);
            current = NULL;
            previous = NULL;
        }

        if (current){
            do{
                if (current->pointer == p){
                    previous->next=current->next;
                    current->next=NULL;
                    switch (l->type){
                    case LIST_BUILDING:
                        freeBuilding((Building*)current->pointer);
                        free(current);
                        --(l->nbElts);
                        break;
                    case LIST_PERSO:
                        freePerso((Perso*)current->pointer);
                        free(current);
                        --(l->nbElts);
                        break;
                    case LIST_RESOURCE:
                        freeResource((Resource*)current->pointer);
                        free(current);
                        --(l->nbElts);
                        break;
                    case LIST_ANIMALS:
                        freeAnimals((Animals*)current->pointer);
                        free(current);
                        --(l->nbElts);
                        break;
                    }
                    break;
                }
                else{
                    previous=current;
                    current=current->next;
                }
            }while (current);
        }
    }
}


/**
*   \brief to delete the first element
*/
static void remove_first(List *l)
{
    if (l->first != NULL)
    {
        l->first = l->first->next;
        --(l->nbElts);
    }
}


/**
*   \brief to delete an element by searching it in the list without freeing it
*   \param \a l the list where we want to delete the \a p element
*   \note If the element is not found, nothing will happen
*/
static void removeBySearch(List *l, void *p)
{
    if(l){
        Element *previous = l->first;
        Element *current =l->first;
        if(l->first->pointer==p){
            remove_first(l);
            current = NULL;
            previous = NULL;
        }

        if (current){
            do{
                if (current->pointer == p){
                    previous->next=current->next;
                    --(l->nbElts);
                    break;
                }
                else{
                    previous=current;
                    current=current->next;
                }
            }while (current);
        }
    }
}

/**
*   \brief to free the list BUT NOT it's elements
*   \param \a l the list we want to free
*/
void emptyList(List *l){
    if (l){
        while(l->first!=NULL){
            remove_first(l);
        }
        free(l);
    }
}



static void * findElement(List *l, int d)
{
    if (l)
    {
    Element *current = l->first;
        switch (l->type)
        {
        case LIST_BUILDING:
            if (current){
                do{
                    Building *tmp = (Building *)current->pointer;
                    if (tmp->dist_to_cam==d)
                        return current;
                }while((current = current->next));
            }
            break;
        case LIST_PERSO:
            if (current){
                do{
                    Perso *tmp = (Perso *)current->pointer;
                    if (tmp)
                    {
                        if (tmp->dist_to_cam==d)
                        return current;
                    }
                }while((current = current->next));
            }
            break;
        case LIST_RESOURCE:
            if (current){
                do{
                    Resource *tmp = (Resource *)current->pointer;
                    if (tmp->dist_to_cam==d)
                        return current;
                }while((current = current->next));
            }
            break;
        case LIST_ANIMALS:
            if (current){
                do{
                    Animals *tmp = (Animals *)current->pointer;
                    if (tmp->dist_to_cam==d)
                        return current;
                }while((current = current->next));
            }
            break;
        }
    }
    return NULL;
}

static int* saveListDist(List *l)
{
    const int nb =l->nbElts;
    int *tabDist=malloc(sizeof(int)*nb);
    if (l)
    {
        int i;
        Element *current = l->first;
        switch (l->type)
        {
        case LIST_BUILDING:
            i=0;
            if (current){
                do{
                    Building *tmp = (Building *)current->pointer;
                    tabDist[i]=tmp->dist_to_cam;
                    ++i;
                    }while((current=current->next));
            }
            break;
        case LIST_PERSO:
            i=0;
            if (current){
                do{
                    Perso *tmp = (Perso *)current->pointer;
                    if (tmp)
                        tabDist[i]=tmp->dist_to_cam;
                    ++i;
                }while((current=current->next));
            }
            break;
        case LIST_RESOURCE:
            i=0;
            if (current){
                do{
                    Resource *tmp = (Resource *)current->pointer;
                    tabDist[i]=tmp->dist_to_cam;
                    ++i;
                }while((current=current->next));
            }
            break;
        case LIST_ANIMALS:
            i=0;
            if (current){
                do{
                    Animals *tmp = (Animals *)current->pointer;
                    tabDist[i]=tmp->dist_to_cam;
                    ++i;
                }while((current=current->next));
            }
            break;
        }
        return tabDist;
    }
    return NULL;
}

static int compare (void const *a, void const *b)
{
   int const *pa = a;
   int const *pb = b;
   return *pb - *pa;
}

List *sortList(List *l)
{

    if (l){
        List * res=NULL;
        int nbElt = l->nbElts;
        switch(l->type)
        {
        int i;
        int *a;
        case LIST_BUILDING:
            res=initList(LIST_BUILDING);
            a = saveListDist(l);
            qsort(a, l->nbElts, sizeof(int), compare);
            for(i=0;i<nbElt; i++)
            {
                Element *e = findElement(l, a[i]);
                Building *b = (Building *) e->pointer;
                if (e){
                    insert_last(res, (void *)b, e->team);
                    removeBySearch(l, e->pointer);
                    e->pointer=NULL;
                    e->next=NULL;
                    free(e);
                }
            }
            free(a);
            emptyList(l);
            break;
        case LIST_PERSO:
            res=initList(LIST_PERSO);
            a = saveListDist(l);
            qsort(a, l->nbElts, sizeof(int), compare);
            for(i=0; i<nbElt; i++)
            {
                Element *e = findElement(l, a[i]);
                if (e){
                    Perso *p = (Perso*)e->pointer;
                    insert_last(res,(void*) p, e->team);
                    removeBySearch(l, (void*) p);
                    e->pointer=p=NULL;
                    e->next=NULL;
                    free(e);
                }
            }
            free(a);
            emptyList(l);
            break;
        case LIST_RESOURCE:
            res=initList(LIST_RESOURCE);
            a = saveListDist(l);
            qsort(a, l->nbElts, sizeof(int), compare);   /* a completer tester et vérifier*/
            for(i=0; i<nbElt; i++)
            {
                Element *e = findElement(l, a[i]);
                Resource *r = (Resource *)e->pointer;
                if (e){
                    insert_last(res,(void*)r, e->team);
                    removeBySearch(l, (void*)r);
                    e->pointer=NULL;
                    e->next=NULL;
                    free(e);
                }
            }
            free(a);
            emptyList(l);
            break;
        case LIST_ANIMALS:
            res=initList(LIST_ANIMALS);
            a = saveListDist(l);
            qsort(a, l->nbElts, sizeof(int), compare);   /* a completer tester et vérifier*/
            for(i=0; i<nbElt; i++)
            {
                Element *e = findElement(l, a[i]);
                Animals *ani = (Animals *)e->pointer;
                if (e){
                    insert_last(res,(void*)ani, e->team);
                    removeBySearch(l, (void*)ani);
                    e->pointer=NULL;
                    e->next=NULL;
                    free(e);
                }
            }
            free(a);
            emptyList(l);
            break;
        }

        return res;
    }
    return NULL;
}
