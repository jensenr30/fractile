#include <SDL2/SDL.h>

Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void set_pixel(SDL_Surface *surf, int x, int y, Uint32 pixel);

SDL_Texture *load_image_to_texture(char *filename);
