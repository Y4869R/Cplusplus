//使用promise传递被调用线程返回结果，通过共享状态变化通知调用线程已获得结果


#include <thread>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>
#include <future>

void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise){
  int sum = std::accumulate(first, last, 0);
  //将结果存入，同时让共享状态变为就绪，提醒future
  accumulate_promise.set_value(sum);
}

void do_work(std::promise<void> barrier) {
  std::cout << "do_work: wait for 3s." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  barrier.set_value();
  std::cout << "do_work: finish waiting." << std::endl;
}

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6 };
  std::promise<int> accumulate_promise;
  std::future<int> accumulate_future = accumulate_promise.get_future();
  std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise) );

  //future::get() will wait until the future has a valid result and retrieves it.
  //Calling wait() before get() is not needed
  accumulate_future.wait();
  std::cout << "result=" << accumulate_future.get() << std::endl;
  work_thread.join();

  //using promise<void> to signal state between threads.
  std::promise<void> barrier;
  std::future<void> barrier_future = barrier.get_future();
  std::thread new_work_thread(do_work, std::move(barrier));
  barrier_future.wait();
  new_work_thread.join();

  return 0;
}

