#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int width = 1600;
unsigned int height = 900;

// Windows wrapper. I'm not proud of this.
int main(int argc, char *argv[]){
	
	SDL_Window *myWindow = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *myRenderer = SDL_CreateRenderer(myWindow, -1, 0);
	
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
	SDL_RenderClear(myRenderer);
	SDL_RenderPresent(myRenderer);
	
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // make the scaled rendering look smoother
	//SDL_RenderSetLogicalSize(myRenderer, width, height);
	
	SDL_Texture *myTexture = SDL_CreateTexture(myRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	
	Uint32 *myPixels;
	myPixels = malloc(width*height*sizeof( Uint32));
	
	int i, j;
	for(j=0; j<width; j++){
		for(i=0; i<height; i++){
			myPixels[i*width+j] = i*width+j;
		}
	}
	
	SDL_UpdateTexture(myTexture, NULL, myPixels, width*sizeof(Uint32) );
	
	SDL_RenderClear(myRenderer);
	SDL_RenderCopy(myRenderer, myTexture, NULL, NULL);
	SDL_RenderPresent(myRenderer);
		
	SDL_Delay(3000);
	
	return 0;
}
