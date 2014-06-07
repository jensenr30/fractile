//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
#include "mt19937int.h"


// use lsgenrand() for getting a seed.
// use genrand() for generating a random number


/// this returns a value between the boundary values lowBound and highBound (including boundary values) 
#define rand_range(lowBound, highBound) (genrand()%(highBound-lowBound+1))+lowBound





/// this returns a value between the boundary values lowBound and highBound (including boundary values)
// this function does not assume that lowBound is lower than highBound.
int rand_get_check(int, int);



// get a number ranging from 0 to 100,000 (0 to 100.000 percent)
#define rand_ht() rand_range(0, 100000)
// get a 1 or a 0
#define rand_bool() rand_range(0,1)





// returns the value of the bigger one.
// this doesn't exactly have anything to do with random numbers, does it?  ;)
int bigger(int, int);





// returns one or the other randomly
int rand_either(int, int);


void rand_test(int);



// this is for testing how quickly PRNGs work.
void test_random_functions(unsigned long);







