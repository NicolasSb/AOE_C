/**
 *  \file files.h
 *  \brief Definitions for handling files
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date February, 5th 2017
 */

#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdio.h>
#include <SDL.h>

/**
 *  \def Defines the maximum length of the filename
*/
#define MAX_FILENAME    50

/**
 *  \fn FILE *openFile(char *filename, char *mode)
 *  \brief Opens a file given in parameter
 *  \param filename  the name of the file to open
 *  \param mode the mode with which to open the file
 *  \return a pointer on the file
 */
FILE *openFile(char *filename, char *mode);


/**
 * \fn void closeFile(FILE* f)
 * \brief Closes a file
 * \param f the file pointer to close or \e NULL if opening failed
 */
void closeFile(FILE* f);

/**
 *  \fn SDL_Surface *loadImage(char *filename)
 *  \brief Loads a surface with an image which path is given in parameter
 *  \param filename the name of the file to use
 *  \return a pointer on a SDL_Surface, ie the image
 */
SDL_Surface *loadImage(char *filename);


int fileExists(const char *fname);

void emptyBuffer(void);

#endif
