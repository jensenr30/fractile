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
    
    // this is the height of the buttons located at the top of the sidebar
    unsigned int topButtonsHeight;
    // this is the width of the normal buttons
    unsigned int topButtonsWidth;
    // this is the width of the left << and right >> buttons.
    unsigned int topButtonsLeftRightWidth;
    
    // this points to the fractal whose information is being displayed in the sidebar
    struct fractal *frac;
    
    // this is the background color of the sidebar
    uint32_t colorBackground;
};




int sidebar_render(struct sidebar *sb, SDL_Surface *dest);
int sidebar_init(struct sidebar *sb);
