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

#if __cplusplus >= 201402L && __cplusplus < 202002L

class Worker {
  friend class WorkerManager;

  typedef std::unique_lock<std::mutex> LockType;

public:
  //   std::unique_ptr<JobType> getJob() { return nullptr; }

private:
  Worker(const std::mutex &sharedMutex,
         const std::condition_variable &sharedCondition)
      : mSharedMutex{sharedMutex}, mSharedCondition{sharedCondition} {}

  LockType createLock() {
    return LockType{const_cast<std::mutex &>(this->mSharedMutex)};
  }

  static void threadLoop(Worker &worker) {
    while (true) {
      std::unique_ptr<JobContract> contract = nullptr;
      {
        auto lock = worker.createLock();
        if (worker.mShouldTerminate) {
          worker.mThread.join();
        }
        if (worker.mpCurrentContract == nullptr) {
          continue;
        }
        contract = std::move(worker.mpCurrentContract);
        worker.mpCurrentContract = nullptr;
      }
      contract->start();
    }
  }

private:
  const std::mutex &mSharedMutex;
  const std::condition_variable &mSharedCondition;
  std::unique_ptr<JobContract> mpCurrentContract = nullptr;
  std::thread mThread = std::thread{Worker::threadLoop, std::ref(*this)};
  bool mShouldTerminate = false;
};

#else // C++20 or later

class Worker {};

#endif // C++14 or C++17

} // namespace tpl

#endif // TPL_SOURCE_WORKER_HPP