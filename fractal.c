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
void fractal_render(struct fractal *frac, SDL_Surface *dest)
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
	fractal_render_iteration(frac, dest, 0, frac->x, frac->y, frac->zoom, frac->twist);
	
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
void fractal_render_iteration(struct fractal *frac, SDL_Surface *dest, int iteration, float x, float y, float scale, float twist)
{
	
	int i;
	float tx[FRACTAL_MAX_SHAPE_POINTS];
	float ty[FRACTAL_MAX_SHAPE_POINTS];
	// render all the shapes. this is where the magic happens
	for(i=0; i<frac->numberOfShapes; i++)
	{
		// draw a line between <x[0],y[0]> and <x[1],y[1]>
		switch(frac->shapes[i].type)
		{
			case fst_circle:
				twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
				draw_circle(dest, x + tx[0]*scale, y + ty[0]*scale, frac->shapes[i].radius*scale, frac->shapes[i].color);
				break;
			
			default:
			case fst_line:
				twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
				twist_xy(frac->shapes[i].x[1], frac->shapes[i].y[1], twist, &tx[1], &ty[1]);
				draw_line(dest, x + tx[0]*scale, y + ty[0]*scale, x + tx[1]*scale, y + ty[1]*scale, frac->shapes[i].radius*scale, frac->shapes[i].color);
				break;
			
		}
		
	}
	
	// if this iteration is not supposed to be the last iteration,
	if(iteration < frac->iterations)
	{
		// render each child.
		for(i=0; i<frac->numberOfChildren; i++)
		{
			// calculate the twist for each child (using the tx and ty variables)
			twist_xy(frac->children[i].x, frac->children[i].y, twist, &tx[0], &ty[0]);
			// render a child recursively
			fractal_render_iteration(frac, dest, iteration + 1, x + tx[0]*scale, y + ty[0]*scale, frac->children[i].scale*scale, twist + frac->children[i].twist);
		}
	}
	
}

/// this will take a point <x,y> and rotate it around 0 by twist degrees
// <x,y> is a point relative to <0,0>
// twist is the amount (in degrees) that <x,y> should be twisted
// <x_ret,y_ret> is <x,y> rotated by the angle twist
void twist_xy(float x, float y, float twist, float *x_ret, float *y_ret)
{
	float mag = sqrt(x*x + y*y);
	
	float ang;
	
	if(x != 0)
	{
		ang = atan(y/x);
		// if x is less that 0, then we need to add this correction factor because of the 180 degree cyclic nature of the atan() function
		if(x < 0) ang -= 3.141592654;
	}
	// if x IS equal to zero, then we cannot use atan(y/x) because we would be dividing by zero. fortunately for us, we know the angle must either be positive or negative 90 degrees based on the sign on y
	else
	{
		if(y > 0)
		{
			ang = 1.570796;
		}
		else
		{
			ang = -1.570796;
		}
	}
	
	
	
	// convert twist to radians and add it to the angle
	ang += twist*0.01745329252;
	
	// return the twisted x and y coordinates
	*x_ret = mag*cos(ang);
	*y_ret = mag*sin(ang);
	
}

/// this function renders the children's placement as simple circles.
// this allows the user to visually see where the fractal's "exit" points are. Where it will copy itself
void fractal_render_children(struct fractal *frac, SDL_Surface *dest, int iterations)
{
	
	int c;
	for(c=0; c<frac->numberOfChildren; c++)
	{
		draw_circle(dest, frac->x + frac->children[c].x*frac->zoom, frac->y + frac->children[c].y*frac->zoom, FRACTAL_CHILDREN_DISPLAY_RADIUS*frac->zoom, 0xffffffff);
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
		frac->children[c].twist = FRACTAL_DEFAULT_CHILDREN_TWIST;					// default twist for children
	
	}
	// set defaults for other things
	frac->iterations = FRACTAL_DEFAULT_ITERATIONS;					// default iterations
	frac->iterationsChildren = FRACTAL_DEFAULT_ITERATIONS_CHILDREN;	// default iterations of rendered children points
	frac->numberOfChildren = FRACTAL_DEFAULT_CHILDREN;				// default number of children
	frac->numberOfShapes = FRACTAL_DEFAULT_SHAPES;					// default number of shapes
	frac->zoom = FRACTAL_DEFAULT_ZOOM;								// default zoom scale
	frac->twist = FRACTAL_DEFAULT_TWIST;							// default twist
	frac->x = windW/2;												// default placement of origin on SDL_Surface rendered to is the center of the screen
	frac->y = windH/2;												// "
}

