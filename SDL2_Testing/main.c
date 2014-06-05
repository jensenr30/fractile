#include <SDL.h>
#include <stdio.h>

// Windows wrapper. I'm not proud of this.
int main(int argc, char *argv[]){
	
	SDL_Window *myWindow = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600, SDL_WINDOW_BORDERLESS);
	SDL_Renderer *myRenderer = SDL_CreateRenderer(myWindow, -1, 0);
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
	SDL_RenderClear(myRenderer);
	SDL_RenderPresent(myRenderer);
	SDL_Delay(2000);
	return 0;
}
