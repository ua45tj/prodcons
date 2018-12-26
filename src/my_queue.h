#ifndef MY_QUEUE_H_
#define MY_QUEUE_H_

#include <mutex>
#include <queue>

class MyQueue {
 public:
  void Push(int value);
  int Pop();
  size_t Size();

 private:
  std::queue<int> queue_;
  std::mutex mutex_;
};

#endif  // MY_QUEUE_H_
