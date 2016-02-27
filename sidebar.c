#include "sidebar.h"
#include "fractal.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include "utilities.h"




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
	sb->topButtonsHeight = 24;
	sb->topButtonsWidth = 24;
	sb->topButtonsLeftRightWidth = 16;
	sb->buttonsBorder = 1;
	
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
	
	// print the << left arrow button background
	genrect.x = 0 + sb->buttonsBorder;
	genrect.y = 0 + sb->buttonsBorder;
	genrect.w = sb->topButtonsLeftRightWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	SDL_FillRect(dest, &genrect, sb->colorButtonsBG);
	
	// print the >> right arrow button
	genrect.x = sb->rect.w - sb->topButtonsLeftRightWidth + sb->buttonsBorder;
	genrect.y = 0 + sb->buttonsBorder;
	genrect.w = sb->topButtonsLeftRightWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	SDL_FillRect(dest, &genrect, sb->colorButtonsBG);
	
	// calculate how many top buttons can fit between the left and right top buttons.
	uint8_t topButtonsFit = (sb->rect.w - sb->topButtonsLeftRightWidth*2)/sb->topButtonsWidth;
	uint8_t b = 0;
	
	// print buttons for children
	while(b < topButtonsFit && b < sb->frac->numberOfChildren)
	{
		genrect.x = sb->topButtonsLeftRightWidth + b*sb->topButtonsWidth + sb->buttonsBorder;
		genrect.y = 0 + sb->buttonsBorder;
		genrect.w = sb->topButtonsWidth - 2*sb->buttonsBorder;
		genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
		SDL_FillRect(dest, &genrect, sb->colorButtonsBG);
		b++;
	}
	
	// print buttons for the shapes
	while(b < topButtonsFit && b < sb->frac->numberOfChildren + sb->frac->numberOfShapes)
	{
		genrect.x = sb->topButtonsLeftRightWidth + b*sb->topButtonsWidth + sb->buttonsBorder;
		genrect.y = 0 + sb->buttonsBorder;
		genrect.w = sb->topButtonsWidth - 2*sb->buttonsBorder;
		genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
		SDL_FillRect(dest, &genrect, sb->colorButtonsBG);
		b++;
	}
	
	return 0;
}




/// This will interpret user actions (events) and use them to
/// configure the sidebar and its associated fractal
// pass this a sidebar and an event.
// also pass the last known x and y values
// this function determines if the even is relevant to the sidebar.
// if the user clicks inside the sidebar, the sidebar "owns" that click.
// However, the user perform an action that does not necessarily belong to the sidebar.
// Returns:
//	0	action was NOT relevant to the sidebar and it will need to be evaluated by something else.
//	1	action WAS relevant to the sidebar and was "used" by it.
int sidebar_evaluate(struct sidebar *sb, SDL_Event *event, uint16_t x, uint16_t y)
{
	// Assume the event was irrelevant to the sidebar.
	// If you later find something (like a click inside the sidebar area)
	// that IS relevant, you will have to set this to 1.
	char relevant = 0;
	
	if( within_rect(x, y, &sb->rect) )
	{
		// if the user clicks down in the side bar,
		if(event->button.type == SDL_MOUSEBUTTONDOWN)
		{
			relevant = 1;
		}
		else if(event->type == SDL_MOUSEWHEEL)
		{
			relevant = 1;
		}
		else
		{
			relevant = 0;
		}
	}
	else
	{
		relevant = 0;
	}
	
	
	
	// return if the user event was relevant to the sidebar or not
	return relevant;
}

