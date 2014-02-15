#include "math_custom.h"



/// intersect_p is for when you know some point on each line <x,y> and the slopes of each line.
// returns 1 (true)	 if there is a single intersection
// returns 0 (false) if the lines are parallel but they are not the same line
// returns 2		 if the lines are parallel and they are the same line (have all points in common)

// point <x1,y1> is a point on the first line
// point <x2,y2> is a point on the second line
// slope1 is the slope of the first line  (change-in-y/change-in-x)
// slope2 is the slope of the second line (change-in-y/change-in-x)
// point <xr,yr> is the resulting point where the two lines intersect.
// xr and yr are pointers to variables outside of this function.

int intersect_p(float x1, float y1, float slope1, float x2, float y2, float slope2, float *xr, float *yr){
	
	//------------------------------------------------------------
	// check to see if the lines are parallel and handle special cases
	//------------------------------------------------------------
	if(slope1 == slope2 || slope1 == -slope2){				// if the two lines share the same slope,
		if(x1==x2){											// if the two x points are the same,
			if(y1==y2){										// if the two y points are the same,
				return 2;									// the two lines share an infinite number of points
			}												//otherwise,
			return 0;										// the lines are parallel but not the same line. they share no points.
		}
		float connSlope = (y2-y1)/(x2-x1);					// calculate the slope of a third line that connects the two given data points.
		if(connSlope == slope1 || connSlope == -slope1){	// if the slope between the two points given in the same magnitude as the slopes of the lines,
			return 2;										// there are an infinite number of intersection points
		}
		else{												// otherwise,
			return 0;										// there are no intersection points
		}
	}
	//------------------------------------------------------------
	// calculate where the intersect point is
	//------------------------------------------------------------
	
	//calculate y intercepts
	float b1 = y1 - x1*slope1;
	float b2 = y2 - x2*slope2;
	
	// calculate the intersection coordinates <xr,yr>
	*xr = (b2-b1)/(slope1-slope2);
	*yr = slope1*(*xr) + b1;
	
	// a single intersection point is found.
	return 1; 
}
