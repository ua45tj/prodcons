cmake_minimum_required(VERSION 3.1)

project(prodcons CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(prodcons)

target_link_libraries(prodcons
  Threads::Threads
)

target_sources(prodcons
 PRIVATE
  src/main.cc
  src/my_queue.cc
  src/my_queue.h
  src/prodcons.cc
  src/prodcons.h
  src/raii_log_func.h
)
