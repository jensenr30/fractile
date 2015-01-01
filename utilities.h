//--------------------------------------------------
// error stuff
//--------------------------------------------------
// write errors to error.txt
#define ERROR_FILE "error.txt"
// always append.
#define ERROR_FILE_MODE "a"
// gametime timestamp
#define ERROR_TIMESTAMP ((float)SDL_GetTicks()/1000.0)
// general format
#define ERROR_FORMAT "Seconds= %7.3f\t%s\n"
// format with 1 integer type data argument
#define ERROR_FORMAT_D "Seconds= %7.3f\t%s\t%d\n"
// format with 1 floating point type data argument
#define ERROR_FORMAT_F "Seconds= %7.3f\t%s\t%f\n"

//--------------------------------------------------
// gamelog stuff
//--------------------------------------------------
// this is the file that gamelog prints to
#define GAMELOG_FILE "gamelog.txt"
// this is how the gamelog prints
#define GAMELOG_FILE_MODE "a"
// gametime timestamp
#define GAMELOG_TIMESTAMP ((float)SDL_GetTicks()/1000.0)
// this is the gamelog format
#define GAMELOG_FORMAT "Seconds = %7.3f\t%s\n"
#define GAMELOG_FORMAT_D "Seconds = %7.3f\t%s\t%d\n"
#define GAMELOG_FORMAT_F "Seconds = %7.3f\t%s\t%f\n"


#include <SDL2/SDL.h>


void error(char *errstr);
void error_d(char *errstr, int data);
void error_f(char *errstr, float data);

void gamelog(char *gamestr);
void gamelog_d(char *gamestr, int data);

int init_SDL();
SDL_Surface *create_surface(int witdth, int height);

void clean_up();
