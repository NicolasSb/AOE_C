/**
 * \file map_chunk.h
 * \brief Declaration of MapChunk used to store matrix data / code in QTree
 * \author Léo Gauvin
 * \version 1.0
 * \date March, 28th 2017
 */

#ifndef MAP_CHUNK_H_INCLUDED
#define MAP_CHUNK_H_INCLUDED

#include "list.h"

#define MIN_CHUNK_SIZE  8

 /*In order to handle collision, ground tile are split in two
The resulting chunk are thus four times smaller*/
#define SUBDIVISION_COLISION    4

/**
 *  \struct MatrixData
 *  \brief Structure containing ground layer info and collision map
 */
typedef struct MatrixData MatrixData;
struct MatrixData {
    int w;                  /*!< Width of the chunk */
    int h;                  /*!< Height of the chunk */

    char **ground_layer;    /*!< 2D array containing ground layer */
    char **collision_map;   /*!< 2D array containing the non-walkable zones */
};

/**
 *  \fn MatrixData *initMatrixData()
 *  \brief Allocates space for an empty MatrixData
 *          All values are set to 0 or \e NULL
 *  \return A pointer on a MatrixData
 */
MatrixData *initMatrixData();

/**
 *  \fn MatrixData *initEmptyMatrixDataWithParams(int w, int h)
 *  \brief Allocates space for the data of a w*h map
 *  \param w the width of the map
 *  \param h the height of the map
 *  \return A pointer on the MatrixData
 */
MatrixData *initEmptyMatrixDataWithParams(int w, int h);


/**
 *  \fn void freeMatrixData(MatrixData *md)
 *  \brief Frees the MatrixData after use
 *  \param md the MatrixData to free
 */
void freeMatrixData(MatrixData *md);


/**
 *  \struct MapChunk
 *  \brief Chunk of the whole map (can also be the whole map for the first chunk).
 *         Objects have been split in two groups in order to improve the generation
 *           of the collision map.
 *         A pointer on MatrixData is used in order not to overload the structure
            as only the root of the tree will contain matrix data.
 */
typedef struct MapChunk MapChunk;
struct MapChunk {
    void *nd;               /*!< Node data necessary data to get matrix information */

    List *still_obj;        /*!< List of all the 2nd layer still objects */
    List *moving_obj;       /*!< List of all the 2nd layer moving objects */
};

/**
 *  \fn MapChunk *initMapChunkWithParams(int w, int h)
 *  \brief Allocates space for a map chunk using parameters
 *  \param w the width of the map
 *  \param h the height of the map
 *  \param depth the depth of the node containing the MapChunk
 *  \return A pointer on the MapChunk
*/
MapChunk *initMapChunkWithParams(int w, int h, int depth);

/**
 *  \fn void updateCollisionMap(MapChunk *m)
 *  \brief Generates the collision map from the layers
 *  \param m the map chunk for which we want the collision map
 *  \todo Implement the function
 */
void updateCollisionMap(MapChunk *m);

/**
 * \fn void freeMapChunk(MapChunk *m)
 * \brief Frees the map chunk after use
 * \param m the map chunk to free
 * \param depth the depth of the node containing the MapChunk
 */
void freeMapChunk(MapChunk *m, int depth);

#endif
