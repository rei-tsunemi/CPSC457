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
#include <cmath>
#include <set>
#include <unordered_map>

struct Partition {
	int tag;
	int64_t size, addr;

	Partition(int64_t s, int64_t a) {
		size = s;
		addr = a;
		tag = -1;
	}

	void print() {
		std::cout<<"Printing partition with tag = "<<tag<<":\n";
		std::cout<<"size = "<<size<<std::endl;
		std::cout<<"address = "<<addr<<std::endl;
	}

	bool operator==(const Partition& b) const {
		if(tag != b.tag) return false;
		else if (size != b.tag) return false;
		else if (addr != b.addr) return false;
		else return true;
	}
};

typedef std::list<Partition>::iterator PartitionRef;

struct scmp {
	bool operator()(const PartitionRef& c1, const PartitionRef& c2) const {
		if(c1->size == c2->size) {
			return c1->addr < c2->addr;
		}
		else {
			return c1->size > c2->size;
		}
	}
};

// I recommend you implement the simulator as a class. This is only a suggestion.
// If you decide not to use this class, please remove it before submitting it.
struct Simulator {
	std::list<Partition> all_blocks;
	std::set<PartitionRef, scmp> free_blocks;
	std::unordered_map<long, std::vector<PartitionRef>> tagged_blocks;
	int64_t page_size, pages_requested;

	Simulator(int64_t page_size) {
		this->page_size = page_size;
		pages_requested = 0;
	}

	void allocate(int tag, int size) {
		//std::cout<<"\nALLOCATE "<<tag<<" SIZE = "<<size<<std::endl;
		PartitionRef max_free = *(free_blocks.begin());
		int64_t max_size = 0, max_addr = 0;
		if(!free_blocks.empty()) {
			max_size = max_free->size;
			max_addr = max_free->addr;
		}

		if(max_size < size) {
			//if max_size is too small, request more pages
			int64_t n_pages;
			if(all_blocks.empty() || all_blocks.back().tag != -1) {
				int64_t addr = 0;
				if(!all_blocks.empty()) {
					addr = all_blocks.back().addr;
				}
				//find the starting address of the new block

				n_pages = ceil(size / (double)page_size);
				Partition newPar(n_pages * page_size, addr);
				all_blocks.emplace_back(newPar);
				free_blocks.insert(std::prev(all_blocks.end()));
				//create a brand new partition for the requested space and add it to the end
				//also add the new free partition to the set of free blocks
			}
			else {
				free_blocks.erase(std::prev(all_blocks.end()));
				n_pages = ceil((size - all_blocks.back().size) / (double)page_size);
				all_blocks.back().size += n_pages * page_size;
				free_blocks.insert(std::prev(all_blocks.end()));
				//otherwise just resize the end partition

				//note that the end partition must be removed from free_blocks first since just modifying it will
				//cause the set to be out of order
			}
			pages_requested += n_pages;

			max_free = *(free_blocks.begin());
			max_size = max_free->size;
			max_addr = max_free->addr;
		}

		if(max_size > size) {
			Partition newPar(size, max_addr);
			newPar.tag = tag;
			PartitionRef temp = all_blocks.insert(max_free, newPar);
			tagged_blocks[tag].emplace_back(temp);
			//make a new partition to hold the tagged block and resize the free one

			free_blocks.erase(max_free);
			max_free->size -= size;
			max_free->addr += size;
			free_blocks.insert(max_free);
			//adjust the size of the remaining free space

			//the free partition needs to be removed before adjusting the size and address to ensure the set stays in
			//the correct order
		}
		else {
			max_free->tag = tag;
			free_blocks.erase(max_free);
			tagged_blocks[tag].emplace_back(max_free);
			//since the amount of free space is the same as the required space, just change the tag of the partition
			//and remove the block from the set of free blocks
		}



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
		//std::cout<<"\nDEALLOCTAE "<<tag<<std::endl;
		std::vector<PartitionRef> owned = tagged_blocks[tag];

		for(PartitionRef i: owned) {
			i->tag = -1;

			PartitionRef temp = std::next(i);
			if(temp->tag == -1) {
				i->size += temp->size;
				all_blocks.erase(temp);
				free_blocks.erase(temp);
			}

			temp = std::prev(i);
			if(temp->tag == -1) {
				i->size += temp->size;
				i->addr = temp->addr;
				all_blocks.erase(temp);
				free_blocks.erase(temp);
			}

			free_blocks.insert(i);
		}

		tagged_blocks.erase(tag);

   	// Pseudocode for deallocation request:
   	// - for every partition
    	//     - if partition is occupied and has a matching tag:
    	//         - mark the partition free
    	//         - merge any adjacent free partitions
  	}

	MemSimResult getStats() {
		MemSimResult res;
		res.n_pages_requested = pages_requested;
		PartitionRef temp = *(free_blocks.begin());
		if(!free_blocks.empty()) {
			res.max_free_partition_size = temp->size;
			res.max_free_partition_address = temp->addr;
		}
		else {
			res.max_free_partition_size = 0;
			res.max_free_partition_address = 0;
		}

		return res;
 	}

  	void check_consistency() {
		std::cout<<"PRINTING ALL_BLOCKS-----------------------------------"<<std::endl;
		for(auto i: all_blocks) {
			i.print();
		}

		std::cout<<"PRINTING TAGGED_BLOCKS-----------------------------------"<<std::endl;
		for(auto i: tagged_blocks) {
			std::cout<<"Blocks owned by "<<i.first<<std::endl;
			for(auto j: i.second) {
				j->print();
			}
		}

		std::cout<<"PRINTING FREE_BLOCKS-----------------------------------"<<std::endl;
		for(auto i: free_blocks) {
			i->print();
		}

		std::cout<<"\n\n";

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
		//sim.check_consistency();
	}

	return sim.getStats();
}
