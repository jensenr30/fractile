#include "utilities.h"
#include "globals.h"
#include <stdio.h>

// this will log an error message to the error file
void error(char *errstr) {
    // open error log file
    FILE *fp = fopen(ERROR_FILE, ERROR_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, ERROR_FORMAT, ERROR_TIMESTAMP, errstr);
        // close error fille
        fclose(fp);
    }
}

// this will log an error message and an integer of data to the error file
void error_d(char *errstr, int data) {
    // open error log file
    FILE *fp = fopen(ERROR_FILE, ERROR_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, ERROR_FORMAT_D, ERROR_TIMESTAMP, errstr, data);
        // close error fille
        fclose(fp);
    }
}

// this will log an error message and a floating point number to the error file
void error_f(char *errstr, float data) {
    // open error log file
    FILE *fp = fopen(ERROR_FILE, ERROR_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, ERROR_FORMAT_F, ERROR_TIMESTAMP, errstr, data);
        // close error fille
        fclose(fp);
    }
}

// this will log an error message and a floating point number to the error file
void error_s(char *errstr, char *data) {
    // open error log file
    FILE *fp = fopen(ERROR_FILE, ERROR_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, ERROR_FORMAT_S, ERROR_TIMESTAMP, errstr, data);
        // close error fille
        fclose(fp);
    }
}

void gamelog(char *gamestr) {
    FILE *fp = fopen(GAMELOG_FILE, GAMELOG_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, GAMELOG_FORMAT, GAMELOG_TIMESTAMP, gamestr);
        // close error fille
        fclose(fp);
    }
}

void gamelog_d(char *gamestr, int data) {
    FILE *fp = fopen(GAMELOG_FILE, GAMELOG_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, GAMELOG_FORMAT_D, GAMELOG_TIMESTAMP, gamestr, data);
        // close error fille
        fclose(fp);
    }
}

void gamelog_f(char *gamestr, float data) {
    FILE *fp = fopen(GAMELOG_FILE, GAMELOG_FILE_MODE);
    // if the error file is valid,
    if (fp != NULL) {
        // write error log to error file
        fprintf(fp, GAMELOG_FORMAT_F, GAMELOG_TIMESTAMP, gamestr, data);
        // close error fille
        fclose(fp);
    }
}

void gamelog_startup(int argc, char **argv) {
    gamelog(
        "\n\n\n\n== PROGRAM START ======================================================\n\n\n\n"
    );
    gamelog_d("main() was sent argc =", argc);
    int arg;
    gamelog("START ARGV ARGUMENT LIST:");
    // print all arguments
    for (arg = 0; arg < argc; arg++) {
        gamelog(argv[arg]);
    }
    gamelog("END ARGV LIST");
}

// returns 1 if everything initialized correctly.
// returns 0 if anything screwed up.
int init_SDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("init_SDL(): couldn't initialize SDL using SDL_Init(SDL_INIT_EVERYTHING)");
        return 0;
    }
    return 1;
}

// this returns a pointer to an SDL_Surface
SDL_Surface *create_surface(int width, int height) {

    // try to create surface
    SDL_Surface *retSurf = SDL_CreateRGBSurface(
        SDL_SWSURFACE, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xff000000
    );

    // check to see if the surface creation went well
    if (retSurf == NULL) {
        error("create_surface() could not create surface");
        return NULL;
    }

    return retSurf;
}

void clean_up() {

    // destroy the main window, main renderer, and main texture.
    // SDL_DestroyRenderer(myRenderer);
    // SDL_DestroyTexture(myTexture);
    // SDL_DestroyWindow(myWindow);

    SDL_Quit();
}
