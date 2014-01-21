#include <stdio.h>
#include "SDL/SDL.h"
#include "bool.h"

// this is a single data point
struct pointData{
	float x;
	float y;
};



// maximum number of characters a plot can be
#define MAX_TITLE_LENGTH 256
// the number of lines that come BEFORE the actual data.
// these are the so called "meta lines" because they are data concerning the graph itself (not the actual data being plotted)
#define META_LINES 11

/// these are different line connection styles (lcs's)
/// these describe how the data points will be connected graphically
#define lcs_linear		1	// connect the linearly from the first data point to the last regardless of their proximity
#define lcs_nearest		2	// connect the data points according to their nearest neighbor on the x-axis
#define lcs_all			3	// this will print n factorial (n!) number of lines connecting every single point.

// this is the structure that contains all the necessary data for a single plot
struct plotData{
	
	//----------------------------------------------------
	//data printed in plot file
	//----------------------------------------------------
	
	// title of the graph
	char title[MAX_TITLE_LENGTH+1];
	// title of the x axis
	char xtitle[MAX_TITLE_LENGTH+1];
	// title of the y axis
	char ytitle[MAX_TITLE_LENGTH+1];
	
	// plot log scale (true) or plot linear scale (false)
	bool logScale;
	// print axes (true) or don't print axes (false)
	bool printAxes;
	
	// color of the plot (AARRGGBB) AA=alpha, RR=red, GG=green, BB=blue.
	Uint32 colorPlot;
	// color of the background
	Uint32 colorBack;
	// the radius (in pixels) of the data points. 1=1 pixel (smallest) 
	float pointSize;
	// what size line should be used?
	float lineSize;
	// what line connection style should be used?
	int lineStyle;
	// this tells us how many other points any one point has connections to.
	int linesPerPoint;
	
	// this holds the data for the plot (the floating point numbers)
	struct pointData *dataPoints;
	
	//----------------------------------------------------
	//data NOT printed in plot file
	//----------------------------------------------------
	
	// number of data point entries
	unsigned int entries;
};


void input_line(FILE *, char *);
void plot_load(FILE *, struct plotData *);
void plot(struct plotData *, SDL_Surface *);
