//std::list is a doubly linked list (use in A5)
//be careful and read documentation for insertion

#include <list>
#include <iostream>

void print_list(std::list<int>& list) {
    std::list<int>::iterator myIter;
    //std::list<int>::const_iterator myIterConst;
    //auto someIter = list.begin();

    //for(const auto& i: list) {}

    for(std::list<int>::iterator i = list.begin(); i != list.end(); ++i) {
        std::cout<<*i<<", ";
    }
    std::cout<<"\n";
}

int main() {
    std::list<int> myList;

    myList.emplace_back(1);
    myList.emplace_back(2);
    myList.emplace_back(4);

    print_list(myList);

    //insert 3 after 2
    for(std::list<int>::iterator i = myList.begin(); i != myList.end(); ++i) {
        if(*i == 2) {
            myList.insert(std::next(i), 3);
        }
    }
    //recommendation: always use std::next
    print_list(myList);
    std::cout<<"++myList.begin() = "<<*(myList.begin())<<"\n";

    //delete 2 and print out every element of the list
    /*for(std::list<int>::iterator i = myList.begin(); i != myList.end(); ++i) {
        if(*i == 2) {
            auto copyIter = i;
            i = std::prev(i); //do previous of i since when we continue we will increment i
            myList.erase(copyIter);
            continue;
        }
        std::cout<<*i<<", ";
    }
    std::cout<<"\n";
    */

   std::list<int>::iterator i = myList.begin();
   while(i != myList.end()) {
        if(*i == 2) {
           auto copyIter = i;
            i = std::prev(i); //do previous of i since when we continue we will increment i
            myList.erase(copyIter);
            continue;
        }

        i = std::next(i);
    }
    print_list(myList);
}

/*
 * Q2 DFS:
 * 
 * for each "X":
 *   //<node index, length of chain>
 *   stack<int, int>; //might not be int, beware of overflow
 * 
 *   //start at X
 *   stack.push({"X", 1});  //start count might be 0 or 1, check yourself
 * 
 *   while(!stack.empty()) {
 *      [node_index, chainLength] = s.top(); //copy the values! top() returns a reference
 *      s.pop();
 * 
 *      //leaf case
 *      if(adjList[node_index].size() == 0) {
 *          currentMax = std::max(currentMax, chainLength);
 *      }
 * 
 *      //nonleaf case
 *      for(const auto& i: adjList[nodeIndex]) {
 *          //push neighbours
 *          s.push({i, chainLength + 1});
 *      }
 *   }
 */