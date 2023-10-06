#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <inttypes.h>

/// This specifies the location, size, and every other aspect of the sidebar.
// Many things are automatically discovered from the fractal (frac) that the sidebar
// is pointing at.
// The position of the sidebar is relative to the top left corner of the window.
// All dimensions internal to the sidebar are (probably) in pixels and are relative
// to the top left corner of the sidebar.
struct sidebar {
    // this is used to specify the location and size of the sidebar
    struct SDL_Rect rect;

    // this points to the fractal whose information is being displayed in the sidebar
    struct fractal *frac;

    //--------------------------------------------------------------------------
    // These buttons are at the top of the sidebar and span its width
    //--------------------------------------------------------------------------
    // top buttons are arranged like so:
    // F 0 1 2 3 (...) a b c d (...)
    // where
    // F	is the fractal button. This section of the sidebar has information and
    // 1	is the first child
    // 2	is the seconds child
    // (etc...)
    // a	is the first shape
    // b	is the second shape
    // (etc...)
    //
    // the total number of buttons is 1 + (numberOfChildren) + (numberOfShapes);
    // therefore, the total number of buttons is dynamic and depends on the shapes and children that
    // the user has specified.

    // this indicates the user's position in the top buttons.
    // e.g., if topButtonsPosition = 0, the left-most button is the 'F' button.
    // e.g., if topButtonsPosition = 1, the 'F' button is not displayed (it has been moved left and
    // behind the '<< left button').
    //       The left-most displayed button becomes the '1' button for the first child.
    unsigned int topButtonsPosition;
    // This is the button the user has selected (which one is highlighted).
    // This tells us which options are available in the sidebar
    unsigned int topButtonsSelected;
    // this is the height of the buttons located at the top of the sidebar
    unsigned int topButtonsHeight;
    // this is the width of the normal buttons
    unsigned int topButtonsWidth;
    // this is the width of the left << and right >> buttons.
    unsigned int topButtonsLeftRightWidth;

    // this is the width of the buttons borders (pixels)
    // this does not change the size of the buttons or the hit-box area.
    // this only specifies the color border inside each button to make it look nicer
    uint16_t buttonsBorder;
    // this is the color of the text
    uint32_t colorTopButtonsText;

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
    uint32_t colorTopButtonsBG;
    // this is the border color of all the buttons
    uint32_t colorTopButtonsBorder;

    //--------------------------------------------------------------------------
    // font1 is used for topButtons
    //--------------------------------------------------------------------------

    // the font that is used for the top buttons
    TTF_Font *font1;
    // this points to a string which contains the path of the font you want to use.
    // this points to a .ttf file.
    char *font1Path;
    // this is the font size
    uint16_t font1Size;
    // this is the color of the font
    uint32_t font1Color;
    // these surfaces are pre-rendered in the main() function at the program's startup.
    // This is done so that letters and numbers can be pasted quickly without having to be rendered
    // each and every time. These will point to surfaces that contain the numbers 0 through 9
    // respectively.
    SDL_Surface *font1Numbers[10];
    // This will point to a surface that will always have the letter F rendered on it.
    SDL_Surface *font1LetterF;
    // this is the offset to apply when printing font1
    int16_t font1OffsetX;
    int16_t font1OffsetY;

    //--------------------------------------------------------------------------
    // font2 is used for general text / messages / whatever in the sidebar
    //--------------------------------------------------------------------------

    // the font that is used for general text
    TTF_Font *font2;
    // this points to a string which contains the path of the font you want to use.
    // this points to a .ttf file.
    char *font2Path;
    // this is the font size
    uint16_t font2Size;
    // this is the color of the font
    uint32_t font2Color;
    // this is the offset to apply when printing font2
    int16_t font2OffsetX;
    int16_t font2OffsetY;
};

int sidebar_init(struct sidebar *sb);
int sidebar_load_fonts(struct sidebar *sb);
int sidebar_render(struct sidebar *sb, SDL_Surface *dest);
int sidebar_evaluate(struct sidebar *sb, SDL_Event *event, uint16_t x, uint16_t y);

#endif /* SIDEBAR_H */
