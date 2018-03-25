#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>
//this example shows how to use acquire-release semantics to get the right execute ordering. 
std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);
void thread_1(){
    data[0].store(42,std::memory_order_relaxed);
    data[1].store(97,std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141,std::memory_order_relaxed);
    data[4].store(2003,std::memory_order_relaxed);
    sync1.store(true,std::memory_order_release);
}

void thread_2(){
    while(!sync1.load(std::memory_order_acquire));
    sync2.store(true,std::memory_order_release);
}
void thread_3(){
    while(!sync2.load(std::memory_order_acquire));
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == -141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
}


std::atomic<int> sync(0);
void thread_another_1(){
    data[0].store(42,std::memory_order_relaxed);
    data[1].store(97,std::memory_order_relaxed);
    data[2].store(17,std::memory_order_relaxed);
    data[3].store(-141,std::memory_order_relaxed);
    data[4].store(2003,std::memory_order_relaxed);
    sync.store(1,std::memory_order_release);
}

void thread_another_2(){
    int expected = 1;
    while(!sync.compare_exchange_strong(expected, 2, std::memory_order_acq_rel))
    expected = 1;
}
void thread_another_3(){
    
    while(sync.load(std::memory_order_acquire) < 2);
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == -141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
}
int main(){
    std::thread t1(thread_1);
    std::thread t2(thread_2);
    std::thread t3(thread_3);
    t1.join();
    t2.join();
    t3.join();
    for(auto& i: data){
        i = -1;
    }
    t1 = std::thread(thread_another_1);
    t2 = std::thread(thread_another_2);
    t3 = std::thread(thread_another_3);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
