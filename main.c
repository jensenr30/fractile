#include "general_functions.h"
#include "graphics.h"
#include "globals.h"
#include "math_custom.h"
#include "math.h"
#include "fractile.h"
#include <stdlib.h>
#include <time.h>

int main( int argc, char* argv[] ) 
{
	//random seed
	srand(time(NULL));
	
	// initial screen sizes
	SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
	SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;
	
<<<<<<< Updated upstream
	//----------------------------------------------------
	//debugging the input arguments
	//----------------------------------------------------
	FILE *debugFile = fopen("debug.txt", "w");
	if(debugFile == NULL) return 5;
	fprintf(debugFile, "argc = %d\n",argc);
	int i;
	for(i=0; i<argc; i++){
		fprintf(debugFile, "%2d. %s\n",i,argv[i]);
	}
	fclose(debugFile);
	
	
	FILE *dataFile = fopen("data.txt", "r");	// open file
	if(dataFile == NULL) return 1;				// quit if plot file is null
	plot_load(dataFile, &myPlot);				// load plot data
	fclose(dataFile);							// close file
	
=======
>>>>>>> Stashed changes
	
	//----------------------------------------------------
	// VARIABLES USED IN MAIN()
	//----------------------------------------------------
    int x, y;									// this is the location of the player's mouse
    
    int mouseLeft[2] = {0,0};					// this keeps track of the user's left  mouse button state. [0]=current, [1]=previous
    int mouseRight[2] = {0,0};					// this keeps track of the user's right mouse button state. [0]=current, [1]=previous
	
	int ticksSinceLastFPSUpdate=0;				// time since last FPS update (ideally, goes from 0 to 1000 milliseconds and then resets)
	int cumulativeFrames = 0;					// this counts how many Frames there have been since the last
	int currentTicks = 0;						// this is how many 
	int quit = false;							//make sure the program waits for a quit
	float lines=2*PI;
	//----------------------------------------------------
	// initialize lots of stuff
	//----------------------------------------------------
    if( init() == false ) return 1;				// make sure you can boot up the necessary libraries
    if( load_files() == false ) return 2;		// make sure all files are loaded corre
    
    SDL_Rect screenRect;
    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.w = SCREEN_WIDTH;
    screenRect.h = SCREEN_HEIGHT;
    
    
    //----------------------------------------------------
	// testing fractal utilities
	//----------------------------------------------------
    struct fractalData myfractal;
    myfractal.scale = 0.5;
    myfractal.iterations = 3;
    myfractal.thickness = 5;
    myfractal.color1 = 0xff00ff00;
    fractal_random(&myfractal, 15, 5);
    
    
    //----------------------------------------------------
	// enter the main while loop of the game.
	//----------------------------------------------------
    while(1){
		
    	//While there's an event to handle
    	while( SDL_PollEvent( &event ) ){
			
    		//If the user has Xed out the window
    		if( event.type == SDL_QUIT || quit == true ){
				//Quit the program
				quit_game(0);
			}
			
            if( event.type == SDL_MOUSEBUTTONDOWN ){						/// mouse down
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseLeft[0] = 1;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseRight[0] = 1;
                }
                
                else if( event.button.button == SDL_BUTTON_WHEELUP ){
					// zoom in
                }
				else if( event.button.button == SDL_BUTTON_WHEELDOWN ){
					// zoom out
				}
				
            }
            else if(event.type == SDL_MOUSEBUTTONUP){						/// mouse up
				x = event.motion.x;
				y = event.motion.y;
            }
            else if( event.type == SDL_MOUSEMOTION ){						/// mouse motion
				x = event.motion.x;
				y = event.motion.y;
            }
            else if(event.type == SDL_VIDEORESIZE){							/// window resize
				
//				float new_cell_size = CELL_SIZE * event.resize.h/((float)SCREEN_HEIGHT); // adjust the pixel size.
//				if(new_cell_size - ((int)new_cell_size) >= 0.5f) CELL_SIZE = new_cell_size + 1;
//				else CELL_SIZE = new_cell_size;
				SCREEN_WIDTH = event.resize.w;
				SCREEN_HEIGHT = event.resize.h;
				set_window_size(event.resize.w, event.resize.h);		// set window to correct dimensions
				screenRect.w = event.resize.w;
				screenRect.h = event.resize.h;
			}
			
            if( event.type == SDL_KEYDOWN ){		///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_r: fractal_random(&myfractal, 7, 7); break;
				default: break;
				}
			}
			if( event.type == SDL_KEYUP ){								///keyboard event
                switch( event.key.keysym.sym ){
				default: break;
				}
			}
                
		
    	} // end while(event)
		//no more events to handle at the moment.
		
<<<<<<< Updated upstream
		
		
		plot(&myPlot, screen);						// display plot
		
        draw_circle(screen, 300.0, 300.0, 50, 0x00000000);
=======
		//draw_line(screen, -200,10,200,50,1,0xffff0000);
		//draw_line(screen, (int)(y*0.9),(int)(x*0.9),x,y,1,0xffff0000);
		//draw_line(screen, 1200,200,x,y,1,0xffff0000);
        //draw_circle(screen, 300.0, 300.0, 50, 0x00000000);
>>>>>>> Stashed changes
        
        //draw_line(screen, 200, 1200, 200,-200,1,0xffff0000);
        
        /*
        float theta;
        lines += 1;
        for(theta=0; theta<2*PI; theta = theta + 2*PI/lines){
			draw_line(screen,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,SCREEN_WIDTH*cos(theta)+SCREEN_WIDTH/2, SCREEN_WIDTH*sin(theta)+SCREEN_HEIGHT/2, 1, 0xff0000ff);
        }
        SDL_Delay(50);
        //updates the screen
        */
        
<<<<<<< Updated upstream
=======
        fractal_print(screen, &myfractal, 400, 100);
        SDL_Flip( screen );
        SDL_FillRect(screen, &screenRect, 0);
        
>>>>>>> Stashed changes
        //----------------------------------------------------------------------
		// FPS calculation and variable handling
		//----------------------------------------------------------------------
        currentTicks = SDL_GetTicks();
        // it is officially the next second
        if(currentTicks >= ticksSinceLastFPSUpdate + 1000){
			// calculate the FPS
			FPS = cumulativeFrames;//(cumulativeFrames*1000 ) / (currentTicks-ticksSinceLastFPSUpdate);
			cumulativeFrames=0;				// reset cumulative amount of frames
			ticksSinceLastFPSUpdate = currentTicks;	// reset the last FPS update to the number of ticks now.
        }
        cumulativeFrames++;
        mouseRight[1] = mouseRight[0];	// make the previous mouse state equal the current one (for the next loop iteration)
        mouseLeft[1]  = mouseLeft[0];	// ^
        
        
    }// end while(quit == false)


    // quit the game
    quit_game(0);

    return 0;
}
