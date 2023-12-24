#ifndef TPL_SOURCE_WORKER_HPP
#define TPL_SOURCE_WORKER_HPP

#include "Base.hpp"
#include "JobContract.hpp"

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>

namespace tpl {

class WorkerManager;

#if __cplusplus >= 201402L && __cplusplus < 202002L

class Worker {
  friend class WorkerManager;

public:
  Worker() = delete;
  Worker(const Worker &) = delete;
  Worker(Worker &&) noexcept = default;
  Worker &operator=(Worker &&) = default;
  Worker &operator=(const Worker &) noexcept = delete;

  ~Worker() noexcept { this->terminate(); }

private:
  explicit Worker(std::function<void(Worker &)> workerLoop)
      : mThread{workerLoop, std::ref(*this)} {}

  void terminate() noexcept {
    // std::mutex mtx;
    // std::unique_lock<std::mutex> lock{mtx};

    // if (!this->mShouldTerminate) {
    //   return;
    // }

    // std::cout << std::boolalpha << this->mShouldTerminate << std::endl;

    if (this->mThread.joinable()) {
      this->mThread.join();
    }
  }

private:
  bool mShouldTerminate = false;
  std::thread mThread;
};

#else // C++20 or later

class Worker {};

#endif // C++14 or C++17

} // namespace tpl

#endif // TPL_SOURCE_WORKER_HPP