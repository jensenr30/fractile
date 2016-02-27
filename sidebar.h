#include <inttypes.h>
#include <SDL2/SDL.h>


/// This specifies the location, size, and every other aspect of the sidebar.
// Many things are automatically discovered from the fractal (frac) that the sidebar
// is pointing at.
// The position of the sidebar is relative to the top left corner of the window.
// All dimensions internal to the sidebar are (probably) in pixels and are relative
// to the top left corner of the sidebar.
struct sidebar
{
	// this is used to specify the location and size of the sidebar
	struct SDL_Rect rect;
	
	
	//--------------------------------------------------------------------------
	// These buttons are at the top of the sidebar and span its width
	//--------------------------------------------------------------------------
	
	// this indicates the user's position in the top buttons.
	unsigned int topButtonsPosition;
	// this is the height of the buttons located at the top of the sidebar
	unsigned int topButtonsHeight;
	// this is the width of the normal buttons
	unsigned int topButtonsWidth;
	// this is the width of the left << and right >> buttons.
	unsigned int topButtonsLeftRightWidth;
	// this is the width of the buttons borders (pixels)
	//unsigned int topButtonsBorder;
	
	
	// this points to the fractal whose information is being displayed in the sidebar
	struct fractal *frac;
	
	//--------------------------------------------------------------------------
	// Colors
	//--------------------------------------------------------------------------
	// all colors are stored in 32-bit unsigned integers.
	// the format is 0xAARRGGBB where
        // 0x indicates hexadecimal notation
        // AA is 8-byte alpha (opposite/inverse of opacity) value
        // RR is 8-byte red   value
        // GG is 8-byte green value
        // BB is 8-byte blue  value
	
	// this is the background color of the sidebar
	uint32_t colorBG;
	// this is the background color of all the buttons
	uint32_t colorButtonsBG;
	// this is the border color of all the buttons
	uint32_t colorButtonsBorder;
	
};




int sidebar_render(struct sidebar *sb, SDL_Surface *dest);
int sidebar_init(struct sidebar *sb);
int sidebar_evaluate(struct sidebar *sb, SDL_Event *event, uint16_t x, uint16_t y);
