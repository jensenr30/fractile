#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Windows.h"
#include "bool.h"
#include "general_functions.h"
#include "plot.h"
#include "globals.h"

/*
/// this handles an error.
void handle_error(int type, char *msg){
	char fullMsg[256];
	
	switch(type){
	case e_loadfile:
		strcpy(fullMsg, "Error loading:   ");
		strcat(fullMsg, msg);
		MessageBox(NULL, fullMsg, "Error loading file", MB_ICONERROR);
		break;
	case e_surface_creation:
		MessageBox(NULL, msg, "Error creating surface",MB_ICONERROR);
		break;
	case e_scalingFactor:
		MessageBox(NULL, msg, "Error scaling surface", MB_ICONERROR);
		break;
	default:
		MessageBox(NULL, msg, "General Error", MB_ICONWARNING);
		break;
	}
}
*/

/// this function checks to see if a file is empty or not.
// returns true if it is empty.
// returns false if it is NOT empty.

bool is_empty_file(char *relativeFilePath){
	FILE *datFile = fopen(relativeFilePath,"r");	// attempt to open the file
	if(datFile == NULL) return true;				// if there is no file, then the file is empty.
	
	#if(DEBUG_IS_EMPTY_FILE)
	fprintf(debugFile,"\n\nchecking if %s is empty:\n\t",relativeFilePath);
	#endif
	
	// check to see if the first character is an end of File character
	char c;
	while(1){
		c = fgetc(datFile);
		if( c == EOF ){
			#if(DEBUG_IS_EMPTY_FILE)
			fprintf(debugFile, "first character is EOF. returning true.\n");
			#endif
			fclose(datFile);	// shut the door on the way out
			return true;		// the file is empty
		}
		else{
			#if(DEBUG_IS_EMPTY_FILE)
			fprintf(debugFile,"first character is (char)%c = (int)%d\n. returning false.",c,(int)c);
			#endif
			fclose(datFile);	// shut the door on the way out
			return false;		// the file is NOT empty
		}
	}
}


// clean up and terminate the program
void quit_game(Uint32 quitFlag){
	clean_up();
	exit(quitFlag);
}


 
 void set_window_size(int w, int h){
	screen = SDL_SetVideoMode( w, h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE );

	//If there was an error setting up the screen
	if(screen == NULL )
	{
		exit(111);
	}
}
 
 
 
 
SDL_Surface *load_image(char* filename){
    //Temporary storage for the image that is loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image with either SDL_image or LoadBMP. comment-out the one you are not using
    loadedImage = IMG_Load( filename );
    //loadedImage = SDL_LoadBMP( filename );
    
    //If the image was loaded correctly
    if( loadedImage != NULL ){
        // Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        // set correct alpha properties
        //SDL_SetAlpha(optimizedImage, 0, 0xff);
        //Free the old image
        SDL_FreeSurface(loadedImage);
    }
    // if the image was not loaded correctly
    else{
		//handle_error(e_loadfile, filename);
    }
    
    //Return the optimized image
    return optimizedImage;
}


//this returns a pointer to an SDL_Surface
SDL_Surface *create_surface(int width, int height){
	
	// try to create surface
	SDL_Surface *retSurf = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xff000000);
	
	// check to see if the surface creation went well
	if(retSurf == NULL){
		quit_game(9);
		return NULL;
	}
	
	SDL_Surface *retSurfAlpha = SDL_DisplayFormatAlpha(retSurf);
	// delete old surface
	SDL_FreeSurface(retSurf);
	return retSurfAlpha;
}

int init(){
	//Initialize all subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){
		return false;
	}
	
	//Set up the screen
	set_window_size(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//If there was an error setting up the screen
	if(screen == NULL ){
		return false;
	}
	
	//Set the window caption
	SDL_WM_SetCaption( "GridVenture 1.0", NULL );
	
	
	
	//If everything initialized fine
	return true;
}

int load_files(){
	/*
	//Load the image
	image = load_image( "box.png" );
	
	//if there was an error in loading the image
	if( image == NULL )
	{
		return false;
	}
	*/
	
	
	
	//Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        MessageBox(NULL, "Error in initializing TTF (True Type Font) library", "TTF Error", MB_OK);
        return false;
    }
    
    //open font file
    font22 = TTF_OpenFont( "resources\\fonts\\FreeMonoBold.ttf", 22 );
    font16 = TTF_OpenFont( "resources\\fonts\\FreeMonoBold.ttf", 16);
    
    if (font22 == NULL || font16 == NULL)
    {
        MessageBox(NULL, "Could not load FreeMonoBold.ttf", "Error loading font", MB_OK);
    }
	
	
	//If everthing loaded fine
	return true;
}





void clean_up(){
	SDL_FreeSurface(screen);
	
	// de-allocate plot memory
	//free(myPlot.dataPoints);
	
	//Quit SDL
	SDL_Quit();
	TTF_Quit();
}

/// returns a random number from lowBound to highBound.
// e.g. if you call:
//		get_rand(3,7);
// then you could get: 3, 4, 5, 6, 7 as a return value.
//#define get_rand(lowBound, highBound) (rand()%((highBound) - (lowBound) + 1) + (lowBound))

int get_rand(int lowBound, int highBound){
	
	// if the lowBound is higher than the highBound, then flip them around and return that.
	if(highBound < lowBound){
		return get_rand(highBound, lowBound);
	}
	
	return ( rand() % (highBound-lowBound+1) ) + lowBound;
}





/// when you call this function, it returns a 1 or a 0.
/// your chances of getting a '1' returned are chance/100,000
/// for example, if call:
/// 	roll_ht(5000);
/// then there is a 5000/100000 chance (5%) that roll_ht will return a 1.
/// "roll_ht" means, "roll hundred thousand"
int roll_ht(int chance){
	if(chance > get_rand(0,99999)) return 1; // return 1 if the number the user gave you is greater than a random number between 0 and 99999.
	return 0; // otherwise, return 0
}

/// this evaluates if a coordinate (x,y) is inside the given rectangle
bool within_rect(SDL_Rect *datrect, int x, int y){
	if(x >= datrect->x && x <= datrect->x+datrect->w && y >= datrect->y && y <= datrect->y+datrect->h)
		return true;
	else
		return false;
}

/// this function prints a big red box in the middle of the screen.
// it is nice for debugging things, but serves no gameplay purpose
void print_red_box(SDL_Surface *dest){
	SDL_Rect testRect;
	testRect.x = SCREEN_WIDTH /4;
	testRect.y = SCREEN_HEIGHT/4;
	testRect.w = SCREEN_WIDTH /2;
	testRect.h = SCREEN_HEIGHT/2;
	SDL_FillRect(dest, &testRect, 0xffff0000);
}

