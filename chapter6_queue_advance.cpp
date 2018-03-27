#include <iostream>
#include <memory>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <atomic>
template<typename T> 
class queue{
private:
    struct node{
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;

    node* get_tail(){
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head(){
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data(){
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]{return head.get()!=get_tail();});
        return std::move(head_lock);
    }

    std::unique_ptr<node> wait_pop_head(){
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value){
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value =     std::move(*head->data);
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(){
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail()){
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value){
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail()){
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }

public:
    queue():head(new node), tail(head.get()){}
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;
    std::shared_ptr<T> try_pop(){
        if(!head){
            return std::shared_ptr<T>{};
        }
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(head->data)));
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }
    std::shared_ptr<T> wait_and_pop(){
        std::unique_ptr<node> const old_data = wait_pop_head();
        return old_data->data;
    }
    void wait_and_pop(T& value){
        std::unique_ptr<node> const old_head = wait_pop_head(value);
    }
    void push(T new_value){
		std::shared_ptr<T>	new_data(
		std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node>	p(new	node);
		{
				std::lock_guard<std::mutex>	tail_lock(tail_mutex);
				tail->data=new_data;
				node*	const	new_tail=p.get();
				tail->next=std::move(p);
				tail=new_tail;
		}
		data_cond.notify_one();
    }

    bool empty(){
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};
std::atomic<bool> stop(true);
std::atomic<bool> finish(false);
void produce(queue<int>& q, int n){
    while(stop.load());
    for(int i = 0; i < n; i++){
        q.push(i);
    }
    finish.store(true);
}
void consume(queue<int>& q){
    while(stop.load());
    while(true){
        int value;
        q.wait_and_pop(value);
        std::cout << "pop: " << value << std::endl;
        if(finish.load()&&q.empty()) break;
    }
}
int main(){
    queue<int> q;
    std::thread t1(produce, std::ref(q), 10000);
    std::thread t2(consume, std::ref(q));
    auto now = std::chrono::steady_clock::now();
    stop = false;
    t1.join();
    t2.join();
    auto duration = std::chrono::steady_clock::now() - now;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << std::endl;
}
