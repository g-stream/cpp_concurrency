#include <exception>
#include <vector>
#include <mutex>
#include <thread>
#include <stack>
#include <iostream>
#include <memory>
struct empty_stack:std::exception{
    const char* what() const throw(){
        return "empty stack";
    }
};
template<typename T, 
            template<typename Elem, typename Allocator = std::allocator<Elem>> 
            class Container = std::vector>
class threadsafe_stack{
    std::stack< T,  Container<T>> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack*) = delete;

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }
    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = std::move(data.top());
        data.pop();
    }
    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
void produce(threadsafe_stack<int, std::vector>& stk){
    int n = 100;
    while(n--){
        stk.push(n);
    }
}
void consume(threadsafe_stack<int, std::vector>& stk){
    while(true){
        try {
            int val;
            stk.pop(val);
            std::cout << "pop:" << val << std::endl;
        } catch (empty_stack& ex){
            std::cout << ex.what() << std::endl;
        }
    }
}
int main(){
    threadsafe_stack<int,std::vector> stk;
    std::thread p(produce, std::ref(stk));
    std::thread c1(consume, std::ref(stk));
    std::thread c2(consume, std::ref(stk));
    p.join();
    c1.join();
    c2.join();
    return 0;
}