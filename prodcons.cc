// !g++ -std=c++11 -pthread prodcons.cc && ./a.out
// TODO Add a build system: Makefile or CMake

#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <unistd.h> // TODO Replace with C++11 sleep

class RaiiLogFunc {
 public:
  RaiiLogFunc(const std::string& name) 
      : name_(name) {
    std::cout << name_ << " ENTER" << std::endl;
  }
  ~RaiiLogFunc() {
    std::cout << name_ << " EXIT" << std::endl;
  }
 
 private:
  std::string name_;
};

#define RAII_LOG_FUNC RaiiLogFunc raii_log_func(__func__)

std::mutex mutex;
std::queue<int> queue;
int queue_max_size = 0;

int BuildItem() {
  // TODO Replace with C++11 random
  return rand();
}

void ProduceItem() {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(BuildItem());
  queue_max_size = std::max(queue_max_size, static_cast<int>(queue.size()));
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
  // TODO Replace with C++11 sleep
  usleep(10);
}

void ProducerThread() {
  RAII_LOG_FUNC;
  
  for (int i = 0; i < 1000; ++i) {
    std::cout << 'p';
    ProduceItem();
    Sleep();
  }
}

void ConsumerThread() {
  RAII_LOG_FUNC;

  for (int i = 0; i < 1000; ++i) {
    std::cout << 'c';
    ConsumeItem();
    Sleep();
  }
}

int main(int argc, char* argv[]) {
  RAII_LOG_FUNC;

  std::thread producer_thread(ProducerThread);
  std::thread consumer_thread(ConsumerThread);

  producer_thread.join();
  consumer_thread.join();

  std::cout << std::endl;
  std::cout << "queue.size() " << queue.size() << std::endl;
  std::cout << "queue_max_size " << queue_max_size << std::endl;

  return 0;
}
