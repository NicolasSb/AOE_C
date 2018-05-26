/**
 * \file qtree.h
 * \brief Declaration of QTree used to store map data
 * \author Léo Gauvin
 * \version 1.0
 * \date March, 28th 2017
 */

#ifndef QTREE_H_INCLUDED
#define QTREE_H_INCLUDED

#include <stdio.h>
#include <SDL.h>

#include "map_chunk.h"

#define NB_CHILDREN 4

/**
 *  \struct QTree
 *  \brief 4-ary Tree used to split the map in more manageable chunks
 */
typedef struct QTree QTree;
struct QTree {
    MapChunk *data;         /*!< Data contained in the chunk */
    QTree **children;       /*!< Array containing the children QTree if any */
    int depth;              /*!< Depth of the QTree */
};

/**
 *  \fn QTree *initQTree()
 *  \brief Allocates space for a QTree.
            All attributes are set to 0 or \e NULL except depth
 *  \param depth the depth of the node
 *  \return A pointer on the QTree
 */
QTree *initQTree(int depth);

/**
 *  \fn QTree *buildTree(int w)
 *  \brief Allocates space for the QTree representing the map.
            All the children trees are allocated recursively.
    \param depth the depth of the node allocated
    \param w the width of the map
    \return A pointer on the QTree representing the map
 */
QTree *buildTree(int depth, int w);

/**
 *  \fn int isLeaf(QTree *t)
 *  \brief Tells whether the tree is a leaf or not
 *  \param t the QTree
 */
int isLeaf(QTree *t);

/**
 *  \fn int fillTreeFromFile(QTree *t, FILE *f)
 *  \brief Fills the tree completely from a file
 *  \param t the tree to fill
 *  \param f the file containing the data
 */
void fillTreeFromFile(QTree *t, FILE *f);

/**
 *  \fn SDL_Rect computeAreaFromCode(QTree *t, int map_width)
 *  \brief Computes the area corresponding to the code of a QTree
 *  \param t the tree which area is computed
 *  \param map_width the width of the map necessary for calculations
 *  \return A SDL_Rect corresponding to the area corresponding to the tree
 */
SDL_Rect computeAreaFromCode(QTree *t, int map_width);


/**
 *  \fn void freeQTree(QTree *t)
 *  \brief Frees the QTree after use
 *  \param t the QTree to free
 */
void freeQTree(QTree *t);

/**
 *  \fn void displayTree(QTree *t)
 *  \brief Displays the QTree on the command line
 *  \param t the QTree to display
 */
void displayTree(QTree *t);

#endif
