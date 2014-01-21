#include "plot.h"
#include "globals.h"


void input_line(FILE *f, char *c){
	// test input arguments
	if(f == NULL){
		if(c != NULL){
			strcpy(c, "Invalid FILE pointer.");
		}
		return;
	}
	if(c == NULL) return;
	
	//input line
	int i;
	for(i=0; i<MAX_TITLE_LENGTH; i++){		// input characters.
		c[i] = fgetc(f);
		if(c[i] == EOF || c[i] == '\n' || c[i] == '\0'){
			c[i] = '\0';
			break;
		}
	}
	c[MAX_TITLE_LENGTH] = '\0';				// terminate string.
}




// this will transfer the data stored in a file into a plotData structure.
// the input file pointer must be opened for reading.
// the plotData pointer must point to a valid plotData structure.

void plot_load(FILE *f, struct plotData *p){
	// handle bad input
	if(f == NULL){
		if(p != NULL){
			strcpy(p->title, "Invalid File Pointer");
		}
		return;
	}
	if(p == NULL) return;
	
	// find out how many data entries there are
	int n=0;
	char inputChar;
	while(1){
		inputChar = fgetc(f);
		if(inputChar == EOF || inputChar == '\0') break;
		else if(inputChar == '\n') n++;
	}
	p->entries = n - META_LINES;
	
	rewind(f); // roll back the file pointer back to the beginning.
	
	if(f == NULL || p == NULL) return;
	input_line(f, p->title);				// input graph title.
	input_line(f, p->xtitle);				// input x-axis title.
	input_line(f, p->ytitle);				// input y-axis title.
	fscanf(f, "%d", (int*)&p->logScale);	// input logScale.
	fscanf(f, "%d", (int*)&p->printAxes);	// input printAxes.
	fscanf(f, "%x", &p->colorPlot);			// input plot color.
	fscanf(f, "%x", &p->colorBack);			// input background color.
	fscanf(f, "%d", &p->pointSize);			// input pointSize.
	fscanf(f, "%d", &p->lineSize);			// input lineSize.
	fscanf(f, "%d", &p->lineStyle);			// input line connection style.
	fscanf(f, "%d", &p->linesPerPoint);		// input lines per point.
	
	// allocate space for data points in memory
	p->dataPoints = malloc(p->entries*sizeof(struct pointData));
	
	for(n=0; n<p->entries; n++){
		fscanf(f, "%f,%f\n",&(p->dataPoints[n].x), &(p->dataPoints[n].y));
	}
}


void plot(struct plotData *p, SDL_Surface *s){
	
	struct plotData *dat = p;
	SDL_Surface *dest = s;
	// put something here to print the plot ;) 
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.w = SCREEN_WIDTH;
	screenRect.h = SCREEN_HEIGHT;
	
	//----------------------------------------------------
	// print background
	//----------------------------------------------------
	
	SDL_FillRect(dest, &screenRect, dat->colorBack);
	int i;
	
	//----------------------------------------------------
	// print points
	//----------------------------------------------------
	
	for(i=0; i<dat->entries; i++){
		draw_circle(dest, (float)dat->dataPoints[i].x, (float)dat->dataPoints[i].y, (float)dat->pointSize, dat->colorPlot);
		//set_pixel(dest, (int)dat->dataPoints[i].x, (int)dat->dataPoints[i].y, dat->colorPlot);
	}
	
	//----------------------------------------------------
	// print lines
	//----------------------------------------------------
	
	if(dat->lineStyle == lcs_linear){
		for(i=1; i<dat->entries; i++){
			draw_line(dest, (int)dat->dataPoints[i].x, (int)dat->dataPoints[i].y, (int)dat->dataPoints[i-1].x, (int)dat->dataPoints[i-1].y, dat->lineSize, dat->colorPlot);
		}
	}
	// need to implement other line connection styles here
	
}




