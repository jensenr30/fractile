#include <stdio.h>
#include <SDL2/SDL.h>
#include "utilities.h"

void error(char *errstr){
	FILE *fp = fopen("error.txt","a");
	fprintf( fp, "Ticks= %7.3f\t %s\n", ((float)SDL_GetTicks()/1000.0), errstr );
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
