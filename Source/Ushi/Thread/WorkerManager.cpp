#include "WorkerManager.hpp"

namespace ushi {

WorkerManager::WorkerManager(u64 nWorkers) {
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

ContractStatusPtr WorkerManager::assignJob(JobContractPtr pContract) noexcept {
  ScopedLockType lock{this->mMutex};

  if (pContract == nullptr) {
    return {};
  }

  this->mContractQueue.push(pContract);
  mCondition.notify_one();
  return pContract->getStatusObserver();
}

void WorkerManager::clear() noexcept {
  {
    ScopedLockType lock{this->mMutex};
    this->mShouldTerminate.store(true);
  }

  this->mCondition.notify_all();
  this->mWorkers.clear();
  this->mNAvailableWorkers.store(0);
}

void WorkerManager::workerLoop() {
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

} // namespace ushi