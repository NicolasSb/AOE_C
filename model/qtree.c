/**
 * \file qtree.c
 * \brief Definition of QTree used to store map data
 * \author Léo Gauvin
 * \version 1.0
 * \date March, 28th 2017
 */

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "qtree.h"
#include "list.h"
#include <math.h>

QTree *initQTree(int depth)
{
    QTree *t = (QTree *) calloc(1, sizeof(QTree));

    if(t == NULL)
    {
        fprintf(stderr, "Error when allocating QTree in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    t->data = NULL;
    t->depth = depth;
    t->children = NULL;

    return t;
}

/**
 *  \fn static int computeMaxDepth(int w)
 *  \brief Computes the max depth of a QTree based on the size of the map
 *  \param w the map width
 *  \return The maximum depth of the tree
*/
static int computeMaxDepth(int w)
{
    int maxDepth = 0, nb_sub = 0;

    nb_sub = w / MIN_CHUNK_SIZE;

    maxDepth = (int)(log(nb_sub)/log(2));

    return maxDepth;
}

QTree *buildTree(int depth, int w)
{
    static int maxDepth = 0;
    int i;

    QTree *t = NULL;

    t = initQTree(depth);
    t->depth = depth;

    if(depth == 0)
    {
        maxDepth = computeMaxDepth(w);

        /* The map will always be a square so h=w*/
        t->data = initMapChunkWithParams(w, w, 0);

        if(t->data == NULL)
        {
            fprintf(stderr, "Error when allocating data of root in %s, at line %d\n",
                __FILE__, __LINE__);
            return NULL;
        }
    }
    else
    {
        t->data = initMapChunkWithParams(w, w, depth);

        if(t->data == NULL)
        {
            fprintf(stderr, "Error when allocating data of child in %s, at line %d\n",
                __FILE__, __LINE__);
            return NULL;
        }
    }

    if(depth < maxDepth)
    {
        t->children = (QTree **) calloc(NB_CHILDREN, sizeof(QTree *));

        if(t->children == NULL)
        {
            fprintf(stderr, "Error when allocating children in %s, at line %d\n",
                __FILE__, __LINE__);
            return NULL;
        }

        for(i = 0; i < NB_CHILDREN; i++)
        {
            t->children[i] = buildTree(depth+1, w);
        }
    }

    else
        t->children = NULL;

    return t;
}

int isLeaf(QTree *t)
{
    return t->children == NULL;
}

/**
 * \fn int fillRootFromFile(QTree *t, FILE *f)
 * \brief Reads the 2D array of the map from the file and fills the tree accordingly
 * \param t a pointer on the tree representing the map
 * \param f a pointer on the file to read from
 * \return 0 if everything went fine, -1 otherwise
*/
static int fillRootFromFile(QTree *t, FILE *f)
{
    int i, j;
    char buff;

    MatrixData *md = (MatrixData *) (t->data->nd);

    if(md == NULL)
    {
        fprintf(stderr, "Error when accessing ground layer in file %s, at line %d\n",
                __FILE__, __LINE__);
        return -1;
    }

    for(i = 0; i < md->h; i++)
    {
        for(j = 0; j < md->w; j++)
        {
            fscanf(f, "%c", &buff);
            md->ground_layer[i][j]=buff-'0';
        }
    }

    return 0;
}

/**
 *  \fn static void setChildrenNodeCode(QTree *t, int depth, char child_nb)
 *  \brief Computes the children code recursively
 *  \param t the node for which we compute the code
 *  \param depth the depth of the node
 *  \param child_nb the number corresponding to the position of the node
            relatively to its parent
 *  \param code_p the code of the parent
 */
static void setChildrenNodeCode(QTree *t, int depth, char child_nb, char *code_p)
{
    char *code = NULL;

    if(depth > 0)
        code = (char *)(t->data->nd);


    if(depth > 0)
    {
        /* if the parent of the node is not the root we copy its code*/
        if(depth > 1)
        {
            int i = 0;
            for(i=0; i < depth-1; i++)
            {
                code[i] = code_p[i];
            }
        }

        /* Then we can add the next number of the code */
        code[depth-1] = child_nb;
    }

    if(!isLeaf(t))
    {
        int i = 0;
        for(i=0; i < NB_CHILDREN; i++)
        {
            setChildrenNodeCode(t->children[i], depth+1, i, code);
        }
    }
}

SDL_Rect computeAreaFromCode(QTree *t, int map_width)
{
    SDL_Rect area;
    area.x = area.y = 0;

    area.w = area.h = map_width >> t->depth;

    if(t->depth != 0)
    {
        int i = 0;
        int dx = 0, dy = 0;
        char *code = (char *) t->data->nd;

        for(i=0; i < t->depth; i++)
        {
            dx = (int)(code[i]) >> 1;
            dy = (int)(code[i]) & 1;

            area.x += dx * (map_width >> t->depth);
            area.y += dy * (map_width >> t->depth);
        }
    }

    return area;
}

void fillTreeFromFile(QTree *t, FILE *f)
{
    if(t == NULL || f == NULL)
        exit(EXIT_FAILURE);

    fillRootFromFile(t, f);

    /* These parameters are used only when the function is called on the root.
        This is arbitrary as the two last parameters are not taken into acount
        for the root
    */
    setChildrenNodeCode(t, 0, -1, NULL);
}

void freeQTree(QTree *t)
{
    if(t)
    {
        if(t->data)
            freeMapChunk(t->data, t->depth);

        if(t->children)
        {
            int i =0;
            for(i=0; i < NB_CHILDREN; i++)
            {
                freeQTree(t->children[i]);
            }

            free(t->children);
        }

        free(t);
    }
}

/**
 *  \fn static void printCode(char *code, int depth)
 *  \brief Prints the code of a node
 *  \param code the code to print
 *  \param depth the depth of the node
 */
static void printCode(char *code, int depth)
{
    int i = 0;
    for(i=0; i < depth; i++)
        printf("\t");

    for(i=0; i < depth; i++)
        printf("%d", code[i]);

    if(depth == 0)
        printf("R");
    printf("\n");
}

/**
 *  \fn static void displayTreeRec(QTree *t, int depth)
 *  \brief Displays a tree recursively
 *  \param t the QTree to display
 *  \param depth the depth of the tree to display
 */
static void displayTreeRec(QTree *t, int depth)
{
    char *codep = NULL;

    if(depth == 0)
        codep = "R";
    else
        codep = (char *) (t->data->nd);

    if(t->children)
    {
        displayTreeRec(t->children[3], depth+1);
        displayTreeRec(t->children[2], depth+1);
        printCode(codep, depth);
        displayTreeRec(t->children[1], depth+1);
        displayTreeRec(t->children[0], depth+1);
        printf("\n");
    }

    else
        printCode(codep, depth);
}

void displayTree(QTree *t)
{
    if(t)
    {
        printf("Displaying tree : \n");
        printf("********************\n\n");

        displayTreeRec(t, 0);

        printf("\n");
    } else
    {
        printf("No tree to display !\n");
    }

}
