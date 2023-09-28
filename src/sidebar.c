#include "sidebar.h"
#include "fractal.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include "utilities.h"




/// This will set a sidebar struct to the default values.
// returns:
//	0	operation success
//	-1	NULL sb pointer
int sidebar_init(struct sidebar *sb)
{
	// check for a NULL sb pointer
	if(sb == NULL)
	{
		error("sidebar_init() was sent a NULL [sb] pointer! Aborting function call.");
		return -1;
	}

	// set default position and size
	sb->rect.x = 0;
	sb->rect.y = 0;
	sb->rect.w = 200;
	sb->rect.h = get_window_height();


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
	sb->colorTopButtonsBG = 0xFF707070;
	// set the default button border color
	sb->colorTopButtonsBorder = 0xFFa0a0a0;

	// the font used for topButtons
	sb->font1Path = "Inconsolata-Bold.ttf";			// path to the font
	sb->font1Size = sb->topButtonsHeight;			// size of the font
	sb->font1Color = 0xFF000000;					// color (0xAARRGGBB)
	sb->font1OffsetX = 5;							// x offset (pixels)
	sb->font1OffsetY = -2;							// y offset (pixels)

	// the font used for general text
	sb->font2Path = "Inconsolata-Regular.ttf";		// path to the font
	sb->font2Size = 10;								// size of the font
	sb->font1Color = 0xFF000000;					// color (0xAARRGGBB)
	sb->font2OffsetX = 5;							// x offset (pixels)
	sb->font2OffsetY = -2;							// y offset (pixels)

	// successfully initialized the sidebar
	return 0;
}


/// This renders all the text.
// this should be called once at the beginning of the program.
// I supposed if you wanted to change font mid-way through the program you could call it a second time.
// it
// returns:
// 0	operation success
// -1	bad sb pointer
// -2	could not open font1
// -3	could not open font2

// -9	couldn't render letter 'F'
// -10	couldn't render number 0
// -11	couldn't render number 1
// -12	couldn't render number 2
// -13	couldn't render number 3
// -14	couldn't render number 4
// -15	couldn't render number 5
// -16	couldn't render number 6
// -17	couldn't render number 7
// -18	couldn't render number 8
// -19	couldn't render number 9

int sidebar_load_fonts(struct sidebar *sb)
{
	// make sure the sb is valid
    if(sb == NULL)
	{
		error("sidebar_load_fonts() was sent a NULL sb pointer!");
		return -1;
	}

	// attempt to open font1
    sb->font1 = TTF_OpenFont( sb->font1Path, sb->font1Size );
	//If there was an error in loading the font
    if( sb->font1 == NULL )
    {
    	error_s("could not open font1, ",sb->font1Path);
        return -2;
    }

    // get the color and put in into an SDL_Color format
    SDL_Color textColor;
    textColor.r = getColorRed(sb->font1Color);
    textColor.g = getColorGreen(sb->font1Color);
    textColor.b = getColorBlue(sb->font1Color);
    textColor.a = getColorAlpha(sb->font1Color);
	// render all 10 digits (0 through 9) so that they are ready to go when they are needed.
	uint8_t n;
	char myStr[2] = "0";
	for(n = 0; n < 10; n++)
	{
		myStr[0] = '0' + n;
		sb->font1Numbers[n] = TTF_RenderText_Blended(sb->font1, myStr, textColor);
		// make sure the error is reported
		if(sb->font1Numbers[n] == NULL)
		{
			error_d("sidebar_load_fonts() using font1 could not render number ",n);
			return -10-n;
		}
	}

	// render the letter 'F' so it is ready to go
	sb->font1LetterF = TTF_RenderText_Blended(sb->font1, "F", textColor);


    // attempt to open font2
    sb->font2 = TTF_OpenFont( sb->font2Path, sb->font2Size );
	//If there was an error in loading the font
    if( sb->font2 == NULL )
    {
    	error_s("could not open font2, ",sb->font2Path);
        return -3;
    }

	// success
	return 0;
}


/// This renders the sidebar
// this will render the sidebar onto a surface that is the same size (height
// and width) as the sidebar. This essentially ignores the rect.x and rect.y
// variables.
// it always renders at x=0, y=0.
// if you want to move the sidebar somewhere else, you will need to do that yourself.
// Inputs:
//  *event a pointer to an SDL_Event.
// returns...
// 0	operation success
// -1	NULL sb pointer
// -2	NULL dest pointer
int sidebar_render(struct sidebar *sb, SDL_Surface *dest)
{
	if(sb == NULL)
	{
		error("sidebar_render() was sent a NULL sb pointer!");
		return -1;
	}


	if(dest == NULL)
	{
		error("sidebar_render() was sent a NULL dest pointer!");
		return -2;
	}
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
	SDL_FillRect(dest, &genrect, sb->colorTopButtonsBorder);

	// print the << left arrow button background
	genrect.x = 0 + sb->buttonsBorder;
	genrect.y = 0 + sb->buttonsBorder;
	genrect.w = sb->topButtonsLeftRightWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	SDL_FillRect(dest, &genrect, sb->colorTopButtonsBG);

	// print the >> right arrow button
	genrect.x = sb->rect.w - sb->topButtonsLeftRightWidth + sb->buttonsBorder;
	genrect.y = 0 + sb->buttonsBorder;
	genrect.w = sb->topButtonsLeftRightWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	SDL_FillRect(dest, &genrect, sb->colorTopButtonsBG);

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
		SDL_FillRect(dest, &genrect, sb->colorTopButtonsBG);
		b++;
	}

	// print shapes for the buttons
	genrect.y = 0 + sb->buttonsBorder;
	genrect.w = sb->topButtonsWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	while(b < topButtonsFit && b < sb->frac->numberOfChildren + sb->frac->numberOfShapes)
	{
		genrect.x = sb->topButtonsLeftRightWidth + b*sb->topButtonsWidth + sb->buttonsBorder;
		SDL_FillRect(dest, &genrect, sb->colorTopButtonsBG);
		b++;
	}

	// test print the 'F'
	genrect.y = 0 + sb->buttonsBorder + sb->font1OffsetY;
	genrect.w = sb->topButtonsWidth - 2*sb->buttonsBorder;
	genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
	genrect.x = sb->topButtonsLeftRightWidth + 0*sb->topButtonsWidth + sb->buttonsBorder + sb->font1OffsetX;
	SDL_BlitSurface(sb->font1LetterF, NULL, dest, &genrect);

	// test printing numbers over topButtons
	for(b = 1; b < 7; b++)
	{
		genrect.y = 0 + sb->buttonsBorder + sb->font1OffsetY;
		genrect.w = sb->topButtonsWidth - 2*sb->buttonsBorder;
		genrect.h = sb->topButtonsHeight - 2*sb->buttonsBorder;
		genrect.x = sb->topButtonsLeftRightWidth + b*sb->topButtonsWidth + sb->buttonsBorder + sb->font1OffsetX;
		SDL_BlitSurface(sb->font1Numbers[b], NULL, dest, &genrect);
	}


	// success
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

