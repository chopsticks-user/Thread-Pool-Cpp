#ifndef USHI_INCLUDE_USHI_WORKER_MANAGER_HPP
#define USHI_INCLUDE_USHI_WORKER_MANAGER_HPP

#if __cplusplus >= 201402L
#include "Base.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"

#include <queue>
#include <vector>

namespace ushi {

class WorkerManager {
public:
  WorkerManager() = delete;
  WorkerManager(const WorkerManager &) = delete;
  WorkerManager(WorkerManager &&) noexcept = default;
  WorkerManager &operator=(const WorkerManager &) = delete;
  WorkerManager &operator=(WorkerManager &&) noexcept = default;
  ~WorkerManager() noexcept { this->clear(); }

  explicit WorkerManager(u64 nWorkers) {
    // Use std::lock_guard to reduce overhead
    // https://codereview.stackexchange.com/questions/275834/tiny-thread-pool-implementation
    ScopedLockType lock{this->mMutex};

    mWorkers.reserve(nWorkers);
    while (nWorkers > 0) {
      mWorkers.emplace_back(Worker{[&]() { this->workerLoop(); }});
      nWorkers--;
    }
    this->mNAvailableWorkers.store(this->mWorkers.size());
  }

  u64 nWorkers() const noexcept { return this->mWorkers.size(); }

  u64 nAvailableWorkers() const noexcept { return this->mNAvailableWorkers; }

  ContractStatusPtr assignJob(JobContractPtr pContract) noexcept {
    ScopedLockType lock{this->mMutex};

    if (pContract == nullptr) {
      return {};
    }

    this->mContractQueue.push(pContract);
    mCondition.notify_one();
    return pContract->getStatusObserver();
  }

  void clear() noexcept {
    {
      ScopedLockType lock{this->mMutex};
      this->mShouldTerminate.store(true);
    }

    this->mCondition.notify_all();
    this->mWorkers.clear();
    this->mNAvailableWorkers.store(0);
  }

private:
  void workerLoop() {
    while (true) {
      JobContractPtr contract;
      {
        UniqueLockType lock{this->mMutex};

        this->mCondition.wait(lock, [this] {
          return !this->mContractQueue.empty() || this->mShouldTerminate.load();
        });

        // https://codereview.stackexchange.com/questions/275834/tiny-thread-pool-implementation
        if (this->mShouldTerminate.load() && this->mContractQueue.empty()) {
          --this->mNAvailableWorkers;
          return;
        }

        contract = this->mContractQueue.front();
        this->mContractQueue.pop();
        this->mPendingContractQueue.push(contract);
        --this->mNAvailableWorkers;
      }

      contract->start();

      {
        ScopedLockType lock{this->mMutex};
        contract->signalFinished();
        this->mPendingContractQueue.pop();
        ++this->mNAvailableWorkers;
      }
    }
  }

private:
  std::mutex mMutex = {};
  std::condition_variable mCondition = {};
  std::queue<JobContractPtr> mContractQueue = {};
  std::queue<JobContractPtr> mPendingContractQueue = {};
  std::vector<Worker> mWorkers = {};
  std::atomic<u64> mNAvailableWorkers{0};
  std::atomic<bool> mShouldTerminate{false};
};

} // namespace ushi
#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Ushi library requires C++14 or newer");
#endif // C++14 or later

#endif // USHI_INCLUDE_USHI_WORKER_MANAGER_HPP