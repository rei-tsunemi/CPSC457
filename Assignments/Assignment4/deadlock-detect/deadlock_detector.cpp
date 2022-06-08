// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"
#include <unordered_map>
#include <iostream>

std::string my_trim(std::string s) {
	std::string whitespace = " \n\r\t\f\v";
	size_t start, end, range;
	start = s.find_first_not_of(whitespace);
	end = s.find_last_not_of(whitespace);
	//find the start and end position of the actual word, ignoring whitespace

	range = end - start + 1;
	return s.substr(start, range);
}

std::vector<std::string> my_split(std::string s) {
	std::string delim1 = "->";
	std::string delim2 = "<-";
	std::vector<std::string> result;
	std::string str;

	size_t pos = 0;
	if((pos = s.find(delim1)) != std::string::npos) {
		//if the arrow is pointing right, trim the string to get the two words and push them both to the front of the vector
		str = my_trim(s.substr(0, pos));
		str.push_back('p');	//add a p to the end of the string to indicate that it is a process
		result.push_back(str);

		s.erase(0, pos + delim1.length());
		s = my_trim(s);
		s.push_back('r');	//add an r to the end of the string to indicate that it is a resource
		result.push_back(s);
	}
	else if((pos = s.find(delim2)) != std::string::npos) {
		//if the arrow points left, trim the string but put both parts to the back of the vector
		str = my_trim(s.substr(0, pos));
		str.push_back('p');	//add a p to the end of the string to indicate that it is a process
		result.push_back(str);

		s.erase(0, pos + delim2.length());
		s = my_trim(s);
		s.push_back('r');	//add an r to the end of the string to indicate that it is a process
		result.insert(result.begin(), s);
	}

	return result;
}


class Graph {
private:
	std::unordered_map<std::string, std::vector<std::string>> adj_list;
	std::unordered_map<std::string, int> out_counts;

public:
	void add(std::string e) {
		std::vector<std::string> r = my_split(e);
		//split the string into the starting and end point of the edge. Ex. "2 <- a" would be split into "a" and "2" with "a" being the first element

		std::string start, end;
		start = r.at(0);
		end = r.at(1);
		//get the starting and end points of the edge

		out_counts[start]++;
		//increment the number of edges coming from the start point

		auto found = adj_list.find(end);
		if(found == adj_list.end()) {
			std::vector<std::string> temp;
			temp.emplace_back(start);
			adj_list.insert({end, temp});
			out_counts[end];
			//if end is not already in the map, create a vector of strings and insert it into the map
		}
		else {
			(found->second).emplace_back(start);
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
			//take one of the vertices out of the queue

			std::vector<std::string> incoming = adj_list[n];
			for(std::string i: incoming) {
				//since n is  being removed from the graph, all incoming edges will be deleted
				out[i]--;
				if(out[i] == 0) {
					zeros.insert(zeros.begin(), i);
					//if any element i now has no outgoin edges, add it to the beginning of the vector
				}
			}
		}

		std::vector<std::string> result;
		for(auto i: out) {
			std::string s = i.first;
			if(out[s] != 0 && (s.back() == 'p')) {
				//if there is an element with >0 outgoing edges and it is a process (indicated by the p at the end)
				//add it to the result
				s.erase(s.length() - 1, 1);
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
