//使用async传递被调用线程返回的结果

#include <iostream>
#include <thread>
#include <future>
#include <numeric>
#include <chrono>
#include <vector>


int accumulate(std::vector<int>::iterator first, std::vector<int>::iterator last) {
  for(int i=5; i>0; --i) {
    std::cout << "In accumulate, wait for " << i << "s" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return std::accumulate(first, last, 0);
}

int main() {

  std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
  std::cout << "Create async task." << std::endl;
  auto accumulate_future = std::async(std::launch::async, accumulate, numbers.begin(), numbers.end() );
  std::cout << "result=" << accumulate_future.get() << std::endl;

  return 0;
}
