#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;
template<typename T, typename I>
void add_(I begin, I end, T& ans){
    for(;begin!=end;++begin){
        ans += *begin;
    }
}

template<typename T, template<typename A, typename Allocator = std::allocator<A>> class Container = std::vector>
 T sum_parallel(Container<T>& elems){
     auto numThreads = std::thread::hardware_concurrency();
     auto numSize = elems.size()/numThreads;
     vector<T> ans(numThreads);
     vector<thread> threads(numThreads);
     auto start = elems.begin();
     auto end = start;
     for(int i = 0; i < numThreads; ++i){
        std::advance(end, numSize);
        threads[i] = thread(add_<T, decltype(end)>, start, end, ref(ans[i]));
        start = end;
     }
     T tmp{};
    for(; start!=elems.end();++start){
        tmp += *start;
    }
    for(auto& i: threads){
        i.join();
    }
    for(auto const i: ans){
        tmp += i;
    }
    return tmp;
 }


int main()
{
    vector<int> test;
    for(int i = 0; i < 3; ++i){
        test.push_back(i+1);
    }
    cout << sum_parallel<int, vector>(test);
    return 0;
}