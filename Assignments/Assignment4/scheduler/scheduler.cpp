// this is the only file you should modify and submit for grading

#include "scheduler.h"
#include "common.h"
#include <iostream>
#include <limits>
#include <algorithm>

std::vector<int> new_time(
	const int64_t quantum,
	const std::vector<Process> processes,
	int64_t & cur_time,
	std::vector<int>& rq,
	const std::vector<int> jq,
	std::vector<int64_t>& remaining_bursts,
	const int cpu,
	std::vector<int>& seq,
	int max_seq_len
) {
	std::vector<int> res;

	if(cpu == -1 && !jq.empty()) {
		cur_time = processes.at(jq.at(0)).arrival_time;
		//if the cpu is idle and there is a process in the jq, skip to the arrival time of that process
	}
	else if (cpu == -1) {
		cur_time++;
		//if the jq is empty and the cpu is idle, increment time by 1
	}
	else if(rq.empty() && !jq.empty()) {
		//if the ready queue is empty but the job queue is not, find how many quantums can be skipped to get as close
		//to the arrival time of the next process
		int64_t n_quantum = (processes.at(jq.at(0)).arrival_time - cur_time) / quantum;
		//make sur the number of quantums is an integer

		if(n_quantum != 0 && (n_quantum * quantum) <= remaining_bursts.at(cpu)) {
			//if the number of quantms is not 0, and the current process has enough time remaining to
			//skip n_quantums into the future, skip that time
			cur_time += (n_quantum * quantum);
			remaining_bursts.at(cpu) -= (n_quantum * quantum);
		}
		else if(remaining_bursts.at(cpu) <= quantum) {
			cur_time += remaining_bursts.at(cpu);
			remaining_bursts.at(cpu) = 0;
			//otherwise, if the current process did not have enough time left, and that time is <= 1 quantum,
			//skip to the end time of the process
		}
		else {
			cur_time += quantum;
			remaining_bursts.at(cpu) -= quantum;
			//otherwise skip ahead by 1 quantum
		}
	}
	else {
		int64_t min_time = std::numeric_limits<int64_t>::max();
		bool all_started = true, new_proc = false;

		for(int i: rq) {
			all_started = all_started && (processes.at(i).start_time != -1);
			//all_started keeps track if all processes in the ready queue have been run for at least 1 time so far

			if(remaining_bursts.at(i) < min_time ) {
				min_time = remaining_bursts.at(i);
				//min_time keeps track of the shortest amount of time until something happens that requires action
			}
		}
		if(remaining_bursts.at(cpu) < min_time) {
			min_time = remaining_bursts.at(cpu);
			//check if remaining_bursts.at(cpu) is less than the current min_time value
		}
		if(!jq.empty()) {
			new_proc = true;
			//if the job queue has any processes remanining set a boolean to true to track that

			if((processes.at(jq.front()).arrival_time - cur_time) < min_time) {
				min_time = processes.at(jq.front()).arrival_time - cur_time;
				//if the next process's arrival time is less than the min_time, update min_time
			}
		}

		int64_t n_quantum = (min_time / quantum) - 1;
		//calculate the number of quantums that can be skipped
		if(new_proc && n_quantum > 0) n_quantum /= (rq.size() + 1);
		//if there are any processes still in the job queue, divide n_quantum by the number of active processes
		//to ensure the arrival time of the next process is not skipped

		if(n_quantum > 0 && all_started) {
			//if at least 1 quantum can be skipped, and all active processes have been run
			//at least once, skip by n_quantums
			cur_time += n_quantum * quantum * (rq.size() + 1);

			for(int i = 0; i < n_quantum; i++) {
				if((int64_t)seq.size() == max_seq_len) break;

				if((int64_t)seq.size() < max_seq_len && seq.back() != cpu) {
					seq.push_back(cpu);
					//if there is room in seq, add the current cpu process
				}
				for(int n: rq) {
					if((int64_t)seq.size() < max_seq_len && seq.back() != n) {
						seq.push_back(n);
						//if there is room in seq, add the processes in the ready queue
					}
				}
			}

			for(int i = rq.size() - 1; i >= 0; i--) {
				int n = rq.at(i);
				remaining_bursts.at(n) -= n_quantum * quantum;
				//decrease the time remaining at process n

				res.push_back(n);
				rq.pop_back();
				//remove process n from the ready queue and add it to the result vector
			}

			remaining_bursts.at(cpu) -= n_quantum * quantum;
			//decrease the time remaining at the cpu
		}
		else if(remaining_bursts.at(cpu) <= quantum) {
			cur_time += remaining_bursts.at(cpu);
			remaining_bursts.at(cpu) = 0;
			//if n_quantum <=0 or all the processes have not been started, and the current process
			//has a remaining burst <= quantum, set the process as finished
		}
		else {
			cur_time += quantum;
			remaining_bursts.at(cpu) -= quantum;
			//otherwise just skip by 1 quantum
		}
	}

	if(cpu != -1) res.push_back(cpu);
	std::reverse(res.begin(), res.end());
	//add the current cpu process to the result vector and reverse it

	return res;
}

// this is the function you should implement
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//
void simulate_rr(
    int64_t quantum,
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq
) {
	seq.clear();
	std::vector<int> rq, jq;
	int64_t cur_time = 0;
	int cpu = -1;
	std::vector<int64_t> remaining_bursts;

	for(long unsigned int i = 0; i < processes.size(); i++) {
		jq.push_back(i);
		remaining_bursts.push_back(processes.at(i).burst);
		//add all the processes to the job queue and track their remaining bursts
	}

	std::vector<int> cur_procs;
	while(1) {
		if(rq.empty() && jq.empty() && cpu == -1) {
			break;
			//if there are no processes in either the job queue or ready queue, and the cpu is idle,
			//the simulation is done, so break out of the loop to stop
		}

		if((seq.empty() || seq.back() != cpu) && (int64_t)seq.size() < max_seq_len && cur_time != 0 && cur_procs.size() <= 1) {
			seq.push_back(cpu);
			//if a different process is running than what was running previously, and only one process was run in the previous
			//execution sequence, add it to the execution sequence
		}
		//if more than one process was run in the previous chunk of time, the helper function adds them to seq

		//add any process that arrived before the current time to the ready queue
		while(!jq.empty()) {
			int i = jq.front();
			if(processes.at(i).arrival_time < cur_time) {
				rq.push_back(i);
				jq.erase(jq.begin());
				//if the process's start time has come, add it to the ready queue and remove it from the job queue
			}
			else {
				break;
				//since the process are sorted by arrival time, if the first process hasn't come yet, none of them will
			}
		}

		for(int i: cur_procs) {
			if(remaining_bursts.at(i) > 0) {
				rq.push_back(i);
				//if the process needs more time to run, add it back to the ready queue
			}
			else {
				processes.at(cpu).finish_time = cur_time + remaining_bursts.at(cpu);
				//otherwise set its finish time
			}
		}

		if(!cur_procs.empty()) {
			cpu = -1;
			//if any processes were run in the previous time chunk, set the cpu to idle
		}

		//add ay processes that arrive at current time
		while(!jq.empty()) {
			int i = jq.front();
			if(processes.at(i).arrival_time == cur_time) {
				rq.push_back(i);
				jq.erase(jq.begin());
				//if the process's start time has come, add it to the ready queue and remove it from the job queue
			}
			else {
				break;
				//since the process are sorted by arrival time, if the first process hasn't come yet, none of them will
			}
		}

		if(cpu == -1 && !rq.empty()) {
			//if the CPU is idle, and there are processes ready, take one from the ready queue
			cpu = rq.at(0);
			rq.erase(rq.begin());

			if(processes.at(cpu).start_time == -1) {
				processes.at(cpu).start_time = cur_time;
				//if this process hasn't run before, set its start time
			}
		}

		cur_procs = new_time(quantum, processes, cur_time, rq, jq, remaining_bursts, cpu, seq, max_seq_len);
		//use a helper method to find the next time to jump to
		//cur_procs will hold all processes that were run in that time
	}
}
