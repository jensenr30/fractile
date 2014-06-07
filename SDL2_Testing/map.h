#include <SDL2/SDL.h>

#define MAP_BLOCK_CHILDREN		4
#define MAP_BLOCK_NEIGHBORS		8
#define MAP_BLOCK_SCALE_FACTOR	4
#define MAP_BLOCK_WIDTH			128
#define MAP_BLOCK_HEIGHT		128



struct mapBlockData{
	
	signed long long level;
	
	// this is the two dimensional array of elevation values for each block.
	// 16384 points of data (128 x 128)
	float elevation[MAP_BLOCK_WIDTH][MAP_BLOCK_HEIGHT];
	
	// this points to the block that this block is inside.
	struct mapBlockData *parent;
	
	// these are pointers to child blocks.
	// these are pointers to other blocks inside of this main block.
	// at the moment, each block will be split up into four smaller ones.
	struct mapBlockData *children[MAP_BLOCK_CHILDREN];
	
	// these are eight pointers to the eight neighbors on the same level.
	// 
	struct mapBlockData *neighbors[MAP_BLOCK_NEIGHBORS];
};


short block_print_to_file(struct mapBlockData *mapBlock, char *fileName);
short block_random_fill(struct mapBlockData *mapBlock, float range_lower, float range_higher);


/// these are definitions for the block_collector program.
// the function will either be collecting a list of all map blocks being generated (as they are generated)
// or the function will de-allocate (free) all of the data it has collected so far.
#define bc_collect		0
#define bc_clean_up		1

short block_collector(struct mapBlockData *source, int operation);
short map_print(SDL_Surface *dest, struct mapBlockData *source);
