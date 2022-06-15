// -------------------------------------------------------------------------------------
// this is the only file you need to edit
// -------------------------------------------------------------------------------------
//
// (c) 2022, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "memsim.h"
#include <cassert>
#include <iostream>
#include <list>
#include <algorithm>

struct Partition {
	int64_t address, size, tag;

	Partition() {
		size = 0;
		tag = -1;
		//tag = -1 indicates free memory
	}

	Partition(int64_t a, int64_t s) {
		address = a;
		size = s;
		tag = -1;
	}

	Partition(int64_t a, int64_t s, int64_t t) {
		address = a;
		size = s;
		tag = t;
	}
};

// I recommend you implement the simulator as a class. This is only a suggestion.
// If you decide not to use this class, please remove it before submitting it.
class Simulator {
	private:
	std::list<Partition> partitions;
	int64_t page_size;
	int64_t pages_requested;

	public:
	Simulator(int64_t page_size) {
		this->page_size = page_size;
		pages_requested = 0;
	}

	void allocate(int tag, int size) {
		std::list<Partition>::iterator it;
		int64_t max_size = 0, max_address;

		for(it = partitions.begin(); it != partitions.end(); ++it) {
			if(it->tag == -1 && it->size > max_size) {
				max_size = it->size;
				max_address = it->address;
			}
		}

		Partition temp(max_address, max_size);
		it = std::find(partitions.begin(), partitions.end(), temp);
		temp.size -= size;
		temp.address += size;
		partitions.insert(std::next(it), temp);
		it->size = size;
		it->tag = tag;

    // Pseudocode for allocation request:
    // - search through the list of partitions from start to end, and
    //   find the largest partition that fits requested size
    //     - in case of ties, pick the first partition found
    // - if no suitable partition found:
    //     - get minimum number of pages from OS, but consider the
    //       case when last partition is free
    //     - add the new memory at the end of partition list
    //     - the last partition will be the best partition
    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free
	}

	void deallocate(int tag) {
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions
  	}

	MemSimResult getStats() {
   		MemSimResult result;
    		result.n_pages_requested = pages_requested;
		int64_t max_size = 0, max_address = 0;

		std::list<Partition>::iterator it;
		for(it = partitions.begin(); it != partitions.end(); ++it) {
			if(it->tag == -1 && it->size > max_size) {
				max_size = it->size;
				max_address = it->address;
			}
		}

		result.max_free_partition_size = max_size;
		result.max_free_partition_address = max_address;

    		return result;
 	}

  	void check_consistency() {
    // you do not need to implement this method at all - this is just my suggestion
    // to help you with debugging

    // mem_sim() calls this after every request to make sure all data structures
    // are consistent. Since this will probablly slow down your code, you should
    // disable calling this in the mem_sim() function below before submitting
    // your code for grading.

    // here are some suggestions for consistency checks (see appendix also):

    // make sure the sum of all partition sizes in your linked list is
    // the same as number of page requests * page_size

    // make sure your addresses are correct

    // make sure the number of all partitions in your tag data structure +
    // number of partitions in your free blocks is the same as the size
    // of the linked list

    // make sure that every free partition is in free blocks

    // make sure that every partition in free_blocks is actually free

    // make sure that none of the partition sizes or addresses are < 1
  	}
};

// re-implement the following function
// ===================================
// parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests
// return:
//    some statistics at the end of simulation
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{
	// if you decide to use the simulator class, you probably do not need to make
	// any changes to the code below, except to uncomment the call to check_consistency()
	// before submitting your code
	Simulator sim(page_size);
	for (const auto & req : requests) {
		if (req.tag < 0) {
			sim.deallocate(-req.tag);
		}
		else {
			sim.allocate(req.tag, req.size);
		}
		sim.check_consistency();
	}

	return sim.getStats();
}
