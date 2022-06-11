// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"

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
		//split the string everywhere there is a whitespace

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

		int num_elements = (int) conversions.size();

		if(s >= num_elements) {
			conversions.push_back(start);
		}
		if(e >= num_elements) {
			conversions.push_back(end);
		}

		if (s < num_elements) {
			out_counts.at(s)++;
			//increment the number of edges coming from the start point
		}
		else {
			out_counts.push_back(1);
			std::vector<int> empty;
			adj_list.push_back(empty);
			//if the starting element was not already in the lists, create entries
		}

		if(e < num_elements) {
			adj_list.at(e).push_back(s);
			//otherwise just add the new edge
		}
		else {
			std::vector<int> temp;
			temp.push_back(s);
			adj_list.push_back(temp);
			out_counts.push_back(0);
			//if the ending element was not already in the lists, create entries
		}
	}

	//toposort adapted from pseudocode given in hint for Q1
	std::vector<std::string> toposort() {
		std::vector<int> out = out_counts;
		std::vector<int> zeros;
		int num_left = out.size();

		for(long unsigned int i = 0; i < out.size(); i++) {
			if(out.at(i) == 0) {
				zeros.push_back(i);
			}
		}
		//add any elements with no outgoing edges to the zeros vector

		while(zeros.size() != 0) {
			int n = zeros.back();
			zeros.pop_back();
			num_left--;
			//grab an elements from the stack

			std::vector<int> incoming = adj_list.at(n);
			//get all incoming edges at n
			for(int n2: incoming) {
				//since n is being removed from the graph, all n2 will have one less outgoing edge
				out.at(n2)--;
				if(out.at(n2) == 0) {
					zeros.push_back(n2);
					//if one of n2 now has no outgoing edges, add it to zeros
				}
			}
		}

		std::vector<std::string> results;
		if(num_left != 0) {
			//if there are any elements with > 0 outgoing edges, find them
			for(long unsigned int i = 0; i < out.size(); i++) {
				if(out.at(i) != 0) {
					std::string s = conversions.at(i);

					if(s.back() == '*') {
						//if the element is a process, add it to the result vector
						s.pop_back();
						results.push_back(s);
						num_left--;
					}
				}

				if(num_left == 0) {
					break;
					//once all the elements have been found, stop searching
				}
			}
		}

		return results;
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
