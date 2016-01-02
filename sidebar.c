#include "sidebar.h"
#include "fractal.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include "utilities.h"




/// This renders the sidebar
// this will render the sidebar onto a surface that is the same size (height
// and width) as the sidebar. This essentially ignores the rect.x and rect.y
// variables.
// Inputs:
//  *event a pointer to an SDL_Event. This comes in handy when determining
// returns...
//  0 if the user did NOT click inside the sidebar.
//  1
int sidebar_render(struct sidebar *sb, SDL_Surface *dest)
{
	// this is a general purpose rectangle just used in this function
	struct SDL_Rect genrect;
	
	// create a rectangle that is the same size as the sidebar, but is at <0,0>
	// this is just for printing it inside the 
	genrect.x = 0;
	genrect.y = 0;
	genrect.w = sb->rect.w;
	genrect.h = sb->rect.h;
	SDL_FillRect(dest, &genrect, sb->colorBG);
	
	// print the border color 
	genrect.h = sb->topButtonsHeight;
	SDL_FillRect(dest, &genrect, sb->colorButtonsBorder);
	
	// calculate how many top buttons (other than the ones on the left and right)
	//int b = 
	
	
	return 0;
}




/// This will interpret user actions (events) and use them to
/// configure the sidebar and its associated fractal
// pass this a sidebar and an event.
// this function determines if the even is relevant to the sidebar.
// if the user clicks inside the sidebar, the sidebar "owns" that click.
// However, the user perform an action that does not necessarily belong to the sidebar.
// Returns:
//	0	action was NOT relevant to the sidebar and it will need to be evaluated by something else.
//	1	action WAS relevant to the sidebar and was "used" by it.
int sidebar_evaluate(struct sidebar *sb, SDL_Event *event)
{
	// Assume the event was irrelevant to the sidebar.
	// If you later find something (like a click inside the sidebar area)
	// that IS relevant, you will have to set this to 1.
	char relevant = 0;
	
	// if the user clicks down in the side bar,
	if(event->button.type == SDL_MOUSEBUTTONDOWN)
	{
		if(within_rect(event->button.x,event->button.y,&sb->rect))
		{
			relevant = 1;
		}
	}
	
	// return if the user event was relevant to the sidebar or not
	return relevant;
}




/// This will modify a sidebar struct and set the default values.
// returns 
//	0	NULL sb pointer
//	1	operation success
int sidebar_init(struct sidebar *sb)
{
	// check for a NULL sb pointer
	if(sb == NULL)
	{
		error("sidebar_init() was sent a NULL [sb] pointer! Aborting function call.");
		return 0;
	}
	
	// set default position and size
	sb->rect.x = 0;
	sb->rect.y = 0;
	sb->rect.w = 200;
	sb->rect.h = windH;
	
	
	// set the default button properties
	sb->topButtonsPosition = 0;
	sb->topButtonsHeight = 20;
	sb->topButtonsWidth = 20;
	sb->topButtonsLeftRightWidth = 10;
	sb->topButtonsBorder = 1;
	
	// set the fractal to NULL
	sb->frac = NULL;
	
	// set the default background color
	sb->colorBG = 0xFF404040;
	// set the default button background color
	sb->colorButtonsBG = 0xFF707070;
	// set the default button border color
	sb->colorButtonsBorder = 0xFFa0a0a0;
	
	
	// successfully initialized the sidebar
	return 1;
}
