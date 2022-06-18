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
		PartitionRef max_free = *(free_blocks.begin());
		int64_t max_size = 0, max_addr = 0;

		if(!free_blocks.empty()) {
			max_size = max_free->size;
			max_addr = max_free->addr;
			//if there is a free block, set the max size and address of this block appropriately
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
			//track the number of pages needed

			max_free = *(free_blocks.begin());
			max_size = max_free->size;
			max_addr = max_free->addr;
			//adjust all variables tracking the info of the largest free partition
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
	}

	void deallocate(int tag) {
		std::vector<PartitionRef> owned = tagged_blocks[tag];
		//get all partitions owned by the given tag

		for(PartitionRef i: owned) {
			i->tag = -1;
			//change the tag of the current block

			PartitionRef temp = std::next(i);
			if(temp->tag == -1) {
				i->size += temp->size;
				all_blocks.erase(temp);
				free_blocks.erase(temp);
				//if the block to the right is also free, merge it into i and remove it from the free_block list
			}

			temp = std::prev(i);
			if(temp->tag == -1) {
				i->size += temp->size;
				i->addr = temp->addr;
				all_blocks.erase(temp);
				free_blocks.erase(temp);
				//if the block to the left is also free, merge it into i and remove it from the free_block list
			}

			free_blocks.insert(i);
			//add the newly freed block of memory into the free_block list
		}

		tagged_blocks.erase(tag);
		//remove all blocks with this tag from the list of tagged blocks
  	}

	MemSimResult getStats() {
		MemSimResult res;
		res.n_pages_requested = pages_requested;
		//get the number of pages requested throughout the entire simulation

		PartitionRef temp = *(free_blocks.begin());
		if(!free_blocks.empty()) {
			res.max_free_partition_size = temp->size;
			res.max_free_partition_address = temp->addr;
			//if there are any free blocks left, take the top one out of the set
			//to get the block with the max size
		}
		else {
			res.max_free_partition_size = 0;
			res.max_free_partition_address = 0;
			//otherwise there are no free blocks so set the size and address to 0
		}

		return res;
 	}

  	void check_consistency() {
		std::cout<<"PRINTING ALL_BLOCKS-----------------------------------"<<std::endl;
		for(auto i: all_blocks) {
			i.print();
		}
		//check that the linked list is correct

		std::cout<<"PRINTING TAGGED_BLOCKS---------------------------------"<<std::endl;
		for(auto i: tagged_blocks) {
			std::cout<<"Blocks owned by "<<i.first<<std::endl;
			for(auto j: i.second) {
				j->print();
			}
		}
		//check that the hash map is correct

		std::cout<<"PRINTING FREE_BLOCKS-----------------------------------"<<std::endl;
		for(auto i: free_blocks) {
			i->print();
		}
		//check that the set is correct

		std::cout<<"\n\n";
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
