#ifndef TPL_SOURCE_WORKER_HPP
#define TPL_SOURCE_WORKER_HPP

#include "Base.hpp"
#include "JobContract.hpp"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>

namespace tpl {

class WorkerManager;

typedef std::unique_lock<std::mutex> LockType;

#if __cplusplus >= 201402L && __cplusplus < 202002L

class Worker {
  //   friend class WorkerManager;

public:
  Worker() = delete;
  Worker(const Worker &) = delete;
  Worker(Worker &&) noexcept = default;
  Worker &operator=(Worker &&) = default;
  Worker &operator=(const Worker &) noexcept = delete;

  //   explicit Worker(const std::mutex &sharedMutex,
  //                   const std::condition_variable &sharedCondition)
  //       : mSharedMutex{sharedMutex}, mSharedCondition{sharedCondition} {}

  explicit Worker(std::function<void(Worker &)> workerLoop)
      : mThread{workerLoop, std::ref(*this)} {}

  ~Worker() noexcept { this->terminate(); }

  //   bool retired() const noexcept { return this->mShouldTerminate; }

  //   void resign() noexcept { this->mShouldTerminate = true; }

  void terminate() noexcept {
    this->shouldTerminate = true;
    if (this->mThread.joinable()) {
      this->mThread.join();
    }
  }

public:
  bool shouldTerminate = false;

private:
  std::thread mThread;
};

#else // C++20 or later

class Worker {};

#endif // C++14 or C++17

} // namespace tpl

#endif // TPL_SOURCE_WORKER_HPP