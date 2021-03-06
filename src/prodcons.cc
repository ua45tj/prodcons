#include "prodcons.h"

#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

#include "my_queue.h"
#include "raii_log_func.h"

namespace {

MyQueue my_queue;  // Thread safe queue implementation.
std::condition_variable cv;
std::mutex cv_mutex;
bool is_ready = false;
bool has_stopped = false;
int queue_max_size = 0;
int total_produced = 0;

int BuildItem() {
  thread_local std::random_device rd;
  thread_local std::mt19937 generator(rd());
  thread_local std::uniform_int_distribution<> distribution(1, 9);

  const int item = distribution(generator);
  return item;
}

void ProduceItem() {
  int item = BuildItem();
  {
    my_queue.Push(item);
    queue_max_size = std::max(queue_max_size, static_cast<int>(my_queue.Size()));
    ++total_produced;
    is_ready = true;
  }
  cv.notify_one();
}

void HandleItem(int item) {
  std::cout << item;
}

void ConsumeItem() {
  std::unique_lock<std::mutex> lock(cv_mutex);
  cv.wait(lock, []{ return is_ready || (my_queue.Size() > 0) || has_stopped; });
  if (has_stopped) {
    return;
  }
  int item = my_queue.Pop();
  int has_consumed_item = (item != 0);
  is_ready = false;
  lock.unlock();

  if (has_consumed_item) {
    HandleItem(item);
  }
}

void Sleep() {
  std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void ProducerThread() { RAII_LOG_FUNC;
  for (int i = 0; i < 1000; ++i) {
    std::cout << 'p';
    ProduceItem();
    Sleep();
  }
}

void ConsumerThread() { RAII_LOG_FUNC;
  for (int i = 0; i < 1000; ++i) {
    std::cout << 'c';
    ConsumeItem();
    Sleep();
  }
}

}  // namespace

Prodcons::Prodcons(int argc, char* argv[]) { RAII_LOG_FUNC;
}

int Prodcons::Run() { RAII_LOG_FUNC;
  std::thread producer_thread1(ProducerThread);
  std::thread producer_thread2(ProducerThread);
  std::thread producer_thread3(ProducerThread);

  std::thread consumer_thread1(ConsumerThread);
  std::thread consumer_thread2(ConsumerThread);
  std::thread consumer_thread3(ConsumerThread);

  producer_thread1.join();
  producer_thread2.join();
  producer_thread3.join();

  {
    std::lock_guard<std::mutex> lock(cv_mutex);
    has_stopped = true;
  }
  cv.notify_all();

  consumer_thread1.join();
  consumer_thread2.join();
  consumer_thread3.join();

  std::cout << std::endl;
  std::cout << "my_queue.size() " << my_queue.Size() << std::endl;
  std::cout << "queue_max_size " << queue_max_size << std::endl;
  std::cout << "total_produced " << total_produced << std::endl;

  return 0;
}
