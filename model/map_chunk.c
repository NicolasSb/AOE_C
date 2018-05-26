/**
 * \file map_chunk.c
 * \brief Definition of MapChunk functions
 * \author Léo Gauvin
 * \version 1.0
 * \date March, 28th 2017
 */

#include <stdlib.h>
#include <stdio.h>

#include "map_chunk.h"

MatrixData *initMatrixData()
{
    MatrixData *md = (MatrixData *) calloc(1, sizeof(MatrixData));

    if(md == NULL)
    {
        fprintf(stderr, "Error when allocating matrix data in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    md->w = 0;
    md->h = 0;

    md->ground_layer = NULL;
    md->collision_map = NULL;

    return md;
}

MatrixData *initEmptyMatrixDataWithParams(int w, int h)
{
    int i;
    MatrixData *md = (MatrixData *) calloc(1, sizeof(MatrixData));

    if(md == NULL)
    {
        fprintf(stderr, "Error when allocating matrix data in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    if(w <= 0 && h <= 0)
    {
        fprintf(stderr, "Wrong parameters : map size cannot be negative in %s, at line %d\n",
                __FILE__, __LINE__);
        freeMatrixData(md);
        return NULL;
    }
    md->w = w;
    md->h = h;

    /* Ground layer allocation */
    md->ground_layer = (char **) calloc(md->h, sizeof(char *));

    if(md->ground_layer == NULL)
    {
        fprintf(stderr, "Error when allocating ground matrix in %s, at line %d\n",
                __FILE__, __LINE__);
        freeMatrixData(md);
        return NULL;
    }

    for(i=0; i < md->h; i++)
    {
        md->ground_layer[i] = (char *) calloc(md->w, sizeof(char));
    }

    /* Collision map allocation */
    md->collision_map = (char **) calloc(md->h * SUBDIVISION_COLISION,
                                         sizeof(char *));

    if(md->collision_map == NULL)
    {
        fprintf(stderr, "Error when allocating collision matrix in %s, at line %d\n",
                __FILE__, __LINE__);
        freeMatrixData(md);
        return NULL;
    }

    for(i=0; i < md->h * SUBDIVISION_COLISION / 2; i++)
    {
        md->collision_map[i] = (char *) calloc(md->w * SUBDIVISION_COLISION / 2,
                                               sizeof(char));
    }

    return md;
}

void freeMatrixData(MatrixData *md)
{
    if(md)
    {
        if(md->ground_layer)
        {
            int i = 0;
            for(i=0; i < md->h; i++)
            {
                if(md->ground_layer[i])
                    free(md->ground_layer[i]);
            }

            free(md->ground_layer);
        }

        if(md->collision_map)
        {
            int i = 0;
            for(i=0; i < md->h * SUBDIVISION_COLISION; i++)
            {
                if(md->collision_map[i])
                    free(md->collision_map[i]);
            }

            free(md->collision_map);
        }

        free(md);
    }
}


static void *initNodeData(int w, int h, int depth)
{
    void *NodeData = NULL;

    if(depth == 0)
        NodeData = (MatrixData *) initEmptyMatrixDataWithParams(w, h);
    else
        NodeData = (char *) calloc(depth, sizeof(char));

    if(NodeData == NULL)
    {
        fprintf(stderr, "Error when allocating NodeData in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    return NodeData;
}

static void freeNodeData(void *nd, int depth)
{
    if(nd)
    {
        if(depth == 0)
            freeMatrixData(nd);

        else
            free(nd);
    }
}

MapChunk *initMapChunkWithParams(int w, int h, int depth)
{
    MapChunk *m = (MapChunk *) calloc(1, sizeof(MapChunk));

    if(m == NULL)
    {
        fprintf(stderr, "Error when allocating map chunk in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    m->still_obj = NULL;
    m->moving_obj = NULL;

    m->nd = initNodeData(w, h, depth);

    if(m->nd == NULL)
    {
        fprintf(stderr, "Error when allocating node data in %s, at line %d\n",
                __FILE__, __LINE__);
        freeMapChunk(m, depth);
        return NULL;
    }

    if(depth == 0)
        m->nd = (MatrixData *) (m->nd);
    else
        m->nd = (char *) (m->nd);

    return m;
}

void freeMapChunk(MapChunk *m, int depth)
{
    if(m)
    {
        if(m->nd)
        {
            freeNodeData(m->nd, depth);
        }

        if(m->still_obj)
            freeList(m->still_obj);

        if(m->moving_obj)
            freeList(m->moving_obj);

        free(m);
    }
}
