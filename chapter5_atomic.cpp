#include <iostream>
#include <atomic>
#include <thread>

//std::atomic<>  has method load(),store(),exchange(),compare_exchange_weak() compare_exchange_strong()
//Store: memeory_order_relaxed, memory_order_release, memory_order_seq_cst
//Load: memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst
//Read-modift-write operation:  memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst
//all above have default mode -- memory_order_seq_cst
class spinlock_mutex
{
    std::atomic_flag flag;
    public:
    spinlock_mutex():flag(ATOMIC_FLAG_INIT){}  // atomic_flag should be initialized with ATMOIC_FLAG_INIT
    void lock(){
        while(flag.test_and_set(std::memory_order_acquire));
    }
    void unlock(){
        flag.clear(std::memory_order_release);
    }
};
spinlock_mutex mutex{};
void fun_spin(int i){
    while(true){
        mutex.lock();
        std::cout<<"called by: " << i <<std::endl;
        mutex.unlock();
    }

}
int main(){
    std::thread t1(fun_spin, 1);
    std::thread t2(fun_spin, 2);
    t1.join();
    t2.join();
    return 0;


}