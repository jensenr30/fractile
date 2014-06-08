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
#define ERROR_FORMAT "Ticks= %7.3f\t %s\n"
// format with 1 integer type data argument
#define ERROR_FORMAT_D "Ticks= %7.3f\t %s%d\n"
// format with 1 floating point type data argument
#define ERROR_FORMAT_F "Ticks= %7.3f\t %s%f\n"


#include <SDL2/SDL.h>

void error(char *errstr);
void error_d(char *errstr, int data);
void error_f(char *errstr, float data);


int init_SDL();
SDL_Surface *create_surface(int witdth, int height);

void clean_up();
