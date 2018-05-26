/**
 * \file map.c
 * \brief Declaration of GameMap functions
 * \author Léo Gauvin
 * \version 1.0
 * \date February, 26th 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "files.h"
#include "tile.h"
#include "qtree.h"

GameMap *initEmptyMap()
{
    GameMap *m = (GameMap *) calloc(1, sizeof(GameMap));

    if(m == NULL)
    {
        fprintf(stderr, "Error when allocating map in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    m->map_name = NULL;
    m->nb_tilesets = 0;
    m->tlst_filename = NULL;
    m->tlst = NULL;
    m->tlst_offset = NULL;
    m->world_map = NULL;
    m->nb_rsrc = 0;
    m->rsrc = NULL;

    return m;
}

/**
 *  \fn static void initTilesetOffset(GameMap *m)
 *  \brief Sets the offset for each tileset according to the number of tiles in them
 *  \param m the GameMap containing the tilesets
 */
static void initTilesetOffset(GameMap *m)
{
    Tileset **t = m->tlst;

    if(t)
    {
        int i = 0;
        for(i=0; i < m->nb_tilesets; i++)
        {
            if(i == 0)
                m->tlst_offset[i] = 0;
            else
            {
                int col_p = 0, row_p = 0;
                col_p = m->tlst[i-1]->nb_col;
                row_p = m->tlst[i-1]->nb_row;

                m->tlst_offset[i] = m->tlst_offset[i-1] + col_p * row_p;
            }
        }
    }
}

GameMap *initEmptyMapWithParams(char *map_name, int nb_tlst, char **tlst_filename, int w)
{
    GameMap *m = (GameMap *) calloc(1, sizeof(GameMap));
    int i;

    if(m == NULL)
    {
        fprintf(stderr, "Errror when allocating map in %s, at line %d\n",
                __FILE__, __LINE__);
        return NULL;
    }

    m->map_name = NULL;
    m->tlst_filename = NULL;
    m->tlst = NULL;

    if(map_name)
    {
        m->map_name = (char *) calloc(strlen(map_name) + 1, sizeof(char));
        if(m->map_name == NULL)
        {
            fprintf(stderr, "error when allocating string in %s, at line %d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        strcpy(m->map_name, map_name);
    }

    m->nb_tilesets = nb_tlst;

    if(tlst_filename)
    {
        m->tlst = (Tileset **) calloc(nb_tlst, sizeof(Tileset *));
        if(m->tlst== NULL)
        {
            fprintf(stderr, "error when allocating tileset array in %s, at line %d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }


        m->tlst_filename = (char **) calloc(nb_tlst, sizeof(char *));
        if(m->tlst_filename == NULL)
        {
            fprintf(stderr, "error when allocating string array in %s, at line %d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }

        m->tlst_offset = (int *) calloc(nb_tlst, sizeof(int));

        if(m->tlst_offset == NULL)
        {
            fprintf(stderr, "error when allocating offset array in %s, at line %d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }

        for(i=0; i < m->nb_tilesets; i++)
        {
            m->tlst_filename[i] = (char *) calloc(strlen(tlst_filename[i]) + 1,
                                                  sizeof(char));

            if(m->tlst_filename[i] == NULL)
            {
                fprintf(stderr, "error when allocating string in %s, at line %d\n",
                        __FILE__, __LINE__);
                exit(EXIT_FAILURE);
            }

            strcpy(m->tlst_filename[i], tlst_filename[i]);

            m->tlst[i] = loadTilesetFromFile(m->tlst_filename[i]);

        }

        initTilesetOffset(m);
    }

    m->world_map = buildTree(0, w);


    if(m->world_map == NULL)
    {
        fprintf(stderr, "error when allocating world map in %s, at line %d\n",
                    __FILE__, __LINE__);
            exit(EXIT_FAILURE);
    }

    m->nb_rsrc = 0;
    m->rsrc = NULL;

    return m;
}

/**
 *  \fn static void initTilesetsFromFile(GameMap *m, FILE *f)
 *  \brief Reads tilesets data from a file and fills the corresponding GameMap fields
 *  \param m the GameMap containing the tilesets
 *  \param f a pointer on the map file
 */
static void initTilesetsFromFile(GameMap *m, FILE *f)
{
    int nb_tlst;
    int i;
    char tileset_filename[MAX_FILENAME] = "";

    /* Reading the number of tilesets */
    fscanf(f, "%d\n", &nb_tlst);
    m->nb_tilesets = nb_tlst;


    m->tlst_filename = (char **) calloc(nb_tlst, sizeof(char *));

    if(m->tlst_filename == NULL)
    {
        fprintf(stderr, "Error when allocating tileset filename array in file %s, at line %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    m->tlst = (Tileset **) calloc(nb_tlst, sizeof(Tileset *));

    if(m->tlst == NULL)
    {
        fprintf(stderr, "Error when allocating tileset array in file %s, at line %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    m->tlst_offset = (int *) calloc(nb_tlst, sizeof(int));

    if(m->tlst_offset == NULL)
    {
        fprintf(stderr, "Error when allocating tileset offset array in file %s, at line %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    for(i=0; i < nb_tlst; i++)
    {
        fscanf(f, "%s\n", tileset_filename);
        m->tlst_filename[i] = (char *) calloc(1, (strlen(tileset_filename)+1)* sizeof(char));
        strcpy(m->tlst_filename[i], tileset_filename);

        m->tlst[i] = loadTilesetFromFile(tileset_filename);
    }

    initTilesetOffset(m);
}

static void initRessourceData(GameMap *m, FILE *f)
{
    int i;
    fscanf(f, "%d\n", &(m->nb_rsrc));

    m->rsrc = (int **) calloc(m->nb_rsrc, sizeof(int *));

    if(m->rsrc == NULL)
    {
        fprintf(stderr, "Error when allocating ressource array in file %s, at line %d\n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    for(i=0; i < m->nb_rsrc; i++)
    {
        m->rsrc[i] = (int *) calloc(3, sizeof(int));

        fscanf(f, "%d %d %d\n", &(m->rsrc[i][0]),
                                &(m->rsrc[i][1]),
                                &(m->rsrc[i][2]));
    }
}

GameMap *loadFromTextFile(GameMap *m, char *filename)
{
    FILE *f = NULL;
    char mName[MAX_FILENAME]="";
    int w_map = 0;

    if(m == NULL)
        m = initEmptyMap();

    f = openFile(filename, "r");

    if(!f)
        return NULL;

    fscanf(f, "%s\n", mName);
    m->map_name = (char *) malloc((strlen(mName)+1)* sizeof(char));
    strcpy(m->map_name, mName);

    initTilesetsFromFile(m, f);

    fscanf(f,"%d\n", &w_map);

    m->world_map = buildTree(0, w_map);

    if(m->world_map == NULL)
    {
        fprintf(stderr, "error when allocating world map in %s, at line %d\n",
                    __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    fillTreeFromFile(m->world_map, f);

    fscanf(f, "\n");

    initRessourceData(m, f);

    closeFile(f);

    return m;
}

/**
 * \fn static void writeMapMatrix(GameMap *m, FILE *f)
 * \brief Writes 2D array of the map in a file
 * \param m a pointer on the map to fill
 * \param f a pointer on the file to read from
 * \return 0 if everything went fine, -1 otherwise
*/
static void writeMapMatrix(GameMap *m, FILE *f)
{
    MatrixData *md = (MatrixData *) m->world_map->data->nd;
    int i, j;

    fprintf(f, "%d\n", md->w);

    for(i = 0; i < md->h; i++)
    {
        for(j = 0; j < md->w; j++)
        {
            fprintf(f, "%d", md->ground_layer[i][j]);
        }
    }
}

int saveToTextFile(GameMap *m, char *filename)
{
    FILE *f = NULL;
    int i;

    f = openFile(filename, "w");

    if(!f)
        return -1;
    if(!m->map_name || !m->tlst_filename)
        return -1;

    fprintf(f, "%s\n", m->map_name);
    fprintf(f, "%d\n", m->nb_tilesets);

    for(i=0; i < m->nb_tilesets; i++)
    {
        fprintf(f, "%s\n", m->tlst_filename[i]);
    }

    writeMapMatrix(m, f);

    fprintf(f, "\n%d", m->nb_rsrc);

    for(i=0; i < m->nb_rsrc; i++)
    {
        fprintf(f, "%d %d %d\n", m->rsrc[i][0],
                                 m->rsrc[i][1],
                                 m->rsrc[i][2]);
    }

    closeFile(f);

    return 0;
}

void freeMap(GameMap *m)
{
    if(m)
    {
        if(m->map_name)
            free(m->map_name);

        if(m->tlst_filename)
        {
            int i = 0;
            for(i=0; i < m->nb_tilesets; i++)
            {
                free(m->tlst_filename[i]);
            }
            free(m->tlst_filename);
        }

        if(m->tlst)
        {
            int i = 0;
            for(i=0; i < m->nb_tilesets; i++)
            {
                freeTileset(m->tlst[i]);
            }
            free(m->tlst);
        }

        if(m->tlst_offset)
            free(m->tlst_offset);

        if(m->world_map)
        {
            freeQTree(m->world_map);
        }

        free(m);

    }
}

void displayMap(GameMap *m)
{
    int i, j;
    MatrixData *md = NULL;

    printf("*** Displaying %s ***\n", m->map_name);
    printf("%d tilesets :\n", m->nb_tilesets);

    for(i=0; i < m->nb_tilesets; i++)
    {
        printf("%s\n", m->tlst_filename[i]);
    }

    printf("Ground matrix : \n");

    md = (MatrixData *) m->world_map->data->nd;

    printf("Nb of cols : %d\n", md->w);
    printf("Nb of rows : %d\n", md->h);

    for(i=0; i < md->h; i++)
    {
        for(j=0; j < md->w; j++)
        {
            printf("%d ", md->ground_layer[j][i]);
        }
        printf("\n");
    }

    printf("\n\n");
    printf("Object matrix : \n");

    printf("********************************\n");

    displayTree(m->world_map);

    printf("\n\nNb of ressources : %d\n", m->nb_rsrc);
    for(i=0; i < m->nb_rsrc; i++)
    {
        printf("%d %d %d\n", m->rsrc[i][0],
                             m->rsrc[i][1],
                             m->rsrc[i][2]);
    }

    printf("\n");
}
