#include <future>
#include <condition_variable>
#include <iostream>
#include <chrono>

int main()
{
    using sck = std::chrono::system_clock;//系统时钟，通过调节系统的时间会发生不一至的现象
    using stk = std::chrono::steady_clock;//稳定的，不随你调系统时间而变化
    using ratio1 = std::ratio<60,7>; // 时间间隔为60/7秒
    using hrc = std::chrono::high_resolution_clock;	 	//最高精度的钟
    using duration_sec = std::chrono::duration<short, std::ratio<60,1>>;	//存在short中，单位为一分钟
    using duration_sec2 = std::chrono::seconds;
    using duration_millisec = std::chrono::duration<double,std::ratio<1,	1000>>;//存在double中，单位毫秒
    using duration_millisec2 = std::chrono::milliseconds;
}