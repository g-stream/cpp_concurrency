#include <atomic>
#include <thread>
#include <queue>
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>
std::vector<int>	queue_data;
std::atomic<int> count;
void	populate_queue()
{
        int n;
        while(true){
            std::cout << "input num: \n";

            scanf("%d",&n);

		    assert(n>0);
		    queue_data.clear();
		    for(int	i=0;i < n;++i){
        				queue_data.push_back(i);
		    }
		    count.store(n ,std::memory_order_release);
        }

}
void	consume_queue_items()
{
		while(true)
		{
				int	item_index;
				if((item_index=count.fetch_sub(1,std::memory_order_acquire))<=0)		//	2	一个“读-改-写”操作
				{
						std::this_thread::sleep_for(std::chrono::microseconds(1));		//	3	等待更多元素
						continue;
				}
				std::cout << queue_data[item_index-1] << std::endl;		//	4	安全读取queue_data
        }
}

int main()
{
    	std::thread	a(populate_queue);
		std::thread	b(consume_queue_items);
		std::thread	c(consume_queue_items);
        b.detach();
        c.detach();
		a.join();
}