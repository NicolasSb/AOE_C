/**
 *  \file files.c
 *  \brief Declarations of functions for handling files
 *  \author Léo Gauvin
 *  \version 1.0
 *  \date February, 10th 2017
 */

#include "files.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <SDL.h>
#include <SDL_image.h>


void emptyBuffer(void)
{
  int c;
  while((c=getchar()) != EOF && c != '\n');
}


int fileExists(const char *fname)
{
    struct stat buffer;   
    return (stat (fname, &buffer) == 0); 
}


FILE *openFile(char *filename, char *mode)
{
    FILE *f = NULL;
    errno = 0;
    if(filename == NULL || mode == NULL)
    {
        fprintf(stderr, "No filename and/or mode for opening file at line %d, in %s\n",
                __LINE__, __FILE__);
        return NULL;
    }

    f = fopen(filename, mode);

    if(!f)
    {
        fprintf(stderr, "Opening of file \"%s\" failed in %s, line %d\n",
                filename, __FILE__, __LINE__);
        perror("openFile");
        return NULL;
    }

    return f;
}

void closeFile(FILE *f)
{
    fclose(f);
}


SDL_Surface *loadImage(char *filename)
{
	SDL_Surface *loadedImage = NULL;

    loadedImage = IMG_Load(filename);

    if(loadedImage == NULL)
    {
        fprintf(stderr, "Loading image \"%s\" failed in %s, line %d\n",
                filename, __FILE__, __LINE__);
        return NULL;
    }

    return loadedImage;
}

