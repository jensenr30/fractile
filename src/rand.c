
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand.h"
#include "mt19937int.h"


/// this will return a random height given some information
// average is the bias point. it is the expected value. the "DC bias point" if you prefer.
// the radius is how far in either direction (higher than the average, or lower than the average) the result may be.
float rand_radius_retry(float average, float radius, float min, float max){
	float retVal;
	
	while(1){
		retVal = average + ((genrand()%100001)/50000.0)*radius - radius;
		if(retVal >= min && retVal <= max)
			return retVal;
	}
	
	// this should never happen.
	// but if it does, just return -1.0 I guess...
	return -1.0;
}


/// this returns a value between the boundary values lowBound and highBound (including boundary values)
// this function does not assume that lowBound is lower than highBound.
int rand_get_check(int lowBound, int highBound){
	
	// if the lowBound is higher than the highBound, then flip them around and return that.
	if(highBound < lowBound){
		return rand_range(highBound, lowBound);
	}
	
	return ( rand() % (highBound-lowBound+1) ) + lowBound;
}



// returns the value of the bigger one
int bigger(int a, int b){
	if(a >= b) return a;
	else return b;
}




// returns one or the other randomly
int rand_either(int a, int b){
	if(rand_bool()) return a;
	else return b;
}





void rand_test(int iter){
	FILE *output = fopen("random numbers.txt","w");
	if(output == NULL){
		return;
	}
	int i;
	for(i=0; i<iter; i++)
		fprintf(output, "%10lu\n",genrand());
}



// this is for testing how quickly PRNGs work.
void test_random_functions(unsigned long numbersToGen){
	time_t cTime;
	int stdTime, mtTime;
	unsigned long  int i;
	
	//testing the stdlib rand() function's time
	cTime = time(NULL);
	for(i=0;i<numbersToGen; i++) rand();
	stdTime = time(NULL) - cTime;
	printf("Time for stdlib.h's rand() function to calculate %lu random numbers is %d s\n",numbersToGen, stdTime);
	
	// testing the mt19937int.h genrand() function
	cTime = time(NULL);
	for(i=0;i<numbersToGen; i++) genrand();
	mtTime = time(NULL) - cTime;
	printf("Time for mt19937int.h's genrand() function to calculate %lu random numbers is %d s\n",numbersToGen, mtTime);
	
}







