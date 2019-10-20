//thread1.cpp
//创建线程.查看并发执行和阻塞等待
//
//


#include <iostream>
#include <thread>
#include <chrono>


using namespace std;

void thread_function(int n) {
  //get thread id
  std::thread::id this_id = std::this_thread::get_id();

  for(int i= 0; i < 5 ; i++) {
    cout << "Child function thread " << this_id << " is running : " << i+1 << endl;
    //sleep for n seconds
    std::this_thread::sleep_for(std::chrono::seconds(n));
  }
}

class Thread_functor {
  public:
    //overload operator ()
    void operator()(int n) {
      std::thread::id this_id = std::this_thread::get_id();
      for(int i= 0; i < 5 ; i++) {
        cout << "Child functor thread " << this_id << " is running : " << i+1 << endl;
        //sleep for n seconds
        std::this_thread::sleep_for(std::chrono::seconds(n));
      }
    }
};

int main() {

  cout << "#### Start creat child thread1. ####" << endl;
  //传递函数指针作为参数初始化子线程
  //使用join()函数阻塞主线程直到子线程1执行完成
  thread mthread1(thread_function, 1);
  if(mthread1.joinable())
    mthread1.join();
  cout << "#### Back to Main thread. ####" << endl;

  cout << "#### Start creat child thread2. ####" << endl;
  //传递函数对象作为参数初始化子线程
  //使用detach()函数让子线程2和主线程并行运行
  Thread_functor thread_functor;
  thread mthread2(thread_functor, 3);
  if(mthread2.joinable())
    mthread2.detach();
  cout << "#### Back to Main thread. ####" << endl;


  auto thread_lambda= [](int n) {
      std::thread::id this_id = std::this_thread::get_id();
      for(int i= 0; i < 5 ; i++) {
        cout << "Child lambda thread " << this_id << " is running : " << i+1 << endl;
        //sleep for n seconds
        std::this_thread::sleep_for(std::chrono::seconds(n));
      }
  };
  cout << "#### Start creat child thread3. ####" << endl;
  //使用lambda表达式作为参数初始化子线程3
  //使用join()函数阻塞主线程直到子线程3执行完成
  thread mthread3(thread_lambda, 4);
  if(mthread3.joinable())
    mthread3.join();
  cout << "#### Back to Main thread. ####" << endl;


  std::thread::id this_id = std::this_thread::get_id();
  for(int i= 0; i < 5 ; i++) {
    cout << "Main thread " << this_id << " is running : " << i+1 << endl;
    //sleep for n seconds
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  getchar();
  return 0;
}


