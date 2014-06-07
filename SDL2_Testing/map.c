#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include <string.h>
#include "rand.h"
#include "graphics.h"

/// print a mapBlockData
// returns 1 on successful print to file
// this function is to print the data from a map in a readable format.
// THIS IS NOT FOR SAVING MAP DATA
short block_print_to_file(struct mapBlockData *mapBlock, char *fileName){
	
	// open a text file to write to and append (appending should never really happen though).
	FILE *fp = fopen(fileName, "a");
	
	if(fp == NULL){
		// create an error message to write to file.
		char message[512];
		strcpy(message, "block_print_to_file() could not open file \"");
		strcat(message, fileName);
		strcat(message, "\"");
		error(message);
		// return that the map_block_print_to_file() didn't get the job done.
		return 0;
	}
	
	if(mapBlock == NULL){
		error("block_random_fill() could not fill mapBlock. mapBlock = NULL.");
		return 0;
	}
	
	
	// write all elevation data
	int j, i;
	for(j=0; j<MAP_BLOCK_HEIGHT; j++){
		for(i=0; i<MAP_BLOCK_WIDTH; i++){
			fprintf(fp, "%10.10f\t", mapBlock->elevation[i][j]);
		}
		// new line
		fprintf(fp, "\n");
	}
	
	// wrote mapBlock to file successfully.
	return 1;
}




/// throws random data into mapBlockData
// returns 0 on success
// returns 1 when the mapBlock is a NULL pointer.
short block_random_fill(struct mapBlockData *mapBlock, float range_low, float range_high){
	
	// check for mapBlock pointer being NULL.
	if(mapBlock == NULL){
		error("block_random_fill() could not fill mapBlock. mapBlock = NULL.");
		return 1;
	}
	
	// shiffle around the range values if they are not right.
	if(range_low > range_high){
		float temp = range_high;
		range_high = range_low;
		range_low = temp;
	}
	
	int j, i;
	
	// this will generate data that will statistically average zero.
	// the data will not average zero for each set of numbers however.
	for(i=0; i<MAP_BLOCK_WIDTH; i++){
		for(j=0; j<MAP_BLOCK_HEIGHT; j++){
			// generate a random number between range_low and range_high.
			mapBlock->elevation[i][j] = ((genrand()%100001)/100000.0)*(range_high-range_low) + range_low;//((rand()%100001)/100000.0)*(range_higher-range_lower)-((range_higher+range_lower)/2);
		}
	}
	
	// generated random data in mapBlock successfully.
	return 0;
}




// returns 0 on success
// returns -1 when mapBlock == NULL
short block_create_children(struct mapBlockData *source){
	
	// check for source pointer being NULL.
	if(source == NULL){
		error("block_create_children() could not create children. source = NULL.");
		return -1;
	}
	
	int child;
	for(child=0; child<MAP_BLOCK_CHILDREN; child++){
		
		// allocate space for the child in memory.
		source->children[child] = ((struct mapBlockData *)malloc(sizeof(struct mapBlockData)));
		if(source->children[child] == NULL){
			error("block_create_children() could not create child. malloc returned NULL pointer. Out of memory?");
			return child;
		}
		else{
			// make the child point to its parent.
			(source->children[child])->parent = source;
		}
	}
	
	// generated children successfully.
	return 0;
}



/// this creates a list of all of the map blocks that have been created during program run time.
/// this function will record every new map block that is generated.
/// before the program closes, this function will need to be called to clean up all of these blocks.
// if operation = bc_collect, then it will collect 
// returns 0 on success
// returns 1 if sent a null block pointer
// returns 
short block_collector(struct mapBlockData *source, int operation){
	
	// if the operation is bc_collect
	if(operation == bc_collect){
		// check for source pointer being NULL.
		if(source == NULL){
			error("block_collector() could not collect source. source = NULL.");
			return -1;
		}
		else{
			
			// TODO: log block to file
			
		}
		
	}
	else if(operation == bc_clean_up){
		
		// TODO: free all blocks in block list
		
	}
	else{
		error_d("block_collector() received invalid operation. operation = ", operation);
	}
	

	return 0;
}




/// this will print an image of a mapblock to a surface MAP_BLOCK_WIDTH x MAP_BLOCK_HEIGHT pixels
// returns 0 on success
// returns 1 on NULL dest surface
// returns 2 on NULL mapBlock
short map_print(SDL_Surface *dest, struct mapBlockData *source){
	
	if(dest == NULL){
		error("block_collector() could not print to dest. dest = NULL.");
		return 1;
	}
	if(source == NULL){
		error("block_collector() could not print source. source = NULL.");
		return 2;
	}
	
	int i, j;
	for(i=0; i<MAP_BLOCK_WIDTH; i++){
		for(j=0; j<MAP_BLOCK_HEIGHT; j++){
			set_pixel(dest, i, j, 0xff000000|(8*((int)source->elevation[i][j] + 10)));
		}
	}
	
	return 0;
}








