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
void fractal_print(SDL_Surface *dest, struct fractalData *f, double x, double y){
	
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
	fractal_iteration(dest, f, -xmin*scale, -ymin*scale, scale/f->scale, 1);
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

