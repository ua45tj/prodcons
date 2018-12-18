#ifndef RAII_LOG_FUNC_H_
#define RAII_LOG_FUNC_H_

#include <iostream>

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

#endif  // RAII_LOG_FUNC_H_
