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
	std::vector<long> result, distances, starts;
	std::vector<std::vector<long>> adj_list;
	adj_list.resize(fat.size());
	distances.resize(fat.size(), 0);

	for(long i = 0; i < fat.size(); i++) {
		if(fat.at(i) == -1) {
			starts.push_back(i);
			distances.at(i) = 1;
			//track anywhere there is a -1 to indicate a starting point for the search
		}
		else {
			adj_list.at(fat.at(i)).push_back(i);
		}
	}

	for(auto i: starts) {
		std::vector<long> stack{i};
		int max = 0;

		while(!stack.empty()) {
			long n = stack.back();
			stack.pop_back();

			std::vector<long> adj = adj_list.at(n);
			for(auto n2: adj) {
				distances.at(n2) = distances.at(n) + 1;
				stack.push_back(n2);
			}

			if(distances.at(n) > max) {
				max = distances.at(n);
			}
		}

		result.push_back(max);
	}

	std::sort(result.begin(), result.end());
	return result;
}

