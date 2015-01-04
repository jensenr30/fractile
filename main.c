#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "graphics.h"
#include "globals.h"
#include "rand.h"
#include <time.h>
#include "fractal.h"


#define MOUSE_BUTTONS 5


int main(int argc, char *argv[]){
	
	
	
	error("\n\n\n\n== PROGRAM START ======================================================\n\n\n\n");
	
	//--------------------------------------------------
	// initial gamelog write
	//--------------------------------------------------
	gamelog_startup(argc, argv);
	
	windW = 1300;
	windH = 731;
	
	int i;
	//--------------------------------------------------
	// set up surfaces, textures, renderers, windows,
	//--------------------------------------------------
	
	SDL_Window *myWindow = NULL;
	SDL_Renderer *myRenderer = NULL;
	SDL_Texture *myTexture = NULL;
	SDL_Surface *mySurface = create_surface(windW, windH);
	
	sgenrand(time(NULL));
	
	
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) return -99;
	
		// set network window
	myWindow = SDL_CreateWindow("FractalMap - Network Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windW, windH, SDL_WINDOW_RESIZABLE);
	myRenderer = SDL_CreateRenderer(myWindow, -1, 0);
	
	if(myWindow == NULL){
		error("main() could not create myWindow using SDL_CreateWindow");
		return -1;
	}
	if(myRenderer == NULL){
		error("main() could not create myRenderer using SDL_CreateRenderer");
		return -2;
	}
	
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
	SDL_RenderClear(myRenderer);
	SDL_RenderPresent(myRenderer);
	
	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
	SDL_RenderClear(myRenderer);
	SDL_RenderPresent(myRenderer);
	
	
	//--------------------------------------------------
	// event handling
	//--------------------------------------------------
	
	// this records if the user wants to quit
	byte quit = 0;
	const int keysSize = 256;
	// this array keeps track of what variables were JUST pushed.
	// A value of 1 will be set to keys that were just stroked.
	// after an iteration through the loop, all keys are reset to 0.
	byte keys[keysSize];
	// this keeps track of which keys are HELD DOWN
	int_fast8_t keysHeld[keysSize];
	// these keep track of where the mouse is
	int x=0, y=0;
	// these keep track of where the mouse was just moments ago.
	int xlast=0, ylast=0;
	
	// these two 2-element arrays keep the data concerning the state of the right and left mouse buttons.
	// EX:
		// mouse[SDL_BUTTON_LEFT][0] is the CURRENT state of the LEFT mouse button
		// mouse[SDL_BUTTON_LEFT][1] is the LAST    state of the LEFT mouse button
		// mouse[SDL_BUTTON_RIGHT][0] is the CURRENT state of the RIGHT mouse button
		// mouse[SDL_BUTTON_RIGHT][1] is the LAST    state of the RIGHT mouse button
	int mouse[MOUSE_BUTTONS][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
	
	// this is similar to mouse. however, this is used to store where the user clicked when a particular mouse button was clicked.
		// mouseClick[SDL_BUTTON_LEFT][0] is the x position where the user clicked the left  mouse button.
		// mouseClick[SDL_BUTTON_LEFT][1] is the y position where the user clicked the left  mouse button.
		// mouseClick[SDL_BUTTON_RIGHT][0] is the x position where the user clicked the RIGHT mouse button.
	// etc...
	int mouseClick[MOUSE_BUTTONS][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
	
	
	//--------------------------------------------------
	// these variables keep track of time and FPS
	//--------------------------------------------------
	
	Uint32 ticksLast = 0;
	Uint32 ticksNow = 0;
	Uint32 frames = 0;
	Uint32 FPS = 0;
	
	
	//--------------------------------------------------
	// these variables are for fractal stuff
	//--------------------------------------------------
	
	// declare a fractal.
	struct fractal myFractal;
	// set the fractal to default.
	fractal_set_default(&myFractal);
	float zoomFactor = 1.05;
	
	//--------------------------------------------------
	// this is the fabled "main while() loop"
	// This is where the user input is interpreted.
	// mouse motion, mouse clicks, keystrokes, etc...
	//--------------------------------------------------
	
	while(quit == 0){
		
		// reset all keystroke values
		for(i=0; i<keysSize; i++){
			keys[i] = 0;
		}
		while(SDL_PollEvent(&event)){
			// if there is a mouse button down event,
			if(event.button.type == SDL_MOUSEBUTTONDOWN){
				// set mouse button states
				if(event.button.button == SDL_BUTTON_LEFT){
					// record that the left mouse button is down
					mouse[SDL_BUTTON_LEFT][0] = 1;
					// record where the left mouse button was clicked
					mouseClick[SDL_BUTTON_LEFT][0] = x;
					mouseClick[SDL_BUTTON_LEFT][1] = y;
				}
				else if(event.button.button == SDL_BUTTON_RIGHT){
					// record that the right mouse button is down
					mouse[SDL_BUTTON_RIGHT][0] = 1;
					// record where the right mouse button was clicked
					mouseClick[SDL_BUTTON_RIGHT][0] = x;
					mouseClick[SDL_BUTTON_RIGHT][1] = y;
				}
				else if(event.button.button == SDL_BUTTON_MIDDLE){
					// record that the middle mouse button is down
					mouse[SDL_BUTTON_MIDDLE][0] = 1;
					// record where the middle mouse button was clicked
					mouseClick[SDL_BUTTON_MIDDLE][0] = x;
					mouseClick[SDL_BUTTON_MIDDLE][1] = y;
				}
			}
			else if(event.type == SDL_MOUSEBUTTONUP){
				// set mouse button states
				if(event.button.button == SDL_BUTTON_LEFT) mouse[SDL_BUTTON_LEFT][0] = 0;
				if(event.button.button == SDL_BUTTON_RIGHT) mouse[SDL_BUTTON_RIGHT][0] = 0;
				if(event.button.button == SDL_BUTTON_MIDDLE) mouse[SDL_BUTTON_MIDDLE][0] = 0;
			}
			else if(event.type == SDL_MOUSEWHEEL){
				if(event.wheel.y > 0)
					myFractal.zoom *= zoomFactor;
				else
					myFractal.zoom /= zoomFactor;
			}
			else if(event.type == SDL_MOUSEMOTION){
				x = event.motion.x;
				y = event.motion.y;
			}
			else if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym >= 0){
					// set that character, number, or letter to 1.
					keys[(event.key.keysym.sym)%keysSize] = 1;
					keysHeld[(event.key.keysym.sym)%keysSize] = 1;
				}
			}
			else if(event.type == SDL_KEYUP){
				if(event.key.keysym.sym >= 0){
					// set that character, number, or letter to 0.
					keysHeld[(event.key.keysym.sym)%keysSize] = 0;
				}
			}
			// check for window events
			else if(event.type == SDL_WINDOWEVENT ){
				if(event.window.event == SDL_WINDOWEVENT_CLOSE){
					quit = 1;
				}
				
				// if the window was resized, 
				if( event.window.event == SDL_WINDOWEVENT_RESIZED){
					// the new window width and height need to be recorded
					windW = event.window.data1;
					windH = event.window.data2;
					// also the surface needs to be changed right away
					if(mySurface != NULL)SDL_FreeSurface(mySurface);
					mySurface = create_surface(windW, windH);
				}
			}
			else if(event.type == SDL_QUIT){
				quit = 1;
			}
		}
		
		
		
		
		// render the fractal
		fractal_render(&myFractal, mySurface, myFractal.x, myFractal.y, myFractal.zoom);
		
		
		
		
		//--------------------------------------------------
		// this area of the code puts whatever was rendered
		// into mySurface onto the screen for the user to see
		//--------------------------------------------------
		
		
		
		
		// generate texture for the block network
		myTexture = SDL_CreateTextureFromSurface(myRenderer, mySurface);
		// clear the old texture if it exists
		if(mySurface != NULL)SDL_FreeSurface(mySurface);
		mySurface = create_surface(windW, windH);
		// render the mySurface to the myWindow
		SDL_RenderCopy(myRenderer, myTexture, NULL, NULL);
		if(myTexture != NULL)SDL_DestroyTexture(myTexture);
		// display the renderer's result on the screen and clear it when done
		SDL_RenderPresent(myRenderer);
		SDL_RenderClear(myRenderer);
		
		
		
		// store the current x and y values and use them as the "last" values in the next iteration of the loop
		xlast = x;
		ylast = y;
		
		// save all current mouse states.
		for(i=0; i<MOUSE_BUTTONS; i++){
			// set the last state of this mouse button to the current state (for the next loop iteration)
			mouse[i][1] = mouse[i][0];
		}
		
		// increase the frame counter (as we have just successfully rendered a frame)
		frames++;
		// get the current value of ticks
		ticksNow = SDL_GetTicks();
		// if more than a second has passed since the last FPS calculation,
		if(ticksNow - ticksLast >= 1000){
			FPS = (int)(frames/(((float)(ticksNow-ticksLast))/1000.0f) + 0.5f );
			// reset frame counter
			frames = 0;
			// set the last tick time to the current time (this will make the program wait 1 second until recalculating the FPS)
			ticksLast = ticksNow;
			gamelog_d("FPS =", FPS);
		}
		
	}
	
	
	//--------------------------------------------------
	// clean up
	//--------------------------------------------------
	if(myRenderer != NULL) SDL_DestroyRenderer(myRenderer);
	if(myTexture  != NULL) SDL_DestroyTexture (myTexture);
	if(myWindow   != NULL) SDL_DestroyWindow  (myWindow);
	
	// clean up all SDL subsystems and other non-SDL systems and global memory.
	clean_up();
	
	
	return 0;
}





