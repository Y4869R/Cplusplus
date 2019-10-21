//利用packaged_task传递被调用线程返回结果，通过共享状态变化通知调用线程已获得结果
//利用promise和packaged_task通过函数参数与返回值来传递数据，可以降低函数间的耦合度


#include <thread>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <future>

int accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last) {
  int sum = std::accumulate(first, last, 0);
  //将结果存入，同时让共享状态变为就绪，提醒future
  //accumulate_promise.set_value(sum);

  //对于packaged_task,执行结果返回值或者异常被储存在能通过std::future对象访问的共享状态中
  return sum;
}

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6 };
  //std::promise<int> accumulate_promise;
  //std::future<int> accumulate_future = accumulate_promise.get_future();
  //std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise) );
  //利用packaged_task来在线程之间传递结果
  std::packaged_task<int(std::vector<int>::iterator, std::vector<int>::iterator)> accumulate_task(accumulate);
  std::future<int> accumulate_future = accumulate_task.get_future();
  std::thread work_thread(std::move(accumulate_task), numbers.begin(), numbers.end());

  accumulate_future.wait();
  std::cout << "result=" << accumulate_future.get() << std::endl;
  work_thread.join();

  return 0;
}

