// #include <unordered_map>
// #include <vector>
// #include <utility>
// #include <stdio.h>
// #include <algorithm>

// typedef std::pair<int, int> intpair;

// struct MyComp {
//     bool operator()(const intpair& a, const intpair& b) const {
//         return a.second < b.second;
//     }
// };

// int main() {
//     //square bracket operator for maps is special
//     std::vector<std::pair<int, int>> myVec;

//     myVec.push_back({1, 2});
//     myVec.push_back({1, 1});
//     myVec.push_back({2, 3});
//     myVec.emplace_back(2, 3);

//     //if i want to sort the vector of pairs wrt the second element
//     std::sort(myVec.begin(), myVec.end(), MyComp{});

//     for(const auto& i: myVec) {
//         printf("{%d, %d}\n", i.first, i.second);
//     }
// }

#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

struct Input {
    long start, end;
    long* resultPtr;

};

void * parallelWork(void* inputArg) {
    Input * in = (Input*) inputArg;
    long start = in->start, end = in->end;
    long temp = 0;
    for(long i = start; i < end; ++i) {
        temp += i;
    }
    *(in->resultPtr) = temp;

    //return 0;
    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    long partialResults[NUM_THREADS] = {0};
    struct Input inputs[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; ++i) {
        inputs[i] = {.start = i*10, .end = (i+1)*10 - 1, &partialResults[i]};
        pthread_create(&threads[i], NULL, parallelWork, &inputs[i]);
    }

    for(int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    long finalAns;
    for(int i =0; i < NUM_THREADS; ++i) {
        finalAns += partialResults[i];
    }

    printf("answer = %d\n", finalAns);
}