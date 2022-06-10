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
	int64_t cur_time = 0, remaining_slice = quantum;
	int cpu = -1;
	std::vector<int64_t> remaining_bursts;

	for(long unsigned int i = 0; i < processes.size(); i++) {
		jq.push_back(i);
		remaining_bursts.push_back(processes.at(i).burst);
	}

	while(1) {
		//std::cout<<"TIME = "<<cur_time<<std::endl;
		if(rq.empty() && jq.empty() && cpu == -1) {
			break;
			//if there are no processes in either the job queue or ready queue, and the cpu is idle, the simulation is done, so break
		}

		if((seq.empty() || seq.back() != cpu) && seq.size() < max_seq_len) {
			seq.push_back(cpu);
			//if a different process is running than what was running previously, add it to the execution sequence
		}

		//add any process that came before the current time
		while(!jq.empty()) {
			int i = jq.front();
			if(processes.at(i).arrival_time < cur_time) {
				//std::cout<<i<<"th process added to rq"<<std::endl;
				rq.push_back(i);
				jq.erase(jq.begin());
				//if the process's start time has come, add it to the ready queue and remove it from the job queue
			}
			else {
				break;
				//since the process are sorted by arrival time, if the first process hasn't come yet, none of them will
			}
		}

		if(cpu != -1) {
			if(remaining_bursts.at(cpu) > 0 && !rq.empty()) {
				rq.push_back(cpu);
				cpu = -1;
				//if the process needs more time to run, add it back to the ready queue
				}
			else if (remaining_bursts.at(cpu) == 0) {
				processes.at(cpu).finish_time = cur_time;
				cpu = -1;
				//otherwise set its finish time
			}
		}

		//add ay processes that arrive at current time
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
				//since the process are sorted by arrival time, if the first process hasn't come yet, none of them will
			}
		}

		if(cpu == -1 && !rq.empty()) {
			//if the CPU is idle, and there are processes ready, take one from the ready queue
			cpu = rq.at(0);
			rq.erase(rq.begin());
			//std::cout<<"switched to process # "<<cpu<<std::endl;

			if(processes.at(cpu).start_time == -1) {
				processes.at(cpu).start_time = cur_time;
				//if this process hasn't run yet, set its start time
			}
		}

		//std::cout<<"RUNNING PROCESS # "<<cpu<<std::endl;

		if(cpu == -1 && !jq.empty()) {
			cur_time = processes.at(jq.at(0)).arrival_time;
		}
		else if (cpu == -1) {
			cur_time++;
		}
		else if(remaining_bursts.at(cpu) <= quantum) {
			//std::cout<<"time left for process: "<<remaining_bursts.at(cpu)<<std::endl;
			cur_time += remaining_bursts.at(cpu);
			remaining_bursts.at(cpu) = 0;

		}
		else {
			//std::cout<<"time left for process: "<<remaining_bursts.at(cpu)<<std::endl;
			cur_time += quantum;
			remaining_bursts.at(cpu) -= quantum;
		}

		//std::cout<<std::endl;
	}
}
