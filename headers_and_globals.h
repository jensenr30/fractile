#include <stdio.h>
#include "Windows.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "time.h"
#include <math.h>


#define DEBUG 1 // general debug output.

//allowing us to use the bool type. you might want to remove this definition if you are going to try to compile this project as a C++
#define bool char
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1

//The CONSTANT initial size of the screen
#define DEFAULT_SCREEN_WIDTH 896
#define DEFAULT_SCREEN_HEIGHT 576
//The DYNAMIC  initial size of the screen
unsigned int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
unsigned int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;


#define SCREEN_BPP 32

// global variable that tells you the FPS of the game.
int FPS = 404; // arbitrary initial value

// The surfaces that will be used
SDL_Surface *screen = 		NULL;	// this is the surface the player sees.
// The event structure that will be used
SDL_Event event;

// the fint that will be used
TTF_Font *font22 = NULL;
TTF_Font *font16=NULL;

void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination );
void apply_surface_clips( int x, int y,  SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip );
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void clean_up();

// include all of the other header files that have proprietary functions and variables that will be used throughout the program.
#include "general_functions.h"
#include "graphics.h"



