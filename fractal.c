#include "fractal.h"
#include "utilities.h"
#include "graphics.h"
#include <math.h>

/// this function will render a fractal to a destination SDL_Surface
// frac is the fractal to render
// dest is the destination SDL_Surface to render the fractal to
// x and y are the coordinates on the SDL surface (in units of pixels) where the origin of the fractal will be printed.
// x and y can be outside the bounds of the screen. That is absolutely fine and even encouraged. As long as some part of the fractal is still seen.
// scale specifies the scale to render everything at.
	// scale = 1:		one-to-one ratio fractal space to pixels
	// scale = 0.1: 	the image should be shrunk ten times (zoomed out)
	// scale = 10:		the fractal rendered should look very large (zoomed in)
// scale allows the user to zoom in/out
void fractal_render(struct fractal *frac, SDL_Surface *dest, float x, float y, float scale)
{
	
	// check for NULL pointers for frac
	if(frac == NULL)
	{
		error("fractal_render() was sent a NULL frac pointer");
		return;
	}
	// check for NULL pointers for dest
	if(dest == NULL)
	{
		error("fractal_render() was sent a NULL dest pointer");
		return;
	}
	// limit the possible number of shapes a fractal can have
	if(frac->numberOfShapes > FRACTAL_MAX_SHAPES)
	{
		error_d("fractal_render() was sent a frac with numberOfShapes > FRACTAL_MAX_SHAPES. This will be automatically corrected. numberOfShapes was equal to ",frac->numberOfShapes);
		frac->numberOfShapes = FRACTAL_MAX_SHAPES;
	}
	// limit the possible number of children a fractal can have
	if(frac->numberOfChildren > FRACTAL_MAX_CHILDREN)
	{
		error_d("fractal_render() was sent a frac with numberOfChildren > FRACTAL_MAX_CHILDREN. This will be automatically corrected. numberOfChildren was equal to ",frac->numberOfChildren);
		frac->numberOfChildren = FRACTAL_MAX_CHILDREN;
	}
	
	
	// call the first iteration. from here, all of the other iterations will be recursively called.
	fractal_render_iteration(frac, dest, 1, x, y, scale);
	
}


/// this will draw a single iteration of the fractal.
// this function is recursive. It will call itself again to render its children.
// this means that the function will take exponentially longer as the number of iterations increases.
// frac is a pointer to the fractal to be rendered.
// dest is a pointer to the SDL_Surface to be rendered to.
// iteration is the current iteration of the fractal (i.e. how deep in the fractal you are)
// <x,y> is the position of the fractal's origin on the screen (units of pixels, but still floating point numbers to retain accuracy)
// scale tells us what scale this iteration is at.
// NOTE: this function does not check for a bad fractal pointer or a bad SDL_Surface pointer. So don't send this function any NULL or 0xbaadf00d pointers.
void fractal_render_iteration(struct fractal *frac, SDL_Surface *dest, int iteration, float x, float y, float scale)
{
	int i;
	// render all the shapes. this is where the magic happens
	for(i=0; i<frac->numberOfShapes; i++)
	{
		// draw a line between <x[0],y[0]> and <x[1],y[1]>
		switch(frac->shapes[i].type)
		{
			case fst_circle:
				draw_circle(dest, x + frac->shapes[i].x[0]*scale, y + frac->shapes[i].y[0]*scale, frac->shapes[i].radius, frac->shapes[i].color);
				break;
			
			default:
			case fst_line:
				draw_line(dest, x + frac->shapes[i].x[0]*scale, y + frac->shapes[i].y[0]*scale, x + frac->shapes[i].x[1]*scale, y + frac->shapes[i].y[1]*scale, frac->shapes[i].radius, frac->shapes[i].color);
				break;
			
		}
			
	}
	
	// if this iteration is not supposed to be the last iteration,
	if(iteration < frac->iterations)
	{
		// render each child.
		for(i=0; i<frac->numberOfChildren; i++)
		{
			// render a child recursively
			fractal_render_iteration(frac, dest, iteration + 1, x + frac->children[i].x*scale, y + frac->children[i].y*scale, frac->children[i].scale*scale);
		}
	}
}




void fractal_set_default(struct fractal *frac)
{
	// shape, child, pair.
	int s,c,p;
	
	// for every shape in a fractal
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		// set defaults
		frac->shapes[s].color = 0xFFFF00FF;				// magenta by default
		frac->shapes[s].fillPercent = 0.5;				// 50% by default
		frac->shapes[s].fillType = fsf_full;			// diagonal fill by default
		frac->shapes[s].radius = 30;					// 30 pixels by default
		frac->shapes[s].type = fst_line;				// default is a line
		// for every xy pair in a shape
		for(p=0; p<FRACTAL_MAX_SHAPE_POINTS; p++)
		{
			// set defaults
			frac->shapes[s].x[p] = s*30 - p*20;			// some unique value based on s and p
			frac->shapes[s].y[p] = -s*30 + p*20;		// "
		}
	}
	// for every child in a fractal
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		// set defaults
		frac->children[c].x = 200*cos(3.1415*2*c/((float)FRACTAL_MAX_CHILDREN));	// distribute the children points initially radially outward from the fractal origin.
		frac->children[c].y = 200*sin(3.1415*2*c/((float)FRACTAL_MAX_CHILDREN));	// "
		frac->children[c].scale = FRACTAL_DEFAULT_SCALE;							// default scale
		frac->children[c].twist = FRACTAL_DEFAULT_TWIST;							// default twist
	
	}
	// set defaults for other things
	frac->iterations = FRACTAL_DEFAULT_ITERATIONS;			// default iterations
	frac->numberOfChildren = FRACTAL_DEFAULT_CHILDREN;		// default number of children
	frac->numberOfShapes = FRACTAL_DEFAULT_SHAPES;			// default number of shapes
	frac->zoom = FRACTAL_DEFAULT_ZOOM;						// default zoom scale
	frac->x = windW/2;										// default placement of origin on SDL_Surface rendered to
	frac->y = windH/2;										// "
}


