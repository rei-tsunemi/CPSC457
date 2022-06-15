// -------------------------------------------------------------------------------------
// this is the only file you need to edit
// -------------------------------------------------------------------------------------
//
// (c) 2021, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "fatsim.h"
#include <cstdio>
#include <iostream>
#include <algorithm>

// reimplement this function
std::vector<long> fat_check(const std::vector<long> & fat)
{
	/* NOTE: the version I implemented was the one that reverses the direction of arrows in the graph */

	std::vector<long> result, distances, starts;
	std::vector<std::vector<long>> adj_list;
	adj_list.resize(fat.size());
	distances.resize(fat.size(), 0);
	//set the sizes of the adjacency list and distance vector

	for(long i = 0; i < fat.size(); i++) {
		if(fat.at(i) == -1) {
			starts.push_back(i);
			distances.at(i) = 1;
			//track anywhere there is a -1 to indicate a starting point for the search
			//also set the distance for any of these nodes
		}
		else {
			adj_list.at(fat.at(i)).push_back(i);
			//otherwise keep track of all outgoing arrows from each node
		}
	}

	for(auto i: starts) {
		std::vector<long> stack{i};
		int max = 0;
		//for each node that points to -1, there will be a new path,
		//so find the longest path from each of these nodes

		while(!stack.empty()) {
			long n = stack.back();
			stack.pop_back();
			//take the top element out of the stack

			std::vector<long> adj = adj_list.at(n);
			for(auto n2: adj) {
				distances.at(n2) = distances.at(n) + 1;
				stack.push_back(n2);
				//check each adjacent node, and update its distance from -1
			}

			if(distances.at(n) > max) {
				max = distances.at(n);
				//if a new longest distance has been found, update the max distance
			}
		}

		result.push_back(max);
		//add the longest path to the result vector
	}

	std::sort(result.begin(), result.end());
	//sort the result vector from the smallest distance to the largest one
	return result;
}

