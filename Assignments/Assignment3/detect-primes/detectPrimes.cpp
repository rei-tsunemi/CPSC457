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

// C++ barrier class (from lecture notes).
// -----------------------------------------------------------------------------
// You do not have to use it in your implementation. If you don't use it, you
// may delete it.
class simple_barrier {
	std::mutex m_;
	std::condition_variable cv_;
	int n_remaining_, count_;
	bool coin_;

	public:
	simple_barrier(int count = 1) { init(count); }
	void init(int count)
	{
		count_ = count;
		n_remaining_ = count_;
   	coin_ = false;
	}
	bool wait()
	{
		if (count_ == 1) return true;
		std::unique_lock<std::mutex> lk(m_);
		if (n_remaining_ == 1) {
		coin_ = ! coin_;
		n_remaining_ = count_;
		cv_.notify_all();
		return true;
		}
		auto old_coin = coin_;
		n_remaining_--;
		cv_.wait(lk, [&]() { return old_coin != coin_; });
		return false;
	}
};

struct Task {
	std::vector<int64_t> nums;
	std::vector<int64_t> primes;
	pthread_barrier_t* barrier;

	Task(std::vector<int64_t> n, pthread_barrier_t* b) {
		nums = n;
		barrier = b;
	}
};

std::atomic<bool> finished;

// returns true if n is prime, otherwise returns false
// -----------------------------------------------------------------------------
// to get full credit for this assignment, you will need to adjust or even
// re-write the code in this function to make it multithreaded.
static bool is_prime(int64_t n, pthread_t* threads)
{
	// handle trivial cases
	if (n < 2) return false;
	if (n <= 3) return true; // 2 and 3 are primes
	if (n % 2 == 0) return false; // handle multiples of 2
	if (n % 3 == 0) return false; // handle multiples of 3
	// try to divide n by every number 5 .. sqrt(n)
	int64_t i = 5;
	int64_t max = sqrt(n);
	while (i <= max) {
   	if (n % i == 0) return false;
   	if (n % (i + 2) == 0) return false;
   	i += 6;
	}
	// didn't find any divisors, so it must be a prime
	return true;
}

void* thread_start(void* t) {
	struct Task* task = (struct Task*) t;
	while(1) {
		if(pthread_barrier_wait(task->barrier) != 0) {
			//do something
			break;
		}
	}
}

// This` function takes a list of numbers in nums[] and returns only numbers that
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
	std::vector<int64_t> result;
	pthread_t threads[n_threads];
	finished.store(false);
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, n_threads);
	struct Task task(nums, &barrier);

	for(int i = 0; i < n_threads; i++) {
		pthread_create(&threads[i], NULL, thread_start, &task);
		//create all of the threads and send them to start work in the parallel work function
		//the input argument is the corresponding Task entry
	}

	for(int i = 0; i < n_threads; i++) {
		pthread_join(threads[i], NULL);
		//wait for all threads to complete their work and destroy the threads
	}

	// for (auto num : nums) {
   // 	if (is_prime(num, threads)) result.push_back(num);
	// }
	return result;
}
