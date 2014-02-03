#include "SDL/SDL_ttf.h"
//#include "plot.h"


#define DEBUG 1 // general debug output.


//The CONSTANT initial size of the screen
#define DEFAULT_SCREEN_WIDTH 896
#define DEFAULT_SCREEN_HEIGHT 576
//The DYNAMIC  initial size of the screen
unsigned int SCREEN_WIDTH;
unsigned int SCREEN_HEIGHT;


#define SCREEN_BPP 32

// global variable that tells you the FPS of the game.
int FPS; // arbitrary initial value

// The surfaces that will be used
SDL_Surface *screen;;	// this is the surface the player sees.
// The event structure that will be used
SDL_Event event;

// the fint that will be used
TTF_Font *font22;
TTF_Font *font16;

//struct plotData myPlot;
