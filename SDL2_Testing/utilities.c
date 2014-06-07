#include <stdio.h>
#include "utilities.h"


// this will log an error message to the error file
void error(char *errstr){
	FILE *fp = fopen(ERROR_FILE, ERROR_FILE_MODE);
	fprintf( fp, ERROR_FORMAT, ERROR_TIMESTAMP, errstr );
	fclose(fp);
}

// this will log an error message and an integer of data to the error file
void error_d(char *errstr, int data){
	FILE *fp = fopen(ERROR_FILE,ERROR_FILE_MODE);
	fprintf( fp, ERROR_FORMAT_D, ERROR_TIMESTAMP, errstr, data );
	fclose(fp);
}



// returns 1 if everything initialized correctly.
// returns 0 if anything screwed up.
int init_SDL(){
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ){
		error("init_SDL(): couldn't initialize SDL using SDL_Init(SDL_INIT_EVERYTHING)");
		return 0;
	}
	return 1;
}


//this returns a pointer to an SDL_Surface
SDL_Surface *create_surface(int width, int height){

	// try to create surface
	SDL_Surface *retSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xff000000);

	// check to see if the surface creation went well
	if(retSurf == NULL){
		error("create_surface() could not create surface");
		return NULL;
	}
	
	return retSurf;
}
