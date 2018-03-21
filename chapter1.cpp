#include <iostream>
#include <thread>
#include <string>
void fun(std::string name){
    std::cout << "fun says Hello " << name <<  "!\n";
}
class FunObj{
    public: 
    void operator()(std::string name){
        std::cout << "fun object says Hello " << name <<  "!\n";
    }
};
int main (){
    std::thread t1(fun, "yinly");
    std::thread t2(FunObj{},"yinly");
    t1.join();
    t2.join();
    return 0;
}