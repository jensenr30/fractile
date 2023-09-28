#ifndef GLOBALS_H
#define GLOBALS_H

//#include <SDL2/SDL_ttf.h>
#define DEBUG 1
// global display stuff
//SDL_Window *myWindow;
//SDL_Renderer *myRenderer;
//SDL_Texture *myTexture;

typedef unsigned char byte;

unsigned int get_window_width(void);
unsigned int get_window_height(void);

int set_window_width(unsigned int width);
int set_window_height(unsigned int height);

#endif /* GLOBALS_H */
