#include <queue>
#include <deque>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <random>
template<typename T,
        template<typename Elem, typename Allocator = std::allocator<Elem>>
        class Container = std::deque>
class threadsafe_queue{
private:
    mutable std::mutex m;
    std::queue<T>  data;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){}
    threadsafe_queue(const threadsafe_queue& other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_queue& operator= (const threadsafe_queue& other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
        data_cond.notify_one();
    }
    void pop(T& value){
        std::unique_lock<std::mutex> lock(m);
        data_cond.wait(lock, [this]{return !data.empty();});
        value = std::move(data.front());
        data.pop();
    }
    std::shared_ptr<T> pop(){
        std::unique_lock<std::mutex> lock(m);
        data_cond.wait(lock, [this]{return !data.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data.front())));
        data.pop();
        return res;
    }
    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()){
            return std::shared_ptr<T> {};
        }
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data.front())));
        return res;
    }
    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

};
void produce(threadsafe_queue<int>& q){
    while(true){
        for(int i = 0; i < 100; i++){
            q.push(i);
        }
    }
}
void consume(threadsafe_queue<int>& q){
    while(true){
        int value;
        q.pop(value);
        std::cout << "pop: " << value << std::endl;
    }
}
int main(){
    threadsafe_queue<int> q;
    std::thread t1(produce, std::ref(q));
    std::thread t2(consume, std::ref(q));
    std::thread t3(consume, std::ref(q));
    t1.join();
    t2.join();
    t3.join();
}