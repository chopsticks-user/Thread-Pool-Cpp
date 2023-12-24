#ifndef TPL_SOURCE_WORKER_HPP
#define TPL_SOURCE_WORKER_HPP

#include "Base.hpp"
#include "JobContract.hpp"

namespace tpl {

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

} // namespace tpl

#endif // TPL_SOURCE_WORKER_HPP