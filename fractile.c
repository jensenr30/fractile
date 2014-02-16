#include <SDL\SDL.h>
#include "fractile.h"
#include "graphics.h"
#include "globals.h"
#include "math.h"
#include "math_custom.h"




// this is the recursive function that will draw the fractal.
void fractal_iteration(SDL_Surface *dest, struct fractalData *f, double entryx, double entryy, double scale, double twist, int iter){
	
	int i;
	
	if(twist == 0){		// if there is no twist angle
		// print all the vectors
		for(i=0; i<f->numbVectors; i++){
			draw_line(dest, entryx+(f->vects[i].x0)*scale, entryy+(f->vects[i].y0)*scale, entryx+(f->vects[i].x0+f->vects[i].x)*scale, entryy+(f->vects[i].y0+f->vects[i].y)*scale, f->thickness*scale, f->color1);
		}
	}
	else{				// if there is some twist
		double r, r0;
		double theta, theta0;
		// print all the vectors
		for(i=0; i<f->numbVectors; i++){
			// calculate magnitude of the components vector
			r = sqrt(f->vects[i].x*f->vects[i].x + f->vects[i].y*f->vects[i].y);
			// calculate the angle of the of the components vector with respect to the beginning of the vector
			if(r == 0)
				theta = 0.0f;
			else if(f->vects[i].x == 0)
				theta = PI/2;
			else
				theta = atan(f->vects[i].y/f->vects[i].x);
			if(f->vects[0].x<0) ;//theta+=PI;
			
			// calculate magnitude of the beginning of the vector
			r0 = sqrt(f->vects[i].x0*f->vects[i].x0 + f->vects[i].y0*f->vects[i].y0);
			// calculate the angle of the beginning of the vector with respect to the entry point
			if(r0 == 0)
				theta0 = 0;
			else if(f->vects[i].x0 == 0)
				theta0 = (PI/2.0);
			else
				theta0 = atan((f->vects[i].y0-entryy)/(f->vects[i].x0-entryx));
			if(f->vects[0].x0<0) theta0+=PI;
			// draw the line between the transformed coordinates
			draw_line(dest, entryx+(r0*cos(theta0+twist))*scale, entryy+(r0*sin(theta0+twist))*scale, entryx+(r0*cos(theta0+twist)+r*cos(theta+twist))*scale, entryy+(r0*sin(theta0+twist)+r*sin(theta+twist))*scale, f->thickness*scale, f->color1);
		}
	}
	// return if you are done with all iterations of the fractal
	if(iter == f->iterations) return;
	
	// recursively call more fractal iteration functions at the exit points of this iteration.
	for(i=0; i<f->numbExits; i++){
		if(twist == 0){
			fractal_iteration(dest, f, entryx+f->exits[i].x*scale, entryy+f->exits[i].y*scale, scale*f->scale, twist+f->twist, iter+1);
		}
		else{
			double r, theta;
			// print all the vectors
			for(i=0; i<f->numbVectors; i++){
				r = sqrt(f->exits[i].x*f->exits[i].x + f->exits[i].y*f->exits[i].y);
				if(r == 0)
					theta = 0.0f;
				else if(f->exits[i].x == 0)
					theta = (PI/2.0);
				else
					theta = atan((f->exits[i].y-entryy)/(f->exits[i].x-entryx));
				if(f->vects[0].x0<0) theta+=PI;
				fractal_iteration(dest, f, entryx+f->exits[i].x*scale, entryy+f->exits[i].y*scale, scale*f->scale, twist+f->twist, iter+1);
			}
		}
	}
}


// dest is the SDL_Surface to which everything will be printed
// f is a pointer to the fractal structure that contains all the information about the fractal that will be printed.
// entryx and entryy are the <x,y> coordinates of the first entry point of this iteration of the fractal
void fractal_print(SDL_Surface *dest, struct fractalData *f){
	
	int borderSize = 10;
	
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
	
	double xscale = (SCREEN_WIDTH-borderSize*2)/(xdiff);
	double yscale = (SCREEN_HEIGHT-borderSize*2)/(ydiff);
	
	if(xscale < yscale){
		scale = xscale;
	}
	else{
		scale = yscale;
	}
	
	
	
	// begin drawing the fractal
	fractal_iteration(dest, f, -((xmax+xmin)*scale)/2 + SCREEN_WIDTH/2, -((ymax+ymin)*scale)/2 + SCREEN_HEIGHT/2, scale/f->scale, 0, 1);
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
    f->twist = PI/100;//f->twist = (((rand()%20001)-10000)*PI)/10000;
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
	
	//these keep track of where the mouse was last time this function was called.
	static int xlast;
	static int ylast;
	// this is a general purpose rectangle.
	static SDL_Rect genRect;
	// this keeps track of whether or not this is the first time running through this function.
	static bool firstTime = true;
	// this is a rectangle than describes the position <x,y> and size <w,h> of the editor virtual window.
	static SDL_Rect editor;
	// this tells us if the editor window is open
	static bool editorOpen=true;
	// this tells us what button the editor is currently on.
	static int editorButton=0;
	// this is the current width of the editor
	static int currentWidth = EDITOR_DEFAULT_WIDTH;
	// this is for keeping track of which vector is currently selected
	static int currentVector=0;
	// this is the scale at which the vectors in the editor window are printed to the screen (pixels/vector-size)
	static double scale;
	// this keeps track of what state the right mouse button was/is in.
	static bool rightMouseButton=false;
	// these keep track of where the right mouse button was pressed down.
	static int xRightMouseInitial;
	static int yRightMouseInitial;
	static double xVectInit;
	static double yVectInit;
	static double dragSpeed = 0.1f;
	
	// this keeps track of whether or not we need to calculate the vector-to-pixel scaling factor
	bool calculateVectorScale=false;
	
	static SDL_Rect buttons[EDITOR_BUTTONS_NUMBER_OF];
	int i;
	
	
	// handle all first-time-through things
	if(firstTime){
		editor.x = 0;
		editor.y = 0;
		calculateVectorScale = true;
		for(i=0; i<EDITOR_BUTTONS_NUMBER_OF; i++){
			buttons[i].w = buttons[i].h = EDITOR_BUTTON_SIZE;
			buttons[i].y = i*EDITOR_BUTTON_SIZE;
		}
		firstTime = false;
	}
	
	//-------------------------------------------------------------------------------
	// check for user input
	//-------------------------------------------------------------------------------
	//check for toggling
	if(editorEvent == ee_toggle) editorOpen^=1;
	//check for selecting a vector
	if(editorOpen && x<currentWidth && y>=EDITOR_TITLE_BAR_HEIGHT && editorEvent == ee_left_click_down){
		currentVector = ((y-EDITOR_TITLE_BAR_HEIGHT)/currentWidth)%f->numbVectors;
	}
	//check for left clicking (clicking and dragging)
	if(editorEvent == ee_right_click_down){
		rightMouseButton = true;
		xRightMouseInitial = x;
		yRightMouseInitial = y;
		xVectInit = f->vects[currentVector].x;
		yVectInit = f->vects[currentVector].y;
	}
	if(editorEvent == ee_right_click_up){
		rightMouseButton = false;
		f->exits[currentVector].x = f->vects[currentVector].x = xVectInit + ((x-xRightMouseInitial)*dragSpeed)/scale;
		f->exits[currentVector].y = f->vects[currentVector].y = yVectInit + ((y-yRightMouseInitial)*dragSpeed)/scale;
		calculateVectorScale=true;
	}
	
	
	//-------------------------------------------------------------------------------
	// check to see if we need to calculate the scaling factor for printing the editor window vectors
	//-------------------------------------------------------------------------------
	if(calculateVectorScale){
		//-------------------------------------------------------------------------------
		//determine the longest magnitude vector in this vector
		//-------------------------------------------------------------------------------
		double yMaxMag=0.0f;
		double xMaxMag=0.0f;
		for(i=0; i<f->numbVectors; i++){
			if(fabs(f->vects[i].x)>xMaxMag) xMaxMag = fabs(f->vects[i].x);
			if(fabs(f->vects[i].y)>yMaxMag) yMaxMag = fabs(f->vects[i].y);
		}
		//-------------------------------------------------------------------------------
		// set the scale of the vectors in the editor sidebar
		//-------------------------------------------------------------------------------
		// this is the scale at which the vectors of the fractal will be printed in the editor
		// scale is in units of (pixels/vector-size) so that it can be multiplied by a vector's size and the units work out to be pixels.
		if(xMaxMag > yMaxMag){
			scale = currentWidth/(xMaxMag*2);
		}
		else{
			scale = currentWidth/(yMaxMag*2);
		}
	}
	
	//-------------------------------------------------------------------------------
	// check for scaling of the current vector
	//-------------------------------------------------------------------------------
	if(rightMouseButton){
		f->exits[currentVector].x = f->vects[currentVector].x = xVectInit + ((x-xRightMouseInitial)*dragSpeed)/scale;
		f->exits[currentVector].y = f->vects[currentVector].y = yVectInit + ((y-yRightMouseInitial)*dragSpeed)/scale;
	}
	
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
		
		
		//-------------------------------------------------------------------------------
		// print the sidebar
		//-------------------------------------------------------------------------------
		genRect.x = editor.x;
		genRect.w = genRect.h = currentWidth;
		for(i=0; i<f->numbVectors && i*currentWidth<SCREEN_HEIGHT; i++){
			
			// if i is an odd integer, print an secondary color background square
			if(i%2){
				genRect.y = editor.y + i*currentWidth + EDITOR_TITLE_BAR_HEIGHT;
				SDL_FillRect(dest, &genRect, EDITOR_COLOR_SECONDARY);
			}
			if(i==currentVector){
				genRect.y = editor.y + i*currentWidth + EDITOR_TITLE_BAR_HEIGHT;
				apply_outline(dest, &genRect, 2, EDITOR_CURRENT_VECTOR_OUTLINE_COLOR);
			}
			
			draw_line(dest, currentWidth/2, currentWidth*(i+0.5) + EDITOR_TITLE_BAR_HEIGHT, currentWidth/2 + f->vects[i].x*scale, currentWidth*(i+0.5) + EDITOR_TITLE_BAR_HEIGHT + f->vects[i].y*scale, f->thickness, f->color1);
		}
	}
	// the user did not interact with the editor.
	return false;
}

