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
        /*std::lock(a.ml,b.ml);
        std::lock_guard<std::mutex>(a.ml,std::adopt_lock);
        std::lock_guard<std::mutex>(b.ml, std::adopt_lock);*/ //两个互斥量,并且两个 	std:lock_guard	实例已经创建好了。提供 	std::adopt_lock	参数除了
                                                              //表示 	std::lock_guard	对象可获取锁之外,还将锁交由 std::lock_guard	 对象管理,而不需
                                                              //要 	std::lock_guard	对象再去构建新的锁。（测试中，不用该参数会造成死锁）

        std::lock_guard<std::mutex>(a.ml);
        std::lock_guard<std::mutex>(b.ml);  //这里所有的线程用相同的顺序取锁，所以没有发生死i锁的问题，而上面注释部分的代码则用到了对锁的锁，c++库中的lock特性， 一次性锁住了两个互斥量。
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
    for(auto i = threads.begin(); i != threads.end(); ++i) {
        (*i).join();
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