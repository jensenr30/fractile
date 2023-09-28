#include "globals.h"

#include <SDL2/SDL.h>

static unsigned int width = 800;
static unsigned int height = 600;

unsigned int get_window_width(void) {
    return width;
}

unsigned int get_window_height(void) {
    return height;
}

int set_window_width(unsigned int input_width) {
    width = input_width;
}

int set_window_height(unsigned int input_height) {
    height = input_height;
}
