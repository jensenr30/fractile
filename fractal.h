/*

fractal.h
Ryan Jensen
2015

The purpose of this file is to draw fractals.
This is accomplished with the SDL2 graphics library.
The "graphics.c" and "graphics.h" files are utilized in order to draw lines, shapes, and other things on the screen.
These lines and shapes are the building blocks of the fractals drawn here.


Contents:
	- fractal structure implementations
	- fractal function prototypes
	- fractal #definitions
*/



//-------------------------------------------------------
// header files
//-------------------------------------------------------

// need this for integer types
#include <inttypes.h>
// I wish I didn't have to include this here, but I need it because SDL surfaces are passed through fractal functions.
#include "SDL2/SDL.h"


//-------------------------------------------------------
//definitions
//-------------------------------------------------------

// this is the current version of the program
#define FRACTAL_VERSION				((float)1.0)
// this is the maximum number of shapes that can exist in a fractal
#define FRACTAL_MAX_SHAPES			20
// this specifies how many "children" a fractal can have.
// children are the points that will be used as the origins when rendering he NEXT iteration of the fractal.
// consequently, this is how many copies 
#define FRACTAL_MAX_CHILDREN		20
// this is the number of <x,y> pairs each fractal shape has.
// currently, there are 3 (just enough to specify triangles)
#define FRACTAL_MAX_SHAPE_POINTS 	3

// this is how many iterations fractals will have by default
#define FRACTAL_DEFAULT_ITERATIONS				3	
#define FRACTAL_DEFAULT_CHILDREN				20		
#define FRACTAL_DEFAULT_SHAPES					1			
#define FRACTAL_DEFAULT_ZOOM					1		
#define FRACTAL_DEFAULT_SCALE					0.707107	//sqrt(0.5)
#define FRACTAL_DEFAULT_TWIST					0			// degrees
#define FRACTAL_DEFAULT_CHILDREN_TWIST			45//9.53	
#define FRACTAL_DEFAULT_ITERATIONS_CHILDREN		1		

// this is how big the circle is that is displayed where children are
#define FRACTAL_CHILDREN_DISPLAY_RADIUS			3		


// this is how far away (in pixels) a user's click has to be from an object for it to count as being selected
#define FRACTAL_SELECT_UNSELECTED -1
#define FRACTAL_SELECT_PIXEL_DISTANCE 13

/// these are all of the fractal shape types (fst = fractal shape type)
// they tell the renderer how to interpret the <x,y> data of the fractal shape.
#define fst_line			0	// this draws a line between <x[0],y[0]> and <x[1],y[1]>
#define fst_pixel			1	// this draws a pixel at <x[0],y[0]>
#define fst_circle			2	// this draws a circle with center <x[0],y[0]> and a radius of the [radius] variable
#define fst_tri				3	// this uses <x[0],y[0]>, <x[1],y[1]>, and <x[2],y[2]> as points to specify a triangle.
#define fst_rect			4	// this uses <x[0],y[0]> and <x[1],y[1]> as opposite vertices of a rectangle

#define fst_TOP				4	// the highest valid fractal shape type (the last one). This is used when randomly selecting a shape type.


/// these are all of the ways a fractal shape can be filled (fsr = fractal shape fill)

#define fsf_full			0	// this will fill the entire shape. This is the most "normal" or "plain" setting.
#define fsf_outline			1	// this will only draw the outline of a shape
#define fsf_diag			2	// this fills shapes with a diagonal checkerboard pattern
#define fsf_rand			3	// this will randomly fill pixels in the shape
#define fsf_plaid			4	// this will fill the shape with a plaid pattern

#define fsf_TOP				4  // this is the highest valid number for fractal shape fill



//-------------------------------------------------------
// structure declarations
//-------------------------------------------------------

/// this is an <x,y> pair 
struct fractalChild
{
	// this is the x position of the child with respect to its parent
	float x;
	// this is the y position of the child with respect to its parent
	float y;
	
	// this is a number (in degrees) that specifies how much this child is rotated with respect to its parent.
	float twist;
	// this describes the size of this child is with respect to their parents.
	float scale;
	
};


/// this is a single shape in a fractal
struct fractalShape
{
	
	// each shape has some number of x,y pairs
	float x[FRACTAL_MAX_SHAPE_POINTS];
	float y[FRACTAL_MAX_SHAPE_POINTS];
	
	// for a line, this might be the thickness of the line. for a circle, this would be its radius
	float radius;
	
	
	// this specifies the type of shape this is
	unsigned char type;
	// this specifies how the shape is to be rendered
	unsigned char fillType;
	// this is a modifier of the fillType.
	// this tells us how dense the fill is for this shape
	unsigned char fillPercent;
	
	// this is the color of the shape (in the format 0xAARRGGBB)
	// highest 8 bits: 			Alpha (opacity)
	// second highest 8 bits:	Red
	// second lowest 8 bits:	Green
	// lowest 8 bits:			Blue
	uint32_t color;
	
};


/// this is a fractal containing a number of shapes
struct fractal
{
	
	// these are all the shapes that make up the fractal
	struct fractalShape shapes[FRACTAL_MAX_SHAPES];
	// this tells us how many shapes the fractal has.
	unsigned int numberOfShapes;
	
	// these are the exits of the fractal (the points that will be used as the origin for the next iteration of this fractal.
	// this is just an <x,y> pair
	struct fractalChild children[FRACTAL_MAX_CHILDREN];
	// this tells us how many children the fractal has.
	unsigned int numberOfChildren;
	
	// this is the number of iterations the fractal is to be evaluated.
	float iterations;
	// this is how many children iterations are rendered
	float iterationsChildren;
	
	// this is how zoomed in/out we are when rendering the fractal.
	float zoom;
	// this is how much the entire fractal is twisted
	float twist;
	// these are the position of the origin of the fractal on SDL_Surface the fractal is rendered to
	// these are in pixel space (screen space)
	float x, y;
	
	// this keeps track of what part of the fractal is being selected
	struct fractalSelect select;
	
};



/// this structure is used to store which component of a fractal is selected and being graphically adjusted (manually) by the user
// this keeps track of what part(s) of a fractal is being played with
// this structure is built in such a way as to allow the user to select multiple parts of a fractal at one time.
struct fractalSelect
{
	// this tells us which xy pair the user has selected (if any)
	// 0 = unselected, 1 = selected
	uint8_t shapePoints[FRACTAL_MAX_SHAPES][FRACTAL_MAX_SHAPE_POINTS];
	// this tells if the radius of a shape was selected
	// 0 = unselected, 1 = selected
	uint8_t shapeRadius[FRACTAL_MAX_SHAPES];
	
	// this tells us which child the user has selected
	// 0 = unselected, 1 = selected
	uint8_t child[FRACTAL_MAX_CHILDREN];
};



//-------------------------------------------------------
// function prototypes
//-------------------------------------------------------

void fractal_render(struct fractal *frac, SDL_Surface *dest);

void fractal_render_iteration(struct fractal *frac, SDL_Surface *dest, int iteration, float x, float y, float scale, float twist);

void twist_xy(float x, float y, float twist, float *x_ret, float *y_ret);

void fractal_render_children(struct fractal *frac, SDL_Surface *dest, int iterations);



void fractal_set_default(struct fractal *frac);

