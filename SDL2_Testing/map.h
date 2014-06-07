

// ORIGINAL
#define MAP_BLOCK_DIVISIONS 4
#define MAP_BLOCK_SCALE_FACTOR 4
#define MAP_BLOCK_WIDTH 64
#define MAP_BLOCK_HEIGHT 64



struct mapBlockData{
	
	signed long long level;
	
	// this is the two dimensional array of elevation values for each block.
	// 4096 points of data (64 x 64)
	float elevation[MAP_BLOCK_HEIGHT][MAP_BLOCK_WIDTH];
	
	// this points to the block that this block is inside.
	struct mapBlockData *parent;
	
	// these are pointers to child blocks.
	// these are pointers to other blocks inside of this main block.
	// at the moment, each block will be split up into four smaller ones.
	struct mapBlockData *children[MAP_BLOCK_DIVISIONS];
};


short map_block_print_to_file(struct mapBlockData *mapBlock, char *fileName);
short map_block_random_fill(struct mapBlockData *mapBlock, float range_lower, float range_higher);
