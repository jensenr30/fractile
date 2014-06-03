//#include "bool.h"
#include "SDL.h"
#include "SDL_ttf.h"


Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void set_pixel(SDL_Surface *surf, int x, int y, Uint32 pixel);
void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination );
int apply_text(SDL_Surface *datsurface, int x, int y, TTF_Font *theFont, char *theString, SDL_Color theColor);
void apply_surface_clips( int x, int y,  SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip );
void apply_outline(SDL_Surface *dest, SDL_Rect *box, unsigned short thickness, Uint32 fillColor);
#define grad_linear 0
#define grad_radial 1
void gradient(SDL_Surface *datsurface, SDL_Rect *gradClip, int x1, int y1, int x2, int y2, Uint32 color1, Uint32 color2, unsigned int gradientType);
void draw_line(SDL_Surface *dest, float x1, float y1, float x2, float y2, float thickness, unsigned int lineColor);
// this will draw the line from point (x0,y0) to point (x0+x,y0+y).
#define draw_line_diff(dest, x0, y0, x, y, thickness, lineColor) draw_line(dest, x0, y0, x0+x, y0+y, thickness, lineColor)
void draw_circle(SDL_Surface *dest, float x, float y, float radius, Uint32 color);

