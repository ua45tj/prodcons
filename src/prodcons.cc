#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <random>

#include "raii_log_func.h"

std::mutex mutex;
std::queue<int> queue;
int queue_max_size = 0;
int total_produced = 0;

int BuildItem() {
  thread_local std::random_device rd;
  thread_local std::mt19937 generator(rd());
  thread_local std::uniform_int_distribution<> distribution(0, 9);

  const int item = distribution(generator);
  return item;
}

void ProduceItem() {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(BuildItem());
  queue_max_size = std::max(queue_max_size, static_cast<int>(queue.size()));
  ++total_produced;
}

void HandleItem(int item) {
  std::cout << item;
}

void ConsumeItem() {
  int item;
  int has_consumed_item = false;

  {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.size() > 0) {
      item = queue.front();
      queue.pop();
      has_consumed_item = true;
    }
  }

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

int main(int argc, char* argv[]) { RAII_LOG_FUNC;
  std::thread producer_thread1(ProducerThread);
  std::thread producer_thread2(ProducerThread);
  std::thread producer_thread3(ProducerThread);

  std::thread consumer_thread1(ConsumerThread);
  std::thread consumer_thread2(ConsumerThread);
  std::thread consumer_thread3(ConsumerThread);

  producer_thread1.join();
  producer_thread2.join();
  producer_thread3.join();
  consumer_thread1.join();
  consumer_thread2.join();
  consumer_thread3.join();

  std::cout << std::endl;
  std::cout << "queue.size() " << queue.size() << std::endl;
  std::cout << "queue_max_size " << queue_max_size << std::endl;
  std::cout << "total_produced " << total_produced << std::endl;

  return 0;
}
