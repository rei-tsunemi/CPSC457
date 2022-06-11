// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"

class Graph {
private:
	std::unordered_map<std::string, std::vector<std::string>> adj_list;
	std::unordered_map<std::string, int> out_counts;

public:
	std::vector<std::string> add_and_sort(std::string e) {
		add(e);
		return toposort();
	}

<<<<<<< Updated upstream
	void add(std::string e) {
		std::vector<std::string> r = split(e);
		//split the string into the starting and end point of the edge. Ex. "2 <- a" would be split into "a" and "2" with "a" being the first element
=======
	void add(std::string edge) {
		std::vector<std::string> r = split(edge);
		//split the string everywhere there is a whitespace
>>>>>>> Stashed changes

		std::string start, end;
		if(r.at(1).compare("->") == 0) {
			start = r.at(0);
			start.push_back('*');
			end = r.at(2);
		}
		else {
			start = r.at(2);
			end = r.at(0);
			end.push_back('*');
		}
		//add a * to the appropriate string to indicate a process

<<<<<<< Updated upstream
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
=======
		long unsigned int s = w2i.get(start);
		long unsigned int e = w2i.get(end);
		//get integer values for the strings

		if(s >= conversions.size()) {
			conversions.push_back(start);
		}
		if(e >= conversions.size()) {
			conversions.push_back(end);
		}
		//if the string is a new element, add its string-int conversion to the conversion list

		if (s < adj_list.size()) {
			out_counts.at(s)++;
			//increment the number of edges coming from the start point
>>>>>>> Stashed changes
		}
		else {
			(found->second).push_back(start);
			//otherwise, just add the new element
		}

<<<<<<< Updated upstream
		adj_list[start];
		//if the starting point is already in the adjacency list, do nothing, 
		//otherwise initialize it in the map
=======
		if(e < adj_list.size()) {
			adj_list.at(e).push_back(s);
			//add the new incoming edge
		}
		else {
			std::vector<int> temp;
			temp.push_back(s);
			adj_list.push_back(temp);
			out_counts.push_back(0);
			//if the ending element was not already in the lists, create entries
		}
>>>>>>> Stashed changes
	}

	//toposort adapted from pseudocode given in hint for Q1
	std::vector<std::string> toposort() {
<<<<<<< Updated upstream
		std::unordered_map<std::string, int> out = out_counts;
		std::vector<std::string> zeros;
		int num = out.size();

		for(auto i: out) {
			if(i.second == 0) {
				zeros.emplace_back(i.first);
				//if i has no outgoing edges, add it to the list of vertices with no outgoing edges
			}
		}

		int z_size = zeros.size();
		while(z_size != 0) {
			std::string n = zeros.back();
			zeros.pop_back();
			z_size--;
			num--;

			std::vector<std::string> incoming_n = adj_list[n];
			for(std::string n2: incoming_n) {
				//try use iterator instead of square bracket
				out[n2]--;
				if(out[n2] == 0) {
					zeros.push_back(n2);
					z_size++;
=======
		std::vector<int> zeros, out = out_counts;
		int num_left = out.size();

		for(long unsigned int i = 0; i < out.size(); i++) {
			if(out.at(i) == 0) {
				zeros.push_back(i);
			}
		}

		while(zeros.size() != 0) {
			int n = zeros.back();
			zeros.pop_back();
			num_left--;

			std::vector<int> incoming = adj_list.at(n);
			for(int n2: incoming) {
				out.at(n2)--;
				if(out.at(n2) == 0) {
					zeros.push_back(n2);
>>>>>>> Stashed changes
				}
			}
		}

<<<<<<< Updated upstream

		std::vector<std::string> result;
		if(num != 0) {
			for(auto i: out) {
				std::string s = i.first;
				if(i.second != 0 && s.back() == '*') {
					s.pop_back();
					result.push_back(s);
					//if the deadlocked element is a process (indicated by *), remove the extra character and add it to the result					
=======
		std::vector<std::string> results;
		if(num_left != 0) {
			for(long unsigned int i = 0; i < out.size(); i++) {
				std::string s = conversions.at(i);

				if(out.at(i) != 0 && s.back() == '*') {
					s.pop_back();
					results.push_back(s);
>>>>>>> Stashed changes
				}
			}
		}

<<<<<<< Updated upstream
		return result;
=======
		return results;
>>>>>>> Stashed changes
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
	Graph graph;

	for(long unsigned int i = 0; i < edges.size(); i++) {
		std::string edge = edges.at(i);

		std::vector<std::string> cycle_edges = graph.add_and_sort(edge);
		if(!cycle_edges.empty()) {
			//if cycle_edges is not empty, that means a cycle was detected
			//update results, and then return
			result.dl_procs = cycle_edges;
			result.edge_index = i;
			return result;
		}
	}

	result.edge_index = -1;
	return result;
}
