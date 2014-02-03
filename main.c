#include "general_functions.h"
#include "globals.h"
#include "plot.h"



int main( int argc, char* argv[] ) 
{
	
	// initial screen sizes
	SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
	SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;
	
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
	//plot_load(dataFile, &myPlot);				// load plot data
	fclose(dataFile);							// close file
	
	
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
	
	//----------------------------------------------------
	// initialize lots of stuff
	//----------------------------------------------------
    if( init() == false ) return 1;				// make sure you can boot up the necessary libraries
    if( load_files() == false ) return 2;		// make sure all files are loaded corre
    
    
    
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
			}
			
            if( event.type == SDL_KEYDOWN ){		///keyboard event
                switch( event.key.keysym.sym ){
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
		
		
		
		//plot(&myPlot, screen);						// display plot
		plot_weird_heat(screen);
		
        //draw_circle(screen, 300.0, 300.0, 50, 0x00000000);
        
        //updates the screen
        SDL_Flip( screen );
        
        
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
