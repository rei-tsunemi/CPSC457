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
#include <pthread.h>

void* parallelWork(void* thread_task);

//idea for the struct for each task taken from the hint for Q1
struct Task {
	int start, end, r;
	double rsq;
	uint64_t partial_sum;

	Task() {
		partial_sum = 0;
	};
};

uint64_t count_pixels(int r, int n_threads)
{
	int size_per_thread;
	int remainder = r % n_threads;
	if(remainder != 0) {
		size_per_thread = r / (n_threads - 1);
		//if r cannot be divided evenly into n_threads parts, use n_threads-1 threads to divide the work evenly
		//and use the last thread to handle the remainder
	}
	else {
		size_per_thread = r / n_threads;
		//otherwise divide r evenly into n_threads parts
	}

	Task tasks[n_threads];
	pthread_t threads[n_threads];
	double rsq = double(r) * r;

	if(n_threads == 1) {
		tasks[0].start = 0;
		tasks[0].end = r;
		tasks[0].r = r;
		tasks[0].rsq = rsq;
//		tasks[0].partial_sum = 0;
		//if there is only 1 thread, initialize all values for the task
	}
	else if(size_per_thread == 0) {
		//if r < n_threads, give r threads work and the remaining threads will do nothing
		for(int i = 0; i < r; i++) {
			tasks[i].start = i;
			tasks[i].end = i + 1;
			tasks[i].r = r;
			tasks[i].rsq = rsq;
//			tasks[i].partial_sum = 0;
			//divide the work among the needed threads
		}
		for(int i = r; i < n_threads; i++) {
			tasks[i].start = 0;
			tasks[i].end = 0;
			tasks[i].r = 0;
			tasks[i].rsq = 0;
//			tasks[i].partial_sum = 0;
			//for all unneeded threads, set the start and end values to 0
		}
	}
	else {
		//otherwise give n_threads an equal amount of work
		for(int i = 0; i < n_threads; i++) {
			tasks[i].start = i * size_per_thread;
			tasks[i].end = (i + 1) * size_per_thread;
			tasks[i].r = r;
			tasks[i].rsq = rsq;
//			tasks[i].partial_sum = 0;
			//initialize all Task values for each thread
		}

		if(tasks[n_threads - 1].end != r) {
			tasks[n_threads - 1].end = r;
			//if r does not divide into n_threads evenly, adjust the final Task to have the correct end value
		}
	}

	uint64_t result = 0;

	for(int i = 0; i < n_threads; i++) {
		pthread_create(&threads[i], NULL, parallelWork, &tasks[i]);
		//create all of the threads and send them to start work in the parallel work function
		//the input argument is the corresponding Task entry
	}

	for(int i = 0; i < n_threads; i++) {
		pthread_join(threads[i], NULL);
		//wait for all threads to complete their work and destroy the threads
	}

	for(int i = 0; i < n_threads; i++) {
		result += tasks[i].partial_sum;
		//sum up the results of each thread
	}

	return result * 4 + 1;
}

void* parallelWork(void* thread_task) {
	Task* task = (Task*) thread_task;
	int r = task->r;
	double rsq = task->rsq;
	//convert the input arguments into their correct types


	for(double x = task->start + 1; x <= task->end; x++) {
		for(double y = 0; y <= r; y++) {
			if(x*x + y*y <= rsq)
				task->partial_sum++;
		}
	}
	//sum up the area using a similar loop to the single-threaded version

	pthread_exit(0);
	//return 0
}
