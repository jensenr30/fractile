#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "graphics.h"
#include "globals.h"
unsigned int windW = 1000;
unsigned int windH = 600;

// Windows wrapper. I'm not proud of this.
int main(int argc, char *argv[]){
	
	myWindow = SDL_CreateWindow("My Game Window", 100, 100, windW, windH, SDL_WINDOW_RESIZABLE);
	myRenderer = SDL_CreateRenderer(myWindow, -1, 0);
	
	if(myWindow == NULL){
		error("main() could not create window using SDL_CreateWindow");
		return -1;
	}
	if(myRenderer == NULL){
		error("main() could not create renderer using SDL_CreateRenderer");
		return -2;
	}
	
	
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
	SDL_RenderClear(myRenderer);
	SDL_RenderPresent(myRenderer);
	
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // make the scaled rendering look smoother
	//SDL_RenderSetLogicalSize(myRenderer, windW, windH);
	
	myTexture = SDL_CreateTexture(myRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windW, windH);
	
	SDL_Texture *glider = load_image_to_texture("glider.jpg");
	
	if(myTexture == NULL){
		error("main() could not create texture using SDL_CreateTexture");
		return -2;
	}
	
	if(myTexture == NULL){
		error("main() could not create glider texture using SDL_CreateTexture");
		return -2;
	}
	
	
	/*
	Uint32 *myPixels;
	myPixels = malloc(windW*windH*sizeof( Uint32));
	
	int i, j;
	for(j=0; j<windW; j++){
		for(i=0; i<windH; i++){
			myPixels[i*windW+j] = ((int)(255*i/((float)windW) + (255*j/((float)windH))))<<8;
		}
	}
	SDL_UpdateTexture(myTexture, NULL, myPixels, windW*sizeof(Uint32) );
	*/
	
	
	
	SDL_RenderClear(myRenderer);
	SDL_RenderCopy(myRenderer, myTexture, NULL, NULL);
	SDL_RenderCopy(myRenderer, glider, NULL, NULL);
	SDL_RenderPresent(myRenderer);
		
	SDL_Delay(3000);
	
	return 0;
}
