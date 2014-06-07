#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "globals.h"
#include "utilities.h"

/// this returns the pixel data of a pixel at a certain point on a surface (color and alpha in an Uint32)
Uint32 get_pixel(SDL_Surface *surface, int x, int y){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
    switch(bpp) {
    case 1:
        return *p;
        break;
	
    case 2:
        return *(Uint16 *)p;
        break;
	
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;
	
    case 4:
        return *(Uint32 *)p;
        break;
	
    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/// this sets's a pixel's data (color and alpha)
void set_pixel(SDL_Surface *surf, int x, int y, Uint32 pixel){
    Uint32 *p = (Uint32 *)( surf->pixels + (surf->pitch * y) + x*surf->format->BytesPerPixel );
	*p = pixel;
}




/// returns a pointer to the loaded texture.
SDL_Texture *load_image_to_texture(char *filename){
	
	//attempt to load the image
	SDL_Surface *tempSurface = IMG_Load(filename);
	
	// if the image was not loaded to a surface correctly,
	if(tempSurface == NULL){
		// create an error message to write to file.
		char message[512];
		strcpy(message, "error in load_image(): could not load file to surface \"");
		strcat(message, filename);
		strcat(message, "\"");
		error(message);
		// and return NULL.
		return NULL;
	}
	
	// try to convert the surface into a texture
	SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(myRenderer, tempSurface);
	
	// if the texture was not created from surface correctly,
	if(tempTexture == NULL){
		// write and error message to the error log
		error("error in load_image(): could not create texture from surface");
		// and return NULL.
		return NULL;
	}
	
	//  if the image loaded correctly, return a pointer to the texture that contains it.
	return tempTexture;
}
