#include "filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utilities.h"
#include "globals.h"




//--------------------------------------------------
// filter_lowpass_2D_f() documentation
//--------------------------------------------------
/// this function will perform a low-pass filtering operation on floating point data in a two-dimensional signal.
// x is the input array x[width][height]
// y is the output array y[width][height]
// the filtering will be applied over the x axis (width) and the y axis (height).
// the function will return the filtered input signal data in the "y" floating point array.
// if y is NULL, it will overwrite x with a filtered version of x.

// tau is the time-constant (or, space-constant, if you prefer).
	// tau has units of array elements.
	// for example,
		//it is commonly said that it takes 5*tau for a step response of a traditional single-order low pass filter to approximately reach its final value.
		// so if this function is sent a tau value of 2.0 (elements), one would expect that it takes 10 (elements) for a step response to fully settle in the resulting filtered output signal.
	// another example:
		// if your audio is sampled at a rate of 44 kHz, that means that each element in your array of audio values is 22.7 microseconds apart. (22.7 microseconds/element)
		// so if you wanted a low pass with a time constant of 1 millisecond, you would need to pass a tau value of 1 millisecond*element/(22.7 microseconds) = 44.05 elements.
	// the formula works like this:
		// tau = time_constant * ( time / element )
	// or, for spacial filtering, the word time can be replaced with space:
		// tau = space_constant * ( space / element )
		
// returns 0 if successful.
// returns 1 if x is invalid
// returns 3 if width is invalid
// returns 4 if height is invalid 
// returns 5 if tao is too small 
	// the function will still filter and output the filtered signal as normal, but it will filter with the minimum tau value (FILTER_TAU_MINIMUM)
short filter_lowpass_2D_f(float *x, float *y, unsigned long long int width, unsigned long long int height, float tau){
	
	//--------------------------------------------------
	// checking for errors
	//--------------------------------------------------
	if(x == NULL){
		error("filter_lowpass_2D_f() received NULL x pointer");
		return 1;
	}
	if(width < 1){
		error_d("filter_lowpass_2D_f() received too small width. width = ", (int)width);
		return 3;
	}
	if(height < 1){
		error_d("filter_lowpass_2D_f() received too small height. height = ", (int)height);
		return 4;
	}
	byte tauTooLow = 0;
	if(tau < FILTER_TAU_MINIMUM){
		error_f("filter_lowpass_2D_f() received too small a tau value. tau = ", tau);
		tau = FILTER_TAU_MINIMUM;
		tauTooLow = 1;
	}
	
	//--------------------------------------------------
	// setting up variables and memory
	//--------------------------------------------------
	// calculate the decay factor
	double decayFactor = 1.0 - exp(-1.0/tau);
	
	// this points to a 2D array of floating point values that the program will write the filtered value to.
	float *output;
	// if y is invalid, then overwrite the input array x.
	if(y == NULL)	output = x;
	// if y is valid, output filtered signal into array y.
	else			output = y;
	
	// these are single dimensional arrays for calculating single iterations of low pass filters in one direction.
	float *tempRow1 = malloc(width*sizeof(float));
	float *tempRow2 = malloc(width*sizeof(float));
	float *tempCol1 = malloc(height*sizeof(float));
	float *tempCol2 = malloc(height*sizeof(float));
	
	
	//--------------------------------------------------
	// filter in bidirectionally x dimension
	//--------------------------------------------------
	
	int i,j;
	// smooth 
	for(j=0; j<height; j++){
		
		// initialize first element
		tempRow1[0] = x[j];
		// filter forwards from the second-to-first element to th last element
		for(i=1; i<width; i++){
			tempRow1[i] = tempRow1[i-1] + (x[i*height+j] - tempRow1[i-1])*decayFactor;
		}
		
		// initialize the last element
		tempRow2[width-1] = x[(width-1)*height+j];
		// filter backwards from second-to-last element to first element
		for(i=width-2; i>=0; i--){
			tempRow2[i] = tempRow2[i+1] + (x[i*height+j] - tempRow2[i+1])*decayFactor;
		}
		// average the two directions of low pass filter
		for(i=0; i<width; i++){
			output[i*height+j] = (tempRow1[i]+tempRow2[i])/2.0;
		}
		
	}
	
	//--------------------------------------------------
	// filter in bidirectionally y dimension
	//--------------------------------------------------
	
	// for every width,
	for(i=0; i<width; i++){
		
		// initialize first element
		tempCol1[0] = x[i*height];
		// filter forwards from the second-to-first element to th last element
		for(j=1; j<height; j++){
			tempCol1[j] = tempCol1[j-1] + (x[i*height+j] - tempCol1[j-1])*decayFactor;
		}
		
		// initialize the last element
		tempCol2[height-1] = x[(height-1)+i*height];
		// filter backwards from second-to-last element to first element
		for(j=height-2; j>=0; j--){
			tempCol2[j] = tempCol2[j+1] + (x[i*height+j] - tempCol2[j+1])*decayFactor;
		}
		// average the two directions of low pass filter
		for(j=0; j<height; j++){
			output[i*height+j] = (tempCol1[j]+tempCol2[j])/2.0;
		}
		
	}
	
	
	// free the temporary arrays.
	// There were only used to generate the data. They don't contain it anymore.
	if(tempRow1 != NULL) free(tempRow1);
	if(tempRow2 != NULL) free(tempRow2);
	if(tempCol1 != NULL) free(tempCol1);
	if(tempCol2 != NULL) free(tempCol2);
	
	
	// tau was too low, but the filtering was still applied at the lowest possible tau.
	if(tauTooLow) return 5;
	
	// filtered and stored filtered 2D signal successfully. Everything worked properly.
	return 0;
}
