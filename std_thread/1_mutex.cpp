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
void job1() {
  m_mutex.lock();
  std::cout << "job1 shared job_shared , keep it for 5ms\n";
  std::this_thread::sleep_for(5 * interval);
  ++job_shared;
  m_mutex.unlock();
}

//修改job_shared和job_exclusive
void job2() {
  while(true) {
    if(m_mutex.try_lock()) {
      ++job_shared;
      std::cout << "job2 lock mutex success, shared(" << job_shared << ")\n";
      m_mutex.unlock();
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

