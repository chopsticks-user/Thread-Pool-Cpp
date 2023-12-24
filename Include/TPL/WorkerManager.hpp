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
      mWorkers.emplace_back(
          Worker{[&](Worker &worker) { this->workerLoop(worker); }});
      nWorkers--;
    }
    this->mNActiveWorkers = this->mWorkers.size();
    this->mNAvailableWorkers.store(this->mNActiveWorkers);
  }

  u64 nActiveWorkers() const { return this->mNActiveWorkers; }

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
    { auto lock = LockType{this->mMutex}; }

    // for (auto &worker : this->mWorkers) {
    //   worker.mShouldTerminate = true;
    // }

    this->mShouldTerminate.store(true);
    this->mCondition.notify_all();
    this->mWorkers.clear();
    this->mNActiveWorkers.store(0);
    this->mNAvailableWorkers.store(0);
  }

private:
  void workerLoop(Worker &worker) {
    while (true) {
      std::shared_ptr<JobContract> contract;
      {
        auto lock = LockType{this->mMutex};

        this->mCondition.wait(lock, [&] {
          return !this->mContractQueue.empty() || this->mShouldTerminate.load();
        });

        // if (!this->mContractQueue.empty()) {
        //   contract = this->mContractQueue.front();
        //   this->mContractQueue.pop();
        //   --this->mNAvailableWorkers;
        // } else if (worker.mShouldTerminate) {
        //   --this->mNActiveWorkers;
        //   --this->mNAvailableWorkers;
        //   std::cout << "terminating" << std::endl;
        //   return;
        // }

        if (this->mShouldTerminate.load()) {
          --this->mNActiveWorkers;
          --this->mNAvailableWorkers;
          std::cout << "terminating" << std::endl;
          return;
        }

        contract = this->mContractQueue.front();
        this->mContractQueue.pop();
        this->mPendingContractQueue.push(contract);
        --this->mNAvailableWorkers;
        std::cout << "received" << std::endl;
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
  std::atomic<u64> mNActiveWorkers{0};
  std::atomic<u64> mNAvailableWorkers{0};
  std::atomic<bool> mShouldTerminate{false};
};

} // namespace tpl

#endif // TPL_SOURCE_WORKER_MANAGER_HPP