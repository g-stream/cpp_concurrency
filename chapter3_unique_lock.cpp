#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
void fun(int& a, int& b)
{
    int n = 100000;
    while(n--){
        std::swap(a,b);
    }
}

struct Var{
    int var;
    std::mutex ml;
};

void fun_with_lock(Var& a, Var& b)
{
    int n = 100;
    while(n--){
        std::unique_lock<std::mutex> lock_a(a.ml,std::defer_lock);
        std::unique_lock<std::mutex> lock_b(b.ml, std::defer_lock);
        std::lock(lock_a, lock_b);
        int tmp = a.var;
        a.var = b.var;
        b.var = tmp;
    }
}

int main() {
    int a = 1;
    int b = 0;
    std::cout << "init var: ";
    std::cout << "a: " << a << "b: " << b << "\n" ;
    std::vector<std::thread> threads(10);
    for(auto i = threads.begin(); i != threads.end(); ++i){
        *i = std::thread(fun, std::ref(a), std::ref(b));
    }
    for(auto& i: threads){
        i.join();
    }
    std::cout << "after many times of swap(no lock)" << std::endl;
    std::cout << "a: " << a << "b: " << b << std::endl;


    Var aa,bb;
    aa.var = 1;
    bb.var = 0;
    std::cout << "init var: ";
    std::cout << "a: " << aa.var << "b: " << bb.var << "\n" ;
    for(auto i = threads.begin(); i != threads.end(); ++i){
        *i = std::move(std::thread(fun_with_lock, std::ref(aa), std::ref(bb)));
    }
    for(auto i = threads.begin(); i != threads.end(); ++i){
        (*i).join();
    }
    std::cout << "after many times of swap(no lock)" << std::endl;
    std::cout << "a: " << aa.var << "\nb: " << bb.var;
}