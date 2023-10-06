#include "fractal.h"
#include "globals.h"
#include "graphics.h"
#include "rand.h"
#include "sidebar.h"
#include "utilities.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MOUSE_BUTTONS 5

// global event stuff
SDL_Event event;

int main(int argc, char *argv[]) {

    error("\n\n\n\n== PROGRAM START ======================================================\n\n\n\n"
    );

    //--------------------------------------------------
    // initial gamelog write
    //--------------------------------------------------
    gamelog_startup(argc, argv);

    set_window_width(1300);
    set_window_height(731);

    int i;
    //--------------------------------------------------
    // set up surfaces, textures, renderers, windows,
    //--------------------------------------------------

    SDL_Window *myWindow = NULL;
    SDL_Renderer *myRenderer = NULL;
    SDL_Texture *myTexture = NULL;
    SDL_Surface *mySurface = create_surface(get_window_width(), get_window_height());
    SDL_Texture *fadeTexture = NULL;
    SDL_Surface *fadeSurface = create_surface(
        get_window_width(), get_window_height()
    ); // this is a surface used to fade out old mySurface data. It is used to allow persistence of
       // image.
    uint8_t fadeAlpha = 0x20;
    uint32_t fadeEverySoManyFrames =
        2; // this specifies how often a fade is performed. This allows the decay rate to be dragged
           // out even longer than minimum non-zero alpha value. This says, every this many frames,
           // the fade is done once.
    uint32_t fadeFrames =
        0; // this is the frame fade counter. It keeps track of when fades need to be performed.
    SDL_FillRect(
        fadeSurface, NULL, (fadeAlpha << 24)
    ); // this specifies the color data of the surface. the first 8 bits are the alpha value. the
       // rest should all be zeros so that it is black.

    // these things are not currently useful.
    // I am doing things the (probably) wrong way by doing ablitsurface where the whole surface is
    // black with some level of alpha.
    fadeTexture = SDL_CreateTextureFromSurface(myRenderer, fadeSurface);
    SDL_SetTextureAlphaMod(myTexture, fadeAlpha);
    SDL_SetTextureBlendMode(myTexture, SDL_BLENDMODE_BLEND);

    // get a random seed based on the current time. This prevents the random numbers that are
    // generated from being the same "random" numbers each time the program is started.
    sgenrand(time(NULL));

    // initialize all SDL stuff
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        error("SDL_Init() failed!");
        return -99;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        error("TTF_Init() failed!");
        return -13;
    }

    // create window
    myWindow = SDL_CreateWindow(
        "Fractile 1.0",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        get_window_width(),
        get_window_height(),
        SDL_WINDOW_RESIZABLE
    );
    myRenderer = SDL_CreateRenderer(myWindow, -1, 0);

    if (myWindow == NULL) {
        error("main() could not create myWindow using SDL_CreateWindow");
        return -1;
    }
    if (myRenderer == NULL) {
        error("main() could not create myRenderer using SDL_CreateRenderer");
        return -2;
    }

    SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
    SDL_RenderClear(myRenderer);
    SDL_RenderPresent(myRenderer);

    SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
    SDL_RenderClear(myRenderer);
    SDL_RenderPresent(myRenderer);

    //--------------------------------------------------
    // event handling
    //--------------------------------------------------

    // this records if the user wants to quit
    byte quit = 0;
    const int keysSize = 256;
    // this array keeps track of what variables were JUST pushed.
    // A value of 1 will be set to keys that were just stroked.
    // after an iteration through the loop, all keys are reset to 0.
    byte keys[keysSize];
    // this keeps track of which keys are HELD DOWN
    int_fast8_t keysHeld[keysSize];
    // these keep track of where the mouse is
    float x = 0, y = 0;
    // these keep track of where the mouse was just moments ago.
    float xlast = 0, ylast = 0;

    // todo: refactor: struct
    // these two 2-element arrays keep the data concerning the state of the right and left mouse
    // buttons. EX: mouse[SDL_BUTTON_LEFT][0] is the CURRENT state of the LEFT mouse button
    // mouse[SDL_BUTTON_LEFT][1] is the LAST	state of the LEFT mouse button
    // mouse[SDL_BUTTON_RIGHT][0] is the CURRENT state of the RIGHT mouse button
    // mouse[SDL_BUTTON_RIGHT][1] is the LAST	state of the RIGHT mouse button
    // etc...
    int mouse[MOUSE_BUTTONS][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

    // todo: refactor: struct
    // this is similar to mouse. however, this is used to store where the user clicked when a
    // particular mouse button was clicked. mouseClick[SDL_BUTTON_LEFT][0] is the x position where
    // the user clicked the left  mouse button. mouseClick[SDL_BUTTON_LEFT][1] is the y position
    // where the user clicked the left  mouse button. mouseClick[SDL_BUTTON_RIGHT][0] is the x
    // position where the user clicked the RIGHT mouse button. etc...
    int mouseClick[MOUSE_BUTTONS][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

    //--------------------------------------------------
    // these variables keep track of time and FPS
    //--------------------------------------------------
    Uint32 ticksLast = 0;
    Uint32 ticksNow = 0;
    Uint32 frames = 0;
    Uint32 FPS = 0;

    //--------------------------------------------------
    // these variables are for fractal stuff
    //--------------------------------------------------
    // declare a fractal.
    struct fractal myFractal;
    // print the size of the fractal structure to file
    gamelog_d("sizeof(struct fractal) (in bytes) = ", sizeof(struct fractal));
    // set the fractal to default.
    fractal_set_default(&myFractal);
    // float decayFact = 0.7;

    myFractal.numberOfChildren = 2;
    myFractal.numberOfShapes = 2;
    myFractal.shapes[0].type = fst_circle;
    myFractal.shapes[0].x[0] = -200;
    myFractal.shapes[0].y[0] = 200;
    myFractal.shapes[0].x[1] = -203;
    myFractal.shapes[0].y[1] = 217;
    myFractal.shapes[0].radius = 18;
    myFractal.shapes[0].color = 0xFF6Fc030;

    myFractal.shapes[1].type = fst_circle;
    myFractal.shapes[1].radius = 15;
    myFractal.shapes[1].color = 0xFF7f7f00;

    myFractal.shapes[2].type = fst_circle;
    myFractal.shapes[2].radius = 10;
    myFractal.shapes[2].color = 0xFFfca000;

    myFractal.shapes[3].type = fst_pixel;
    myFractal.shapes[3].radius = 5;
    myFractal.shapes[3].color = 0xFFab3411;

    myFractal.shapes[4].color = 0xFFff8840;
    myFractal.shapes[4].type = fst_pixel; // fst_line;
    myFractal.shapes[4].radius = 25;

    myFractal.shapes[5].type = fst_pixel; // fst_circle;
    myFractal.shapes[5].color = 0xFFaa5588;

    myFractal.children[0].x = -100;
    myFractal.children[0].y = 0;

    myFractal.children[0].twist = 180;
    myFractal.children[1].twist = 0;
    myFractal.children[2].twist = 0;
    myFractal.children[3].twist = 0;

    myFractal.children[0].scale = 0.998; // sqrt(0.5);
    myFractal.children[1].scale = sqrt(0.5);
    myFractal.children[2].scale = sqrt(0.5);
    myFractal.children[3].scale = sqrt(0.5);

    float zoomFactor = 1.05;

    //--------------------------------------------------
    // sidebar stuff
    //--------------------------------------------------
    // create sidebar
    struct sidebar mySideBar;
    // init sidebar
    sidebar_init(&mySideBar);
    // create a surface to print the sidebar onto.
    SDL_Surface *mySideBarSurface = create_surface(mySideBar.rect.w, mySideBar.rect.h);
    // point the sidebar at the fractal you are playing with
    mySideBar.frac = &myFractal;
    // load the fonts that the sidebar will use
    sidebar_load_fonts(&mySideBar);

    //--------------------------------------------------
    // these variables are for panning / fractal part modification
    //--------------------------------------------------
    // these keep track of the mouse position when panning/part modification began
    float pan_orig_x;
    float pan_orig_y;
    // these two keep track of the original <x,y> values of the fractal when panning
    float orig_x;
    float orig_y;
    // keeps track of whether or not we are panning
    char panning = 0;
    // keeps track of whether or now we are modifying a part of the fractal
    char modPart = 0;
    // this is used to store a version of myFractal in
    struct fractal myFractalOrig;
    //  this is used to point to the fractal that is being operated on
    // (this will be set to myFractal or myFractalOrig depending on if the user is editing things or
    // not)
    struct fractal *fracOp;
    // this keeps track of wheter or not the user wants the image to persist
    uint8_t persist = 1;

    // number of times to recursively render the fractal
    int iterations = 3;

    //--------------------------------------------------
    // This is where the user input is interpreted.
    // The fractal rendering routines are called from here.
    // mouse motion, mouse clicks, keystrokes, etc...
    //--------------------------------------------------
    while (quit == 0) {

        // check to see which fractal structure will be operated on.
        // this depends on whether or not the user is modifying a part or panning.
        if (panning || modPart) {
            fracOp = &myFractalOrig;
        } else {
            fracOp = &myFractal;
        }

        // reset all keystroke values
        for (i = 0; i < keysSize; i++) {
            keys[i] = 0;
        }
        while (SDL_PollEvent(&event)) {

            //--------------------------------------------------
            // if the mouse has moved,
            //--------------------------------------------------
            if (event.type == SDL_MOUSEMOTION) {
                x = event.motion.x;
                y = event.motion.y;
            }

            // if the event is NOT relevant to the sidebar,
            if (!sidebar_evaluate(&mySideBar, &event, x, y)) {
                //--------------------------------------------------
                // if a mouse button is pressed,
                //--------------------------------------------------
                if (event.button.type == SDL_MOUSEBUTTONDOWN) {

                    x = event.button.x;
                    y = event.button.y;

                    // set mouse button states
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // record that the left mouse button is down
                        mouse[SDL_BUTTON_LEFT][0] = 1;
                        // record where the left mouse button was clicked
                        mouseClick[SDL_BUTTON_LEFT][0] = x;
                        mouseClick[SDL_BUTTON_LEFT][1] = y;
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT) {
                        // record that the right mouse button is down
                        mouse[SDL_BUTTON_RIGHT][0] = 1;
                        // record where the right mouse button was clicked
                        mouseClick[SDL_BUTTON_RIGHT][0] = x;
                        mouseClick[SDL_BUTTON_RIGHT][1] = y;
                    }

                    else if (event.button.button == SDL_BUTTON_MIDDLE) {
                        // record that the middle mouse button is down
                        mouse[SDL_BUTTON_MIDDLE][0] = 1;
                        // record where the middle mouse button was clicked
                        mouseClick[SDL_BUTTON_MIDDLE][0] = x;
                        mouseClick[SDL_BUTTON_MIDDLE][1] = y;
                    }
                }

                //--------------------------------------------------
                // if a mouse button has been released,
                //--------------------------------------------------
                else if (event.type == SDL_MOUSEBUTTONUP) {
                    // set mouse button states
                    if (event.button.button == SDL_BUTTON_LEFT) mouse[SDL_BUTTON_LEFT][0] = 0;
                    if (event.button.button == SDL_BUTTON_RIGHT) mouse[SDL_BUTTON_RIGHT][0] = 0;
                    if (event.button.button == SDL_BUTTON_MIDDLE) mouse[SDL_BUTTON_MIDDLE][0] = 0;
                }

                //--------------------------------------------------
                // if the mouse wheel is turned,
                //--------------------------------------------------
                else if (event.type == SDL_MOUSEWHEEL) {
                    if (event.wheel.y > 0)
                        fractal_zoom(fracOp, zoomFactor, x, y);
                    else
                        fractal_zoom(fracOp, 1 / zoomFactor, x, y);
                }

                //--------------------------------------------------
                // if a key has been pressed,
                //--------------------------------------------------
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym >= 0) {
                        // set that character, number, or letter to 1.
                        keys[(event.key.keysym.sym) % keysSize] = 1;
                        keysHeld[(event.key.keysym.sym) % keysSize] = 1;
                    }
                    switch (event.key.keysym.sym) {
                        case SDLK_DOWN:
                            iterations--;
                            if (iterations < 0) iterations = 0;
                            break;
                        case SDLK_UP:
                            iterations++;
                            break;
                        case SDLK_h: // toggle image persistance (works really well with twist)
                            persist = !persist;
                            break;
                        default:
                            break;
                    }
                }

                //--------------------------------------------------
                // if a key has been released,
                //--------------------------------------------------
                else if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym >= 0) {
                        // set that character, number, or letter to 0.
                        keysHeld[(event.key.keysym.sym) % keysSize] = 0;
                    }
                }

                //--------------------------------------------------
                // if there has been a window event
                //--------------------------------------------------
                else if (event.type == SDL_WINDOWEVENT) {
                    // if the window has been closed,
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                        quit = 1;
                    }
                    // if the window was resized,
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        // the new window width and height need to be recorded
                        set_window_width(event.window.data1);
                        set_window_height(event.window.data2);
                        // also the surface needs to be resized right away
                        if (mySurface != NULL) SDL_FreeSurface(mySurface);
                        mySurface = create_surface(get_window_width(), get_window_height());

                        // also the fade surface needs to be resized right away
                        if (fadeSurface != NULL) SDL_FreeSurface(fadeSurface);
                        fadeSurface = create_surface(get_window_width(), get_window_height());
                        SDL_FillRect(
                            fadeSurface, NULL, (fadeAlpha << 24)
                        ); // this specifies the color data of the surface. the first 8 bits are the
                           // alpha value. the rest should all be zeros so that it is black.

                        // we need to resize the height of the sidebar
                        mySideBar.rect.h = get_window_height();
                        // the old sidebar surface will no longer serve us.
                        if (mySideBarSurface != NULL) SDL_FreeSurface(mySideBarSurface);
                        // create a new one with the proper dimensions.
                        mySideBarSurface = create_surface(mySideBar.rect.w, mySideBar.rect.h);
                    }
                }
            }

            //--------------------------------------------------
            // if there has been a quit event,
            //--------------------------------------------------
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        //--------------------------------------------------
        // if the user just left-clicked
        //--------------------------------------------------
        if (mouse[SDL_BUTTON_LEFT][0] && !mouse[SDL_BUTTON_LEFT][1]) {
            // store original x,y positions of mouse
            orig_x = x;
            orig_y = y;

            // see if the user intended to click a part of the fractal
            int clickedPart = fractal_select_point(mySurface, &myFractal, x, y);

            // if a valid part was found
            if (clickedPart >= 1) {
                // enter the part modification mode
                modPart = 1;
                // make a copy of the fractal's original state when modification happened.
                // now myFractal will be set to some modified version of myFractalOrig
                fractal_copy(&myFractal, &myFractalOrig);
            }
            // otherwise, no valid part was found
            else {
                // enter the screen panning mode
                panning = 1;
                // record the original <x,y> position of the fractal on the screen
                pan_orig_x = myFractal.x;
                pan_orig_y = myFractal.y;
            }
        }

        // if the user is currently modifying a part,
        if (modPart) {
            // if the user has the left mouse button held down
            if (mouse[SDL_BUTTON_LEFT][0]) {
                // draw_circle(mySurface, 200, 200, 66.67, 0xFF0000FF);
                //  copy the original state of myFractal into the myFractal
                fractal_copy(&myFractalOrig, &myFractal);
                // modify myFractal
                fractal_select_modify(
                    &myFractal, (x - orig_x) / myFractalOrig.zoom, (y - orig_y) / myFractalOrig.zoom
                );
            }
            // otherwise,
            else {
                modPart = 0;
            }
        }

        // if the user is currently panning,
        if (panning) {
            // if the user has the left mouse button held down
            if (mouse[SDL_BUTTON_LEFT][0]) {
                myFractal.x = pan_orig_x + x - orig_x; //*myFractal.zoom;
                myFractal.y = pan_orig_y + y - orig_y; //*myFractal.zoom;
            }
            // otherwise,
            else {
                panning = 0;
            }
        }

        myFractal.children[0].twist += 0.2; // rand_range_f(-5,5);
        myFractal.children[1].twist -= 0.2;
        myFractal.children[2].twist -= 1;
        myFractal.children[3].twist += 1;

        fractal_render_children(&myFractal, mySurface, 3);

        fractal_render(&myFractal, mySurface, 4);

        // TODO uncomment this:
        // sidebar_render(&mySideBar,mySideBarSurface);

        SDL_BlitSurface(mySideBarSurface, NULL, mySurface, &mySideBar.rect);

        //--------------------------------------------------
        // this area of the code puts whatever was rendered
        // into mySurface onto the screen for the user to see
        //--------------------------------------------------

        myTexture = SDL_CreateTextureFromSurface(myRenderer, mySurface);
        // if the user does not want the image to persist,
        if (!persist) {
            // clear the old texture if it exists
            if (mySurface != NULL) SDL_FreeSurface(mySurface);
            mySurface = create_surface(get_window_width(), get_window_height());
        }

        //--------------------------------------------------
        // Perform fading routine
        //--------------------------------------------------

        // if you are supposed to fade on this frame,
        if (fadeFrames == 0) {
            // apply the fading surface
            SDL_BlitSurface(fadeSurface, NULL, mySurface, NULL);
        }

        // record that another frame has gone by for the fade counter
        fadeFrames++;
        // if you have waited the desired amount,
        if (fadeFrames >= fadeEverySoManyFrames) {
            // reset the fade counter and be sure to fade the next time
            fadeFrames = 0;
        }

        //--------------------------------------------------
        // Render Texture -> Screen
        //--------------------------------------------------

        // render the mySurface to the myWindow
        SDL_RenderCopy(myRenderer, myTexture, NULL, NULL);

        // fade it a little bit
        // SDL_RenderCopy(myRenderer, fadeTexture, NULL, NULL);

        if (myTexture != NULL) SDL_DestroyTexture(myTexture);
        // display the renderer's result on the screen and clear it when done
        SDL_RenderPresent(myRenderer);
        SDL_RenderClear(myRenderer);

        //--------------------------------------------------
        // records mouse position and button states
        //--------------------------------------------------

        // store the current x and y values and use them as the "last" values in the next iteration
        // of the loop
        xlast = x;
        ylast = y;
        // save all current mouse states.
        for (i = 0; i < MOUSE_BUTTONS; i++) {
            // set the last state of this mouse button to the current state (for the next loop
            // iteration)
            mouse[i][1] = mouse[i][0];
        }

        //--------------------------------------------------
        // tracks FPS of the game
        //--------------------------------------------------
        // increase the frame counter (as we have just successfully rendered a frame)
        frames++;
        // get the current value of ticks
        ticksNow = SDL_GetTicks();
        // if more than a second has passed since the last FPS calculation,
        if (ticksNow - ticksLast >= 1000) {
            FPS = (int)(frames / (((float)(ticksNow - ticksLast)) / 1000.0f) + 0.5f);
            // reset frame counter
            frames = 0;
            // set the last tick time to the current time (this will make the program wait 1 second
            // until recalculating the FPS)
            ticksLast = ticksNow;
            gamelog_d("FPS =", FPS);
        }
    }

    //--------------------------------------------------
    // clean up
    //--------------------------------------------------
    if (myRenderer != NULL) SDL_DestroyRenderer(myRenderer);
    if (myTexture != NULL) SDL_DestroyTexture(myTexture);
    if (myWindow != NULL) SDL_DestroyWindow(myWindow);

    // clean up all SDL subsystems and other non-SDL systems and global memory.
    clean_up();

    gamelog("Program exiting normally.");
    return 0;
}
