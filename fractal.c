#include "fractal.h"
#include "utilities.h"
#include "graphics.h"
#include <math.h>
#include "my_math.h"

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
			
			case fst_line:
				twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
				twist_xy(frac->shapes[i].x[1], frac->shapes[i].y[1], twist, &tx[1], &ty[1]);
				draw_line(dest, x + tx[0]*scale, y + ty[0]*scale, x + tx[1]*scale, y + ty[1]*scale, frac->shapes[i].radius*scale, frac->shapes[i].color);
				break;
			
			default:
			case fst_pixel:
				twist_xy(frac->shapes[i].x[0], frac->shapes[i].y[0], twist, &tx[0], &ty[0]);
				draw_pixel(dest, (x + tx[0]*scale)+0.5, (y + ty[0]*scale)+0.5, frac->shapes[i].color);
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
// NOTE: this function does NOT check for NULL x_ret, y_ret pointers. SO DON'T SEND IT ANY!
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
	// check for a NULL frac pointer
	if(frac == NULL)
	{
		error("fractal_set_default() was sent a NULL [frac] pointer! Aborting function call.");
		return;
	}
	
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




/// this will copy the data from one fractalSelect structure into another fractalSelect structure
// returns 0  on successful copy
// returns -1 on NULL source pointer
// returns -2 on NULL dest   pointer
int fractal_select_copy(struct fractalSelect *source, struct fractalSelect *dest)
{
	// check for a NULL select pointer
	if(source == NULL)
	{
		error("fractal_select_copy() was sent a NULL [source] pointer. Aborting function call.");
		return -1;
	}
	// check for a NULL select pointer
	if(dest == NULL)
	{
		error("fractal_select_copy() was sent a NULL [dest] pointer. Aborting function call.");
		return -2;
	}
	
	int s,sp;
	// for every shape,
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		// for every shape point,
		for(sp=0; sp<FRACTAL_MAX_SHAPE_POINTS; sp++)
		{
			// copy each shape point
			dest->shapePoints[s][sp] = source->shapePoints[s][sp];
		}
		// copy the radius of each shape
		dest->shapeRadius[s] = source->shapeRadius[s];
	}
	
	int c;
	// for each child,
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		// copy each child
		dest->child[s] = source->child[s];
	}
	return 0; // success
}




/// this will copy the data from one fractal structure into another fractal structure
// returns 0  on successful copy
// returns -1 on NULL source pointer
// returns -2 on NULL dest   pointer
int fractal_copy(struct fractal *source, struct fractal *dest)
{
	// check for a NULL select pointer
	if(source == NULL)
	{
		error("fractal_copy() was sent a NULL [source] pointer. Aborting function call.");
		return -1;
	}
	// check for a NULL select pointer
	if(dest == NULL)
	{
		error("fractal_copy() was sent a NULL [dest] pointer. Aborting function call.");
		return -2;
	}
	
	int s,sp;
	// copy all shape parameters
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		// copy parameters
		
		dest->shapes[s].type =			source->shapes[s].type;
		dest->shapes[s].color =			source->shapes[s].color;
		dest->shapes[s].radius = 		source->shapes[s].radius;
		dest->shapes[s].fillType = 		source->shapes[s].fillType;
		dest->shapes[s].fillPercent=	source->shapes[s].fillPercent;
		
		// copy all of the xy points for each shape
		for(sp=0; sp<FRACTAL_MAX_SHAPE_POINTS; sp++)
		{
			dest->shapes[s].x[sp] = source->shapes[s].x[sp];
			dest->shapes[s].y[sp] = source->shapes[s].y[sp];
		}
		
	}
	dest->numberOfShapes =		source->numberOfShapes;
	
	int c;
	// copy all child parameters 
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		dest->children[c].scale =	source->children[c].scale;
		dest->children[c].twist =	source->children[c].twist;
		dest->children[c].x =		source->children[c].x;
		dest->children[c].y =		source->children[c].y;
	}
	dest->numberOfChildren = 	source->numberOfChildren;
	
	dest->iterations =			source->iterations;
	dest->iterationsChildren =	source->iterationsChildren;
	dest->zoom =				source->zoom;
	dest->twist =				source->twist;
	dest->x =					source->x;
	dest->y =					source->y;
	
	
	// copy the fractalSelect structure from the source in to the dest
	// also, return the value returned from fractal_select_copy.
	// That will indicate the overall success of the copying process.
	return fractal_select_copy(&source->select, &dest->select);
}



void fractal_select_deselect_all(struct fractalSelect *select)
{
	// check for a NULL select pointer
	if(select == NULL)
	{
		error("fractal_select_clear() was sent a NULL [select] pointer. Aborting function call.");
		return;
	}
	
	int s,sp;
	// for every fractal shape,
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		// deselect each points set of xy points
		for(sp=0; sp<FRACTAL_MAX_SHAPE_POINTS; sp++)
		{
			select->shapePoints[s][sp] = 0;
		}
		// deselect each point's radius
		select->shapeRadius[s] = 0;
	}
	int c;
	// for each child,
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		// deselect that child
		select->child[c] = 0;
	}
	
}




/// this function inputs a position that a user has clicked and a fractal and determines if the user selected any of the fractal's parts.
// x and y are in pixel space (screen space). they are converted to fractal space internally in this function
// returns -1 for NULL frac pointer
// returns 0 when no part of the fractal was within range to be considered "selected"
// returns 1 when there was a part of the fractal that was selected.
int fractal_select_point(SDL_Surface *dest, struct fractal *frac, float x, float y)
{
	// check for a NULL frac pointer
	if(frac == NULL)
	{
		error("fractal_select_clear() was sent a NULL [frac] pointer! Aborting function call.");
		return -1;
	}
	
	// this keeps track of whether or not a part of the fractal was clicked.
	char clickedPart = 0;
	
	// calculate the <x,y> position in fractal space
	float xf = (x - frac->x) / frac->zoom;
	float yf = (y - frac->y) / frac->zoom;
	
	// debugging
	//draw_circle(dest, xf, yf, 10, 0xFFFF3311);
	
	// this keeps track of how close the input <x,y> pair is to the closest part of the fractal.
	// however, for sake of speed of execution, this is stored as a square value.
	// this number is always delta_x^2 + delta_y^2
	// this number's square root is found after all fractal parts have been checked.
	// closest2 is pronounced "closest squared"
	// set to a really high number initially so that everything else is smaller
	float closest2 = __FLT_MAX__;
	// this is used to store the calculation of distance between <x,y> and the current point being checked
	// dist2, like closest2, is pronounced "distance squared"
	float dist2;
	
	int s;
	int sp;
	int shapePointsUsed;
	
	
	//---------------------------------------------
	// check if the user selected parts of a shape
	//---------------------------------------------
	
	// for each fractal shape
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		// figure out how many shape points are relevant to the specific shape
		switch(frac->shapes[s].type)
		{
			case fst_circle:	shapePointsUsed = 1; break;
			case fst_line:		shapePointsUsed = 2; break;
			case fst_pixel:		shapePointsUsed = 1; break;
			case fst_rect:		shapePointsUsed = 2; break;
			case fst_tri:		shapePointsUsed = 3; break;
			default:			shapePointsUsed = 1; break;
		}
		
		
		//---------------------------------------------
		// check if the user clicked near any xy points
		//---------------------------------------------
		
		// for each xy point, check if the user clicked near that point. In this case, "near" mean
		for(sp=0; sp<shapePointsUsed; sp++)
		{
			// calculate the distance between where the user clicked and the xy point of this shape
			dist2 = sqr(xf-frac->shapes[s].x[sp]) + sqr(yf-frac->shapes[s].y[sp]);
			// if the current dist2 is less than the so-far closest found dist2 (closest2), we have found a new closest point
			if(dist2 < closest2)
			{
				// forget all previously selected parts.
				fractal_select_deselect_all(&frac->select);
				// set closest squared to the current calculated distance squared.
				closest2 = dist2;
				// set the selected part to the appropriate child number
				frac->select.shapePoints[s][sp] = 1;
			}
		}
		
		
		//---------------------------------------------
		// check if the user clicked a radius distance
		// away from the first xy point
		//---------------------------------------------
		
		// only check for the user selecting the radius if the shape is a circle
		if(frac->shapes[s].type == fst_circle)
		{
			// calculate the dist2 between <xf,yf> and the first point <x[0],y[0]>
			dist2 = sqr(xf-frac->shapes[s].x[0]) + sqr(yf-frac->shapes[s].y[0]);
			// now calculate the difference between that and the radius to get how close the user is to selecting that radius
			dist2 = dist2 + sqr(frac->shapes[s].radius);
			// if the <xf,yf> point is close to being one radius away from the shape,
			if(dist2 < closest2)
			{
				// forget all previously selected parts.
				fractal_select_deselect_all(&frac->select);
				// set closest squared to the current calculated distance squared.
				closest2 = dist2;
				// set the selected part to the appropriate child number
				frac->select.shapeRadius[s] = 1;
			}
		}// if(type == circle)
		
	}// for(shapes)
	
	
	//---------------------------------------------
	// check if the user clicked near child points
	//---------------------------------------------
	
	int c;
	// check all children
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		// calculate the distance between where the user clicked and the child point
		dist2 = sqr(xf-frac->children[c].x) + sqr(yf-frac->children[c].y);
		// if the current dist2 is less than the reining champion closest2 value, we have found a new closest point
		if(dist2 < closest2)
		{
			// forget all previously selected parts.
			fractal_select_deselect_all(&frac->select);
			// set closest squared to the current calculated distance squared.
			closest2 = dist2;
			// set the selected part to the appropriate child number
			frac->select.child[c] = 1;
		}
	}// for(children)
	
	//---------------------------------------------
	// check if the closest click was close ENOUGH
	// to select a part of the fractal
	//---------------------------------------------
	// if the click was NOT close enough,
	if( !(closest2 < sqr(FRACTAL_SELECT_PIXEL_DISTANCE)) )
	{
		// then we need to reset the selectedParts struct
		fractal_select_deselect_all(&frac->select);
		clickedPart = 0;
	}
	else
	{
		clickedPart = 1;
	}
	
	return clickedPart;
}



/// this function will input some fractal, and modify it based on a change in x and a change in y
// this function will modify all selected parts of the input fractal SIMULTANEOUSLY.
// this allows this function to modify a fractal that has a GROUP of parts selected at the same time (a group modification).
// this function does not care how many parts are selected. It just fucking works. Like a good function should.
// frac is the fractal to be modified
// x is the change in the x direction (in pixels, the unit of the fractal space)
// y is the change in the y direction (in pixels, the unit of the fractal space)
// returns 0 on successful modification
// returns -1 on NULL frac pointer
int fractal_select_modify(struct fractal *frac, float x, float y)
{
	// check for NULL pointer
	if(frac == NULL)
	{
		error("fractal_select_modify() was sent a NULL [frac] pointer! Aborting function call.");
		return -1;
	}
	
	//---------------------------------------------
	// check if shapes need to be modified
	//---------------------------------------------
	
	int s, sp;
	// for all shapes, modify the selected ones
	for(s=0; s<FRACTAL_MAX_SHAPES; s++)
	{
		//---------------------------------------------
		// check if the user wants to modify any xy points
		//---------------------------------------------
		
		// for each xy point, check if the user clicked near that point. In this case, "near" mean
		for(sp=0; sp<FRACTAL_MAX_SHAPE_POINTS; sp++)
		{
			// if the current shape point is selected for modification,
			if(frac->select.shapePoints[s][sp])
			{
				frac->shapes[s].x[sp] += x;
				frac->shapes[s].y[sp] += y;
			}
		}
		
		//---------------------------------------------
		// check if the user wants to modify the radius
		//---------------------------------------------
		
		// if the radius is selected for modification,
		if(frac->select.shapeRadius[s])
		{
			// modify the radius
			frac->shapes[s].radius += x; /// TODO: fix this stupid hack. The radius needs to be configurable is a user-friendly way.
		}
		
	}// for(shape)
	
	//---------------------------------------------
	// check if children need to be modifies
	//---------------------------------------------
	
	int c;
	// check all children
	for(c=0; c<FRACTAL_MAX_CHILDREN; c++)
	{
		// if this child is selected for modification,
		if(frac->select.child[c])
		{
			frac->children[c].x += x;
			frac->children[c].y += y;
		}
	}// for(children)
	
	return 0; // success
}




