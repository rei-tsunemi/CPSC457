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
	int64_t cur_time = 0;
	int cpu = -1;
	std::vector<int64_t> remaining_bursts;
	int64_t remaining_slice = quantum - 1;

	for(long unsigned int i = 0; i < processes.size(); i++) {
		jq.push_back(i);
		remaining_bursts.push_back(processes.at(i).burst);
	}

	bool context_switch = false;
	while(1) {
		//std::cout<<"TIME = "<<cur_time<<std::endl;
		if(rq.empty() && jq.empty() && cpu == -1) {
			break;
			//if there are no processes in either the job queue or ready queue, and the cpu is idle, the simulation is done, so break
		}

		if(context_switch) {
			if(cur_time <= max_seq_len && seq.back() != cpu) {
				seq.push_back(cpu);
			}

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
				//std::cout<<i<<"th process added to rq"<<std::endl;
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
			//std::cout<<"switched to process # "<<cpu<<std::endl;

			if(processes.at(cpu).start_time == -1) {
				processes.at(cpu).start_time = cur_time;
				//if this process hasn't run yet, set its start time
			}
		}

		//std::cout<<"RUNNING PROCESS # "<<cpu<<std::endl;

		if(cpu != -1) {
			remaining_bursts.at(cpu)--;

			if(remaining_bursts.at(cpu) == 0) {
				processes.at(cpu).finish_time = cur_time;
				context_switch = true;
				//if the process is finished, set the end time and set the cpu to idle
			}

			//std::cout<<remaining_slice<<" time left until switch"<<std::endl;
			if(remaining_slice == 0 && (rq.empty() || rq.at(0) != cpu)) {
				context_switch = true;
				//if the quantum expires or the process is done, signal to the simulation that a context switch is needed
			}

			remaining_slice--;
		}
		else {
			context_switch = true;
		}

		if(cur_time == max_seq_len) {
			seq.push_back(cpu);
		}

		cur_time++;
		//std::cout<<std::endl;
	}
}
