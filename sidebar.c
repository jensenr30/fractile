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
    
    SDL_FillRect(dest, &genrect, sb->colorBackground);
    
    
    return 0;
}


/// This will modify a sidebar struct and set the default values.
//
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
    
    
    // set the default button sizes
    sb->topButtonsHeight = 20;
    sb->topButtonsWidth = 20;
    sb->topButtonsLeftRightWidth = 10;
    
    // set the fractal to NULL
    sb->frac = NULL;
    
    // set the default background color
    sb->colorBackground = 0xFF404040;
    
    // successfully initialized the sidebar
    return 1;
}
