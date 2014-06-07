
#define ERROR_FILE "error.txt"
#define ERROR_FILE_MODE "a"
#define ERROR_TIMESTAMP ((float)SDL_GetTicks()/1000.0)
#define ERROR_FORMAT "Ticks= %7.3f\t %s\n"
#define ERROR_FORMAT_D "Ticks= %7.3f\t %s%d\n"
#include <SDL2/SDL.h>

void error(char *errstr);
void error_d(char *errstr, int data);


int init_SDL();
SDL_Surface *create_surface(int witdth, int height);
