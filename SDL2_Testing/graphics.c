#include "graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "globals.h"
#include "utilities.h"


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
