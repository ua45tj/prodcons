#include "my_queue.h"

void MyQueue::Push(int value) {
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(value);
}

int MyQueue::Pop() {
  int value = 0;
  std::lock_guard<std::mutex> lock(mutex_);
  if (!queue_.empty()) {
    value = queue_.front();
    queue_.pop();
  }
  return value;
}

size_t MyQueue::Size() {
  std::lock_guard<std::mutex> lock(mutex_);
  return queue_.size();
}
