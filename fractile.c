#include <SDL\SDL.h>
#include "fractile.h"
#include "graphics.h"
#include "globals.h"




// this is the recursive function that will draw the fractal.
void fractal_iteration(SDL_Surface *dest, struct fractalData *f, double entryx, double entryy, double scale, int iter){
	
	int i;
	// print all the vectors
	for(i=0; i<f->numbVectors; i++){
		draw_line(dest, entryx+(f->vects[i].x0)*f->scale, entryy+(f->vects[i].y0)*scale, entryx+(f->vects[i].x0+f->vects[i].x)*scale, entryy+(f->vects[i].y0+f->vects[i].y)*scale, f->thickness*scale, f->color1);
	}
	
	// return if you are done with all iterations of the fractal
	if(iter == f->iterations) return;
	
	// recursively call more fractal iteration functions at the exit points of this iteration.
	for(i=0; i<f->numbExits; i++){
		fractal_iteration(dest, f, entryx+f->exits[i].x*scale, entryy+f->exits[i].y*scale, scale*f->scale, iter+1);
	}
}


// dest is the SDL_Surface to which everything will be printed
// f is a pointer to the fractal structure that contains all the information about the fractal that will be printed.
// entryx and entryy are the <x,y> coordinates of the first entry point of this iteration of the fractal
void fractal_print(SDL_Surface *dest, struct fractalData *f){
	
	// make sure the fractal and the destination surface are valid
	if(f == NULL || dest == NULL) return;
	
	// make sure numbVectors isn't larger than the cap
	if(f->numbVectors >= MAX_VECTORS_PER_FRACTAL) f->numbVectors = MAX_VECTORS_PER_FRACTAL-1;
	// make sure numbExits   isn't larger than the cap
	if(f->numbExits >= MAX_EXITS_PER_FRACTAL) f->numbExits = MAX_EXITS_PER_FRACTAL-1;
	
	//-------------------------------------------------------------------------------
	// fit the fractal to the screen
	//-------------------------------------------------------------------------------
	double xvmax=0; // largest single vector movement in the +x direction
	double xvmin=0; // largest single vector movement in the -x direction
	double yvmax=0; // largest single vector movement in the +y direction
	double yvmin=0; // largest single vector movement in the -y direction
	
	double xemax=0; // largest single exit movement in the +x direction
	double xemin=0; // largest single exit movement in the -x direction
	double yemax=0; // largest single exit movement in the +y direction
	double yemin=0; // largest single exit movement in the -y direction
	
	double xmax=0; // total movement in the +x direction
	double xmin=0; // total movement in the -x direction
	double ymax=0; // total movement in the +y direction
	double ymin=0; // total movement in the -y direction
	
	int i;
	for(i=0; i<f->numbVectors; i++){
		if(f->vects[i].x+f->vects[i].x0 > xvmax) xvmax = f->vects[i].x+f->vects[i].x0;
		if(f->vects[i].y+f->vects[i].y0 > yvmax) yvmax = f->vects[i].y+f->vects[i].y0;
		if(f->vects[i].x+f->vects[i].x0 < xvmin) xvmin = f->vects[i].x+f->vects[i].x0;
		if(f->vects[i].y+f->vects[i].y0 < yvmin) yvmin = f->vects[i].y+f->vects[i].y0;
	}
	
	for(i=0; i<f->numbExits; i++){
		if(f->exits[i].x > xemax) xemax = f->exits[i].x;
		if(f->exits[i].y > yemax) yemax = f->exits[i].y;
		if(f->exits[i].x < xemin) xemin = f->exits[i].x;
		if(f->exits[i].y < yemin) yemin = f->exits[i].y;
	}
	
	
	double scale;
	for(i=1, scale=1; i<=f->iterations; i++, scale*=f->scale){
		xmax += (xvmax+xemax)*scale;
		ymax += (yvmax+yemax)*scale;
		xmin += (xvmin+xemin)*scale;
		ymin += (yvmin+yemin)*scale;
	}
	
	// the maximum/minimum cannot pass over the origin because the maximum/minimum is at LEAST the origin.
	if(xmin>0)xmin = 0.0f;
	if(ymin>0)ymin = 0.0f;
	if(xmax<0)xmax = 0.0f;
	if(ymax<0)ymax = 0.0f;
	
	double xdiff = xmax - xmin;
	double ydiff = ymax - ymin;
	
	double xscale = SCREEN_WIDTH/(xdiff);
	double yscale = SCREEN_HEIGHT/(ydiff);
	
	if(xscale < yscale){
		scale = xscale;
	}
	else{
		scale = yscale;
	}
	
	
	
	// begin drawing the fractal
	fractal_iteration(dest, f, -((xmax+xmin)*scale)/2 + SCREEN_WIDTH/2, -((ymax+ymin)*scale)/2 + SCREEN_HEIGHT/2, scale/f->scale, 1);
}


void fractal_random(struct fractalData *f, int maxVects, int maxIterations){
	/*
    f->vects[0].x0= 0;
    f->vects[0].y0= 0;
    f->vects[0].x=  200;
    f->vects[0].y=  200;
    f->vects[1].x0= 0;
    f->vects[1].y0= 0;
    f->vects[1].x=  200;
    f->vects[1].y=  0;
    f->vects[2].x0= 0;
    f->vects[2].y0= 0;
    f->vects[2].x=  0;
    f->vects[2].y=  200;
    f->vects[3].x0= 0;
    f->vects[3].y0= 0;
    f->vects[3].x=  -200;
    f->vects[3].y=  200;
    
    f->numbVectors = 4;
    
    f->exits[0].x = 200;
    f->exits[0].y = 200;
    f->exits[1].x = 200;
    f->exits[1].y = 0;
    f->exits[2].x = 0;
    f->exits[2].y = 200;
    f->exits[3].x = -200;
    f->exits[3].y = 200;
    
    f->numbExits = 4;
    */
    f->iterations = maxIterations;
    int i;
    f->numbVectors = f->numbExits = rand()%(maxVects-2)+ 2;
    for(i=0; i<f->numbVectors; i++){
		f->vects[i].x0 = 0;//rand()%400 + 20;
		f->vects[i].y0 = 0;//rand()%400 + 20;
		f->vects[i].x = (rand()%400 + 20)*(rand()%2-0.5)*2;
		f->vects[i].y = (rand()%400 + 20)*(rand()%2-0.5)*2;
		f->exits[i].x = f->vects[i].x0 + f->vects[i].x;
		f->exits[i].y = f->vects[i].y0 + f->vects[i].y;
    }
}


void init_fractal_editor(){
	
	
}


// f if the fractal that we will print the vectors for 
/// returns true if the user's mouse click or event happened inside of the fractal editor.
bool fractal_editor(SDL_Surface *dest, struct fractalData *f, int x, int y, int editorEvent){
	
	static SDL_Rect genRect;
	static bool firstTime = true;
	static SDL_Rect editor;
	// this tells us if the editor window is open
	static bool editorOpen=true;
	// this tells us what button the editor is currently on.
	static int editorButton=0;
	// this is the current width of the editor
	static int currentWidth;
	
	static SDL_Rect buttons[EDITOR_BUTTONS_NUMBER_OF];
	int i;
	
	
	// handle all first-time-through things
	if(firstTime){
		editor.x = 0;
		editor.y = 0;
		currentWidth = EDITOR_DEFAULT_WIDTH;
		
		for(i=0; i<EDITOR_BUTTONS_NUMBER_OF; i++){
			buttons[i].w = buttons[i].h = EDITOR_BUTTON_SIZE;
			buttons[i].y = i*EDITOR_BUTTON_SIZE;
		}
		firstTime = false;
	}
	
	//check for toggling
	if(editorEvent == ee_toggle) editorOpen^=1;
	
	
	
	// these things need to be evaluated every time through the loop.
	for(i=0; i<EDITOR_BUTTONS_NUMBER_OF; i++){
		buttons[i].x = (currentWidth+EDITOR_TITLE_BAR_HEIGHT)*editorOpen;
	}
	editor.h = SCREEN_HEIGHT;
	editor.w = currentWidth*editorOpen;
	
	
	// if the destination surface is null, don't bother printing anything
	if(dest == NULL) return true;
	
	//-------------------------------------------------------------------------------
	// printing stuff
	//-------------------------------------------------------------------------------
	// print buttons
	for(i=0; i<EDITOR_BUTTONS_NUMBER_OF; i++){
		SDL_FillRect(dest, &buttons[i], EDITOR_COLOR_PRIMARY*((i%2)^1) + EDITOR_COLOR_SECONDARY*(i%2) );
	}
	
	if(editorOpen){
		// print the background colors of the editor
		SDL_FillRect(dest, &editor, EDITOR_COLOR_PRIMARY);
		
		genRect.x=editor.x+currentWidth;
		genRect.y=editor.y;
		genRect.w = EDITOR_SCROLL_BAR_WIDTH;
		genRect.h = editor.h;
		SDL_FillRect(dest, &genRect, EDITOR_COLOR_SCROLL_BAR_BACKGROUND);
		
		genRect.x = editor.x;
		genRect.w = genRect.h = currentWidth;
		for(i=0; i<f->numbVectors && i*currentWidth<SCREEN_HEIGHT; i++){
			
			// if i is an odd integer, print an secondary color background square
			if(i%2){
				genRect.y = editor.y + i*currentWidth + EDITOR_TITLE_BAR_HEIGHT;
				SDL_FillRect(dest, &genRect, EDITOR_COLOR_SECONDARY);
			}
			
		}
	}
	// the user did not interact with the editor.
	return false;
}

