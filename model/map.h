/**
 * \file map.h
 * \brief World Map for the game which is edited in the editor
 * \author Léo Gauvin
 * \version 1.0
 * \date February, 26th 2017
 *
 * World map definition
 *
 */

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "tile.h"
#include "qtree.h"

/**
 * \struct GameMap
 * \brief Map of the game
 * \todo Add a second layer for buildings and resources
 * \todo Modify the structure and functions so that it uses a QTree to store the map
 */
typedef struct GameMap GameMap;
struct GameMap
{

    char *map_name;         /*!< Name of the map */
    int nb_tilesets;        /*!< The number of tilesets used for the map */
    char **tlst_filename;   /*!< Names of the tileset files */
    Tileset **tlst;         /*!< Tilesets to use */
    int *tlst_offset;       /*!< Array containing offset for each tileset */

    QTree *world_map;       /*!< World map of the game with ground and objects */

    int nb_rsrc;            /*!< Size of ressource data array */
    int **rsrc;            /*!< Array of ressource data */
};

/**
 * \fn GameMap *initEmptyMap()
 * \brief Allocates space for an empty map.
 *        All attributes are set to 0 or \e NULL
 * \return A pointer on the map
*/
GameMap *initEmptyMap();

/**
 *  \fn GameMap *initEmptyMapWithParams(char *map_name, char *tileset, int w, int h)
 *  \brief Allocates space for a map of size w*h
 *  \param map_name the name of the map
 *  \param nb_tlst the number of tilesets used
 *  \param tlst_filename the names of the tileset files
 *  \param w the width of the map
 *  \return a pointer on the map
*/
GameMap *initEmptyMapWithParams(char *map_name, int nb_tlst, char **tlst_filename, int w);

/**
 * \fn GameMap *loadFromTextFile(GameMap *m, char *filename)
 * \brief Loads a map from a text file.
 * The file used contains all attributes of the \a GameMap structure in their order of appearance
 * \param m a pointer on the map which will be filled
 * \param filename  the name of the file describing the map
 * \return a pointer on the loaded map
*/
GameMap *loadFromTextFile(GameMap *m, char *filename);

/**
 * \fn int saveToTextFile(GameMap *m, char *filename)
 * \brief Saves a map to a text file.
 * The file used will contain all "text-mode writable" attributes of the \a GameMap structure in their order of appearance
 * \param m a pointer on the map which will be filled
 * \param filename  the name of the file describing the map
 * \return 0 if everything went fine, -1 otherwise
*/
int saveToTextFile(GameMap *m, char *filename);

/**
 * \fn void freeMap(GameMap *m)
 * \brief Frees the map after use
 * \param m a pointer on the map to free
*/
void freeMap(GameMap *m);

/**
 * \fn void displayMap(GameMap *m)
 * \brief Displays the map data in command line.
 * This will be used mainly for debugging and in the first stage of the project
 * \param m a pointer on the map to display
*/
void displayMap(GameMap *m);


#endif
