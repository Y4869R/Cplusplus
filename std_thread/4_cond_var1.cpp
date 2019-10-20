//cond_var1.cpp
//用互斥锁实现实现一个生产者消费者模式

#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

//双端队列
std::deque<int> m_queue;

std::mutex m_mutex;

//方案2,增加望江青条件变量,做成"通知--唤醒"模式
std::condition_variable m_condition;

//生产者,往队列中写入数据
void producer() {
  int count = 100;
  while(count > 0) {
    std::unique_lock<std::mutex> ulock(m_mutex);
    m_queue.push_front(count);
    ulock.unlock();

    //方案2 向一个等待线程发送"条件已满足"的通知
    m_condition.notify_one();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    --count;
  }
}


//消费者,从队列中提取数据
void consumer() {
  int data = 0;
  while(data != 1) {
    std::unique_lock<std::mutex> ulock(m_mutex);

    if(!m_queue.empty()) {
      data = m_queue.back();
      m_queue.pop_back();
      ulock.unlock();
      std::cout << "thread consumer got a value from thread producer: " << data << std::endl;
    } else {
      //1.方案1 延迟500ms,降低CPU占用率
      //std::this_thread::sleep_for(std::chrono::milliseconds(500));
      //ulock.unlock();

      //2.方案2.解锁互斥量并陷入睡眠等待通知被唤醒,唤醒后加锁
      //wait中会调用locker的unlock()函数,被唤醒后会调用locker的lock()函数
      //因此可以使用unique_lock,而不能使用lock_guard
      m_condition.wait(ulock);
    }
  }
}

int main() {

  std::thread producer_thread(producer);
  std::thread consumer_thread(consumer);

  if(producer_thread.joinable())
    producer_thread.join();

  if(consumer_thread.joinable())
    consumer_thread.join();


  return 0;
}


