#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
struct empty_stack: std::exception{
    const char* what() const noexcept{
        return "empty stack!";
    };
};
template<typename T>
class threadsafe_stack{
    private:
    std::stack<T> data;
    mutable std::mutex m;
    public:
    threadsafe_stack():data(std::stack<T>()){};
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    void push (T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        return res;
    }
    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

void op(threadsafe_stack<std::string>& stk, int id){
    std::string tmp = std::string("id") + std::to_string(id) + " added : ";
    int n = 50;
    while(n--){
        stk.push(tmp + std::to_string(n));
    }
};

int main(){
    threadsafe_stack<std::string> stk;
    
    std::vector<std::thread> threads(10);
    int k = 0;
    for(auto i = threads.begin(); i != threads.end(); ++i,k++){
        *i = std::thread(op, std::ref(stk), k);
    }
    for(auto i = threads.begin(); i != threads.end(); ++i){
        (*i).join();
    }
    int num = 0;
    while(true){
        try{
            stk.empty();
            std::string tmp = *(stk.pop());
            std::cout << tmp << std::endl;
            stk.pop(tmp);
            num++;
        }catch(...){
            std::cout << "add " << num << " times" << std::endl;
            return 0;
        }
    }

    return 0;
}

