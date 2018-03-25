#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int sleep_one_second(int n){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return n;
}
class X{
    public:
    X(int _x):x(_x){};
    int fun_delay(){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return x;
    }
    private:
    int x;
};
int main(){
    std::future<int> ans = std::async(sleep_one_second, 1);
    X x(100);
    std::cout << "create an async thread" << std::endl;
    std::cout << ans.get() << std::endl;
    auto ans_memfun_with_class_value = std::async(&X::fun_delay, x);

    std::cout << "create an async thread for class x value" << std::endl;
    std::cout << ans_memfun_with_class_value.get() << std::endl;

    auto ans_memfun_with_class_pointer = std::async(&X::fun_delay, &x);
    std::cout << "create an async thread for class x pointer" << std::endl;
    std::cout << ans_memfun_with_class_pointer.get() << std::endl;

    auto ans1 = std::async(std::launch::deferred, sleep_one_second,1);//invoke the function in get() or wait() 
    auto ans2 = std::async(std::launch::async, sleep_one_second,1);//run in a new thread
    auto ans3 = std::async(std::launch::deferred|std::launch::async, sleep_one_second,1); //the default optoin which means the implementation will choose one of the choice
    ans1.wait();// now the func passed into the  future<int> ans1 will be executed

    //shared_future is used in the saturation that many threads  want to get one return value of a function invoke.
    //hightline: many threads get one var will cause data race, but we can let threads keep the copy of the shared_future to solve the problem.
    //future is only moveable, but shared_future is copyable.
    std::shared_future<int> sf1 = std::move(ans1);
    std::cout << (ans1.valid()?"valid":"not valid") << std::endl;
    auto sf2 = sf1;
    std::cout << (sf1.valid()?"valid":"not valid") << std::endl;
    auto sf3 = ans2.share();
    std::cout << (sf3.valid()?"valid":"not valid") << std::endl;

    return 0;    
}