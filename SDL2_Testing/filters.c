#include "filters.h"
#include <math.h>




/// this function will perform a low-pass filtering operation on floating point data in a two-dimensional signal.
// the filtering will be applied over the x axis (width) and the y axis (height).
// the function will return the filtered input signal data in the "y" floating point array.
// if y is NULL, it will overwrite x with a filtered version of x.
// returns 0 if successful.
short filter_lowpass_2D_f(float **x, float **y, unsigned long long int width, unsigned long long int height, float tau){
	
	// calculate the decay factor
	double decayFactor = 1.0 - exp(-1.0/tau);
	
	
	
	// TODO: write multidimensional bidirectional low-pass filter with variable cutoff frequency.
	
	
	
	// filtered and stored filtered 2D signal successfully.
	return 0;
}
