#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
void add(I begin,I end, T& ans){
    for(;begin!=end;++begin){
        ans += *begin;
    }
}
template<typename T, template<A, Allocator = allocator<A>> class Container = std::vector>
 T sum_parallel(Container<T>& elems){
     auto numThreads = std::thread::hardware_concurrency();
     auto numSize = elems.size()/numThreads;
     vector<T> ans(nunThreads);
     auto start = elems.begin();
     auto end = start;

     for(int i = 0; i < nunThreads; ++i){

     }
 }
template<typename I, typename T>

using namespace std;
int main()
{
    
    return 0;
}