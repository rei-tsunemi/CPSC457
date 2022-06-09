// this is the only file you should modify and submit for grading

#include "scheduler.h"
#include "common.h"
#include <iostream>

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
	int64_t cur_time;
	int cpu = -1;
	std::vector<int64_t> remaining_bursts;
	int64_t remaining_slice = quantum - 1;

	for(long unsigned int i = 0; i < processes.size(); i++) {
		jq.push_back(i);
		remaining_bursts.push_back(processes.at(i).burst);
	}

	bool context_switch = false;
	for(cur_time = 0; cur_time < max_seq_len; cur_time++) {
		if(context_switch) {
			//seq.push_back(cpu);
			remaining_slice = quantum - 1;

			if(cpu != -1) {
				if(remaining_bursts.at(cpu) > 0) {
					rq.push_back(cpu);
				}
				else {
					processes.at(cpu).finish_time = cur_time;
				}
			}

			cpu = -1;
		}

		while(!jq.empty()) {
			int i = jq.front();
			if(processes.at(i).arrival_time == cur_time) {
				rq.push_back(i);
				jq.erase(jq.begin());
				//if the process's start time has come, add it to the ready queue and remove it from the job queue
			}
			else {
				break;
			}
		}

		if((cpu == -1 || context_switch) && !rq.empty()) {
			cpu = rq.at(0);
			rq.erase(rq.begin());
			context_switch = false;

			if(processes.at(cpu).start_time == -1) {
				processes.at(cpu).start_time = cur_time;
				//if this process hasn't run yet, set its start time
			}
		}

		if(cpu != -1) {
			remaining_bursts.at(cpu)--;

			if(remaining_bursts.at(cpu) == 0) {
				processes.at(cpu).finish_time = cur_time;
				context_switch = true;
				//if the process is finished, set the end time and set the cpu to idle
			}

			if(remaining_slice == 0 && (!rq.empty() && rq.at(0) != cpu)) {
				context_switch = true;
				//if the quantum expires or the process is done, signal to the simulation that a context switch is needed
			}

			remaining_slice--;
		}
		else {
			context_switch = true;
		}

		seq.push_back(cpu);
	}
}
