/// ============================================================================
/// Copyright (C) 2022 Pavol Federl (pfederl@ucalgary.ca)
/// All Rights Reserved. Do not distribute this file.
/// ============================================================================
///
/// You must modify this file and then submit it for grading to D2L.
///
/// You can delete all contents of this file and start from scratch if
/// you wish, as long as you implement the detect_primes() function as
/// defined in "detectPrimes.h".

#include "detectPrimes.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <pthread.h>

#define MAX_THREADS 256

struct ThreadData {
	int start, n_threads, thread_num;
	std::vector<int64_t>* nums;

	void set(int s, int n_thr, int thread_no, std::vector<int64_t>* n_vec) {
		start = s;
		n_threads = n_thr;
		nums = n_vec;
		thread_num = thread_no;
	}
};

bool finished;
pthread_barrier_t barrier;
std::vector<int64_t> primes;
int64_t n;
bool results[MAX_THREADS];
std::atomic<bool> cancellation;

static bool is_prime(int64_t n, struct ThreadData d) {
	if(n < 2) return false;
	if (n <= 3) return true;
	if(n % 2 == 0) return false;
	if(n % 3 == 0) return false;

	int64_t i = d.start;
	int64_t max = sqrt(n);

	while(i <= max) {
		if(n % i == 0) return false;
		if(n % (i + 2) == 0) return false;

		i += (d.n_threads * 6);
		if(cancellation.load()) {
			return false;
		}
	}
	return true;
}

void* thread_start(void* t) {
	struct ThreadData data = *(struct ThreadData*) t;
	while(1) {
		cancellation.store(false);
		int r = pthread_barrier_wait(&barrier);
		if(r == PTHREAD_BARRIER_SERIAL_THREAD) {
			//SERIAL SECTION
			if(data.nums->empty()) {
				finished = true;
				//if the vector of numbers is empty, set the finished flag to true
			}
			else {
				n = data.nums->back();
				data.nums->pop_back();
				//otherwise pop the number out of the vector and set the global variable to its value
			}
		}
		pthread_barrier_wait(&barrier);

		//PARALLEL SECTION
		if(finished) {
			pthread_exit(0);
		}

		results[data.thread_num] = is_prime(n, data);
		//check if the number is prime

		if(!results[data.thread_num]) {
			cancellation.store(true);
		}

		//SERIAL SECTION
		if(r == PTHREAD_BARRIER_SERIAL_THREAD) {
			//reuse the thread from the last serial section for this one to avoid using extra barriers
			bool res = true;
			for(int i = 0; i < data.n_threads; i++) {
				if(!results[i]) {
					res = false;
					break;
					//if any of the threads found that the number was not prime, set the result to false and break out of the loop
				}
			}
			if(res) {
				primes.push_back(n);
				//if the number was prime, add the number to the result vector
			}
		}
	}
}

// This function takes a list of numbers in nums[] and returns only numbers that
// are primes.
//
// The parameter n_threads indicates how many threads should be created to speed
// up the computation.
// -----------------------------------------------------------------------------
// You will most likely need to re-implement this function entirely.
// Note that the current implementation ignores n_threads. Your multithreaded
// implementation must use this parameter.
std::vector<int64_t>
detect_primes(const std::vector<int64_t> & nums, int n_threads)
{
	std::vector<int64_t> nums_copy = nums;
	pthread_t threads[n_threads];
	finished = false;
	pthread_barrier_init(&barrier, NULL, n_threads);
	struct ThreadData data[n_threads];

	for(int i = 0; i < n_threads; i++) {
		int n = 5 + (i * 6);
		data[i].set(n, n_threads, i, &nums_copy);
	}

	for(int i = 0; i < n_threads; i++) {
		pthread_create(&threads[i], NULL, thread_start, &data[i]);
		//create all of the threads and send them to start work in the parallel work function
		//the input argument is the corresponding Task entry
	}

	for(int i = 0; i < n_threads; i++) {
		//std::cout<<"thread "<<i<<": "<<threads[i]<<std::endl;
		pthread_join(threads[i], NULL);
		//wait for all threads to complete their work and destroy the threads
	}

	// for (auto num : nums) {
   // 	if (is_prime(num, threads)) result.push_back(num);
	// }
	return primes;
}
