/**
	this file has some definitions and functions that will perform some common mathematical operations.
	this is essentially for convenience in other functions.
	
*/

/// this performs a weighted average of the inputs w,x,y, and z. their weights are a,b,c, and d respectively.
#define weighted_ave_4(w,x,y,z,a,b,c,d) ( (w*a + x*b + y*c + z*a)/(a+b+c+d) )

/// this performs a weighted average of the four inputs where the floating point version of the reciprocal of the weights are used
// this is handy when you want to tell the function how close each weight is to the target (the closer, the more weight it has)
// this becomes useful for weighting things based on their distances in a geometric sense.
#define weighted_ave_4_recip_f(w,x,y,z,a,b,c,d) (  (w/((float)(a)) + x/((float)(b)) + y/((float)(c)) + z/((float)(d)))  /  (1/((float)(a)) + 1/((float)(b)) + 1/((float)(c)) + 1/((float)(d)))  )


/// this performs a weighted average of the three inputs where the floating point version of the reciprocal of the weights are used
// this is handy when you want to tell the function how close each weight is to the target (the closer, the more weight it has)
// this becomes useful for weighting things based on their distances in a geometric sense.
#define weighted_ave_3_recip_f(x,y,z,a,b,c) (  ( x/((float)(a)) + y/((float)(b)) + z/((float)(c)) )  /  (1/((float)(a)) + 1/((float)(b)) + 1/((float)(c)))  )

