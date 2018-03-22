// ways to solve the data race problem 
// 1. add some protection mechanism to your data structure which will cause data race.
//end ensure that only the thread actually performing a modification can see the intermediate tates where the invariants asre broken.
// 2. modify the design of your data structure and its invariants so that modifications are done as a series of indivisible changes.
// this is generally referred to as lock-free programming .
// 3. handle the updates to the data structure as a transaction, just as updates to a database are done within a transaction.
// Store series of data modificatons and reads in a transaction log and then committ in a single step.
//here introduce data protection mechanism, and remember DONOT pass pointers and reference to protected data outside the sdcope fo the lock, whether by returning them from a function, storing them in externally visible memory, or passing them as arguments to user-supplied functions.

#include <list>
#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <thread>
std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}
bool list_contains(int value_to_find){
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}
int main () {
    std::vector<std::thread> threads(10);
    for(decltype(threads.size()) i = 0; i < threads.size(); ++i){
          threads[i] = std::thread{add_to_list, i};
    }
    for(auto i = threads.begin(); i != threads.end(); ++i){
       (*i).join();
    }
    for(auto i: some_list){
        std::cout << i << " ";
    }
}