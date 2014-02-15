#include "bool.h"

struct vectorData{
	// magnitude of each component
	double x, y;
	// offsets from the origin
	double x0, y0;
	
};

struct pointData{
	int x, y;
};

// this is the maximum number of individual fractals that can be added to the base iteration.
// when the program DRAWS a fractal, it will end up drawing a LOT MORE than this number.
// this number just describes how many different vectors there can be on the base vector (the starting one)
#define MAX_VECTORS_PER_FRACTAL 100
// this defines how many exit points the fractal can have. and exit point is where the current level fractal will spawn the next level fractal.
#define MAX_EXITS_PER_FRACTAL 100

struct fractalData{
	
	// this is the main color of the fractal
	unsigned long int color1;
	// this is the secondary color of the fractal
	unsigned long int color2;
	
	// the initial thickness of the lines
	float thickness;
	
	
	// this is the fractal's array of vectors
	struct vectorData vects[MAX_VECTORS_PER_FRACTAL];
	// this is the array of the fractal's exit points
	struct pointData exits[MAX_EXITS_PER_FRACTAL];
	
	
	// this is the scaling factor for every iteration down the fractal does
	double scale;
	// this describes how quickly the color changes from the first iteration (at color1) to color2 (in later iterations)
	double colorScaler;
	
	
	// this is how many iterations of the fractal will be printed
	int iterations;
	
	
	// this is the number of exit points the fractal has
	int numbExits;
	// this is how many vectors there currently are in the fractal
	int numbVectors;
};



void fractal_iteration(SDL_Surface *dest, struct fractalData *f, double entryx, double entryy, double scale, int iter);
void fractal_print(SDL_Surface *dest, struct fractalData *fractal);
void fractal_random(struct fractalData *f, int maxVects, int maxIterations);



// the editor is a sidbar on the left side of the window.
#define EDITOR_DEFAULT_WIDTH 150
#define EDITOR_COLOR_PRIMARY 0xff2f2f2f
#define EDITOR_COLOR_SECONDARY 0xff3f3f3f
#define EDITOR_COLOR_SCROLL_BAR 0xff5f5f5f
#define EDITOR_COLOR_SCROLL_BAR_BACKGROUND EDITOR_COLOR_PRIMARY
#define EDITOR_BUTTON_SIZE 32
#define EDITOR_TITLE_BAR_HEIGHT 16
#define EDITOR_SCROLL_BAR_WIDTH 16

// these are enumerations for editor buttons.
#define EDITOR_BUTTON_VECTOR 0
#define EDITOR_BUTTON_OPTIONS 1
// this is how many buttons there are.
#define EDITOR_BUTTONS_NUMBER_OF 2

/// these are enumerations of editor events
#define ee_print 0	// just print the editor. the user isn't doing anything with it at this time.
#define ee_toggle 1		// this will toggle if the editor is open or not.




bool fractal_editor(SDL_Surface *dest, struct fractalData *f, int x, int y, int editorEvent);
void init_fractal_editor();



