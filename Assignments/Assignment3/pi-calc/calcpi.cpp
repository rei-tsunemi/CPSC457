// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
// Currently the function ignores the n_threads parameter. Your job is to
// parallelize the function so that it uses n_threads threads to do
// the computation.

// check for some help maybe? https://github.com/stephanedorotich/CPSC457_TUT
// WORK DOES NOT INCLUDE -1 AT THE END BECAUSE IT IS NOT INCLUSIVE, WATCH OUT FOR THIS DURING IMPLEMENTATION

#include "calcpi.h"

struct Task {int start, end, partial_sum;};

uint64_t count_pixels(int r, int n_threads)
{
	int size_per_thread = r / (n_threads - 1);
	int remainder = r % n_threads;
	Task tasks[n_threads];

	if(size_per_thread == 0) {
		for(int i = 0; i < r; i++) {
			tasks[i].start = i;
			tasks[i].end = i + 1;
			//divide the work among the needed threads 
		} 
		for(int i = r; i < n_threads; i++) {
			tasks[i].start = 0;
			tasks[i].end = 0;
			//for all unneeded threads, set the start and end values to 0
		}
	}
	else {
		for(int i = 0; i < n_threads - 1; i++) {
		tasks[i].start = i * size_per_thread;
		tasks[i].end = (i + 1) * size_per_thread;
		//initialize the starting and ending points of each thread
		}
		tasks[n_threads - 1].start = r - remainder;
		tasks[n_threads - 1].end = r - 1;
		//the last thread will handle the remainder so intitialize that separately
	}
	


	/*
	double rsq = double(r) * r;
	uint64_t count = 0;
	for( double x = 1 ; x <= r ; x ++)
		for( double y = 0 ; y <= r ; y ++)
			if( x*x + y*y <= rsq) count ++;
	return count * 4 + 1;
	*/
}

