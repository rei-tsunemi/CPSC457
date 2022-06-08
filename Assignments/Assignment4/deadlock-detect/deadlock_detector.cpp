// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"
#include <unordered_map>
#include <iostream>

class Graph {
private:
	std::unordered_map<std::string, std::vector<std::string>> adj_list;
	std::unordered_map<std::string, int> out_counts;

public:
	void add(std::string e) {
		std::vector<std::string> r = split(e);
		//split the string into the starting and end point of the edge. Ex. "2 <- a" would be split into "a" and "2" with "a" being the first element

		std::string start, end;
		if(r.at(1).compare("->") == 0) {
			start = r.at(0);
			start.push_back('p');
			end = r.at(2);
			end.push_back('r');
			//add a p and r to the appropriate strings to indicate a process and resource respectively
		}
		else {
			start = r.at(2);
			start.push_back('r');
			end = r.at(0);
			end.push_back('p');
		}

		out_counts[start]++;
		//increment the number of edges coming from the start point

		auto found = adj_list.find(end);
		if(found == adj_list.end()) {
			std::vector<std::string> temp;
			temp.emplace_back(start);
			adj_list.insert({end, temp});
			out_counts[end] = 0;
			//if end is not already in the map, create a vector of strings and insert it into the map
			//also initialize out_counts to 0
		}
		else {
			(found->second).push_back(start);
			//otherwise, just add the new element
		}

		found = adj_list.find(start);
		if(found == adj_list.end()) {
			std::vector<std::string> empty;
			adj_list.insert({start, empty});
			//if the starting point of the edge is not already in the adjacency list, insert it with an empty vector
		}
	}

	//toposort adapted from pseudocode given in hint for Q1
	std::vector<std::string> toposort() {
		std::unordered_map<std::string, int> out = out_counts;
		std::vector<std::string> zeros;

		for(auto i: out) {
			if(i.second == 0) {
				zeros.emplace_back(i.first);
				//if i has no outgoing edges, add it to the list of vertices with no outgoing edges
			}
		}

		while(!zeros.empty()) {
			std::string n = zeros.back();
			zeros.pop_back();
			out.erase(n);

			std::vector<std::string> incoming_n = adj_list[n];
			for(std::string n2: incoming_n) {
				out[n2]--;
				if(out[n2] == 0) {
					zeros.push_back(n2);
				}
			}
		}


		std::vector<std::string> result;
		for(auto i: out) {
			std::string s = i.first;
			if(s.back() == 'p') {
				s.pop_back();
				result.push_back(s);
			}
		}

		return result;
	}
};

/// this is the function you need to (re)implement
///
/// parameter edges[] contains a list of request- and assignment- edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge. As soon as you detect a deadlock, your function
/// needs to stop processing edges and return an instance of Result structure
/// with edge_index set to the index that caused the deadlock, and dl_procs set
/// to contain with names of processes that are in the deadlock.
///
/// To indicate no deadlock was detected after processing all edges, you must
/// return Result with edge_index = -1 and empty dl_procs[].
///
Result detect_deadlock(const std::vector<std::string> & edges)
{
	Result result;
	result.edge_index = -1;
	Graph graph;

	for(long unsigned int i = 0; i < edges.size(); i++) {
		std::string edge = edges.at(i);

		graph.add(edge);

		std::vector<std::string> cycle_edges = graph.toposort();
		if(!cycle_edges.empty()) {
			//if cycle_edges is not empty, that means a cycle was detected
			//update results, and then return
			result.dl_procs = cycle_edges;
			result.edge_index = i;
			return result;
		}
	}

	return result;
}
