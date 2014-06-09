#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "graphics.h"
#include "globals.h"
#include "graphics.h"
#include "map.h"
#include "rand.h"
#include "filter.h"
#include <time.h>

unsigned int windW = 600;
unsigned int windH = 600;


int main(int argc, char *argv[]){
	
	//--------------------------------------------------
	// set up surfaces, textures, renderers, windows,
	//--------------------------------------------------
	// set all surfaces, textures, renderers, and windows to NULL initially to satify if statements that free memory that isn't needed any more.
	// this prevents the game from crashing at startup.
	SDL_Surface *mapSurface = NULL;
	SDL_Texture *mapTexture = NULL;
	myWindow = NULL;
	myRenderer = NULL;
	myTexture = NULL;
	
	
	sgenrand(time(NULL));
	
	
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) return -99;
	
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
	
	//SDL_Texture *glider = load_image_to_texture("glider.jpg");
	
	if(myTexture == NULL){
		error("main() could not create texture using SDL_CreateTexture");
		return -2;
	}
	
	if(myTexture == NULL){
		error("main() could not create glider texture using SDL_CreateTexture");
		return -2;
	}
	
	
	// origin block.
	struct blockData origin;
	
	
	//--------------------------------------------------
	// event handling
	//--------------------------------------------------
	// this keeps tack of whether or not a new map should be generated
	byte makeNewMap = 1;
	// this records if the user wants to quit
	byte quit = 0;
	// this keeps track of if a mouse buttons is held down
	byte down = 0;
	// this keeps track of if the map needs to be smoothed.]
	byte smoothMap = 0;
	// this keeps track of what key was pressed (used only inside the event evaluation loop)
	byte key = 0;
	
	while(quit == 0){
		while(SDL_PollEvent(&event)){
			// if there is a mouse button down event,
			if(event.type == SDL_MOUSEBUTTONDOWN){
				down++;
			}
			else if(event.type == SDL_MOUSEBUTTONUP){
				down--;
			}
			else if(event.type == SDL_KEYDOWN){
				key = event.key.keysym.sym;
				if(key == SDLK_s){
					smoothMap = 1;
				}
				if(key == SDLK_ESCAPE) quit = 1;
				if(key == SDLK_q) makeNewMap = 1;
			}
			else if(event.type == SDL_QUIT){
				quit = 1;
			}
		}
		
		if(makeNewMap||smoothMap){
			// generate new map in origin
			if(makeNewMap)block_random_fill(&origin, -10, 20);
			
			//block_print_to_file(&origin, "origin.txt");
			
			// smooth the map
			//block_smooth(&origin, 0.5);
			if(smoothMap) filter_lowpass_2D_f((float *)origin.elevation, NULL, BLOCK_WIDTH, BLOCK_HEIGHT, 0.5);
			
			//clear old surface
			if(mapSurface != NULL)SDL_FreeSurface(mapSurface);
			// generate image of map
			mapSurface = create_surface(BLOCK_WIDTH, BLOCK_HEIGHT);
			// print map to mapSurface
			map_print(mapSurface, &origin);
			
			// clear the old texture if it exists
			if(mapTexture != NULL)SDL_DestroyTexture(mapTexture);
			// create a texture for the map data
			mapTexture = SDL_CreateTextureFromSurface(myRenderer, mapSurface);
			makeNewMap = 0;
			smoothMap = 0;
		}
		// render the mapTexture to the window
		SDL_RenderCopy(myRenderer, mapTexture, NULL, NULL);
		
		// display the renderer's result on the screen
		SDL_RenderPresent(myRenderer);
		SDL_RenderClear(myRenderer);
	}
	
	//--------------------------------------------------
	// clean up
	//--------------------------------------------------
	SDL_FreeSurface(mapSurface);
	SDL_DestroyTexture(mapTexture);
	// clean up all SDL subsystems and other non-SDL systems and global memory.
	clean_up();
	
	
	return 0;
}





