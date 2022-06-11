// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"
#include <unordered_map>
#include <iostream>

class Graph {
private:
	std::vector<std::vector<int>> adj_list;
	std::vector<int> out_counts;
	Word2Int w2i;
	std::vector<std::string> conversions;

public:
	std::vector<std::string> add_and_sort(std::string e) {
		add(e);
		return toposort();
	}

	void add(std::string edge) {
		std::vector<std::string> r = split(edge);
		//split the string into the starting and end point of the edge. Ex. "2 <- a" would be split into "a" and "2" with "a" being the first element

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

		int s = w2i.get(start);
		int e = w2i.get(end);
		//get integer values for the strings
		//std::cout<<"start = "<<s<<"\tend = "<<e<<std::endl;

		if(s >= conversions.size()) {
			conversions.push_back(start);
		}
		if(e >= conversions.size()) {
			conversions.push_back(end);
		}

		if (s < adj_list.size()) {
			out_counts.at(s)++;
			//increment the number of edges coming from the start point
		}
		else {
			out_counts.push_back(1);
			std::vector<int> empty;
			adj_list.push_back(empty);
			//if the starting element was not already in the lists, create entries
		}

		if(e >= adj_list.size()) {
			std::vector<int> temp;
			temp.push_back(s);
			adj_list.push_back(temp);
			out_counts.push_back(0);
			//if the ending element was not already in the lists, create entries
		}
		else {
			adj_list.at(e).push_back(s);
			//otherwise just add the new edge
		}
	}

	//toposort adapted from pseudocode given in hint for Q1
	std::vector<std::string> toposort() {
		std::vector<int> out = out_counts;
		std::vector<int> zeros;
		int num_left = out.size();

		for(int i = 0; i < out.size(); i++) {
			if(out.at(i) == 0) {
				zeros.push_back(i);
			}
		}

		int z_size = zeros.size();
		while(z_size != 0) {
			int n = zeros.back();
			zeros.pop_back();
			z_size--;
			num_left--;

			std::vector<int> incoming = adj_list.at(n);
			for(int n2: incoming) {
				out.at(n2)--;
				if(out.at(n2) == 0) {
					zeros.push_back(n2);
					z_size++;
				}
			}
		}

		std::vector<std::string> results;
		for(int i = 0; i < out.size(); i++) {
			std::string s = conversions.at(i);
			//std::cout<<i<<" = "<<s<<std::endl;
			if(out.at(i) != 0 && s.back() == '*') {
				s.pop_back();
				results.push_back(s);
			}
		}

		return results;

		/*std::vector<int> out = out_counts;
		std::vector<int> zeros;
		int num = out.size();

		for(int i = 0; i < out.size(); i++) {
			if(out.at(i) == 0) {
				zeros.push_back(i);
				//if i has no outgoing edges, add it to the list of vertices
			}
		}

		int z_size = zeros.size();
		std::cout<<z_size<<std::endl;
		while(z_size != 0) {
			int n = zeros.back();
			zeros.pop_back();
			z_size--;
			num--;

			std::vector<int> incoming_n = adj_list.at(n);
			for(int n2: incoming_n) {
				out.at(n2)--;
				if(out.at(n2) == 0) {
					zeros.push_back(n2);
					z_size++;
				}
			}
		}

		std::vector<std::string> result;
		if(num != 0) {
			for(int i = 0; i < out.size(); i++) {
				std::string s = conversions.at(i);
				if(out.at(i) != 0 && s.back() == '*') {
					s.pop_back();
					result.push_back(s);
				}
			}
		}

		return result; */
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
		//graph.toposort();
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
