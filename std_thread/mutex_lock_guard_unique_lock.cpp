//mutex1.cpp
//通过互斥体lock与unlock保护全局变量


#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

std::chrono::milliseconds interval(100);

std::mutex m_mutex;
//两个线程都能修改的变量,需要mutex的保护
int job_shared = 0;

//只有一个线程能修改,不需要保护
int job_exclusive = 0;

//只修改job_shared
//使用RAII智能锁, std::lock_guard
void job1() {
  //构造lock_guard的时候自动lock()
  std::lock_guard<std::mutex> lockg(m_mutex);
  //m_mutex.lock();
  std::cout << "job1 shared job_shared , keep it for 5ms\n";
  std::this_thread::sleep_for(5 * interval);
  ++job_shared;
  //m_mutex.unlock();
  //析构lock_guard的时候自动unlock()
}

//修改job_shared和job_exclusive
//std::lock_guard不支持try_lock()
//使用std::unique_lock
void job2() {
  while(true) {
    //if(m_mutex.try_lock()) {
    std::unique_lock<std::mutex> ulock(m_mutex, std::try_to_lock);
    //等同于
    //std::unique_lock<std::mutex> ulock(m_mutex, std::defer_lock); ulock.try_lock();
    if(ulock) {
      ++job_shared;
      std::cout << "job2 lock mutex success, shared(" << job_shared << ")\n";
      //m_mutex.unlock();
      return;
    } else {
      ++job_exclusive;
      std::cout << "job2 lock mutex fail, exclusive(" << job_exclusive << ")" << std::endl;
      std::this_thread::sleep_for(interval);
    }
  }
}

int main() {

  std::thread mthread1(job1);
  std::thread mthread2(job2);

  if(mthread1.joinable())
    mthread1.join();
  if(mthread2.joinable())
    mthread2.join();

  getchar();

  return 0;
}

