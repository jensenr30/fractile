#include <stdio.h>
#include "map.h"
#include "utilities.h"
#include <string.h>
#include "rand.h"

/// print a mapBlockData
// returns 1 on successful print to file
// this function is to print the data from a map in a readable format.
// THIS IS NOT FOR SAVING MAP DATA
short map_block_print_to_file(struct mapBlockData *mapBlock, char *fileName){
	
	// open a text file to write to and append (appending should never really happen though).
	FILE *fp = fopen(fileName, "a");
	
	if(fp == NULL){
		// create an error message to write to file.
		char message[512];
		strcpy(message, "map_block_print_to_file() could not open file \"");
		strcat(message, fileName);
		strcat(message, "\"");
		error(message);
		// return that the map_block_print_to_file() didn't get the job done.
		return 0;
	}
	
	if(mapBlock == NULL){
		error("map_block_random_fill() could not fill mapBlock. mapBlock = NULL.");
		return 0;
	}
	
	
	// write all elevation data
	int j, i;
	for(i=0; i<MAP_BLOCK_WIDTH; i++){
		for(j=0; j<MAP_BLOCK_HEIGHT; j++){
			fprintf(fp, "%10.10f\t", mapBlock->elevation[j][i]);
		}
		// new line
		fprintf(fp, "\n");
	}
	
	// wrote mapBlock to file successfully.
	return 1;
}




/// throws random data into mapBlockData
short map_block_random_fill(struct mapBlockData *mapBlock, float range_low, float range_high){
	
	// shiffle around the range values if they are not right.
	if(range_low > range_high){
		float temp = range_high;
		range_high = range_low;
		range_low = temp;
	}
	
	// check for mapBlock pointer being NULL.
	if(mapBlock == NULL){
		error("map_block_random_fill() could not fill mapBlock. mapBlock = NULL.");
		return 0;
	}
	
	int j, i;
	
	// this will generate data that will statistically average zero.
	// the data will not average zero for each set of numbers however.
	for(j=0; j<MAP_BLOCK_HEIGHT; j++){
		for(i=0; i<MAP_BLOCK_WIDTH; i++){
			// generate a random number between range_low and range_high.
			mapBlock->elevation[j][i] = ((genrand()%100001)/100000.0)*(range_high-range_low) + range_low;//((rand()%100001)/100000.0)*(range_higher-range_lower)-((range_higher+range_lower)/2);
		}
	}
	
	// generated random data in mapBlock successfully.
	return 1;
}








