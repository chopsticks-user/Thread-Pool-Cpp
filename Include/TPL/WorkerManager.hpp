#ifndef TPL_SOURCE_WORKER_MANAGER_HPP
#define TPL_SOURCE_WORKER_MANAGER_HPP

#include "Base.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"

#include <iostream>
#include <queue>
#include <vector>

namespace tpl {

typedef std::unique_lock<std::mutex> LockType;

class WorkerManager {
public:
  WorkerManager() = delete;
  WorkerManager(const WorkerManager &) = delete;
  WorkerManager(WorkerManager &&) noexcept = default;
  WorkerManager &operator=(const WorkerManager &) = delete;
  WorkerManager &operator=(WorkerManager &&) noexcept = default;
  ~WorkerManager() noexcept { this->clear(); }

  explicit WorkerManager(u64 nWorkers) {
    LockType lock{this->mMutex};

    mWorkers.reserve(nWorkers);
    while (nWorkers > 0) {
      mWorkers.emplace_back(Worker{[&]() { this->workerLoop(); }});
      nWorkers--;
    }
    this->mNAvailableWorkers.store(this->mWorkers.size());
  }

  u64 nWorkers() const { return this->mWorkers.size(); }

  u64 nAvailableWorkers() const { return this->mNAvailableWorkers; }

  ContractStatusPtr assignJob(std::shared_ptr<JobContract> pContract) {
    LockType lock{this->mMutex};

    if (pContract == nullptr) {
      return {};
    }

    this->mContractQueue.push(pContract);
    mCondition.notify_one();
    return pContract->getStatusObserver();
  }

  void clear() noexcept {
    {
      auto lock = LockType{this->mMutex};
      this->mShouldTerminate.store(true);
    }

    // for (auto &worker : this->mWorkers) {
    //   worker.mShouldTerminate = true;
    // }

    this->mCondition.notify_all();
    this->mWorkers.clear();
    this->mNAvailableWorkers.store(0);
  }

private:
  void workerLoop() {
    while (true) {
      std::shared_ptr<JobContract> contract;
      {
        auto lock = LockType{this->mMutex};

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
        auto lock = LockType{this->mMutex};
        contract->signalFinished();
        this->mPendingContractQueue.pop();
        ++this->mNAvailableWorkers;
      }
    }
  }

private:
  std::mutex mMutex = {};
  std::condition_variable mCondition = {};
  std::queue<std::shared_ptr<JobContract>> mContractQueue = {};
  std::queue<std::shared_ptr<JobContract>> mPendingContractQueue = {};
  std::vector<Worker> mWorkers = {};
  std::atomic<u64> mNAvailableWorkers{0};
  std::atomic<bool> mShouldTerminate{false};
};

} // namespace tpl

#endif // TPL_SOURCE_WORKER_MANAGER_HPP