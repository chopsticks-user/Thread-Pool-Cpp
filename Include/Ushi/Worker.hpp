#ifndef USHI_INCLUDE_USHI_WORKER_HPP
#define USHI_INCLUDE_USHI_WORKER_HPP

#if __cplusplus >= 201402L
#include "Base.hpp"
#include "JobContract.hpp"

namespace ushi {

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
  explicit Worker(WorkerFuncType workerLoop) : mThread{workerLoop} {}

  void terminate() noexcept {
    if (this->mThread.joinable()) {
      this->mThread.join();
    }
  }

private:
  std::thread mThread;
};

} // namespace ushi
#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Ushi library requires C++14 or newer");
#endif // C++14 or later

#endif // USHI_INCLUDE_USHI_WORKER_HPP