#ifndef TPL_SOURCE_WORKER_MANAGER_HPP
#define TPL_SOURCE_WORKER_MANAGER_HPP

#include "Base.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"

#include <queue>
#include <vector>

namespace tpl {

class WorkerManager {
public:
  WorkerManager() = delete;
  WorkerManager(const WorkerManager &) = delete;
  WorkerManager(WorkerManager &&) noexcept = default;
  WorkerManager &operator=(const WorkerManager &) = delete;
  WorkerManager &operator=(WorkerManager &&) noexcept = default;
  ~WorkerManager() noexcept { this->clear(); }

  explicit WorkerManager(u64 nWorkers) {
    mWorkers.reserve(nWorkers);
    while (nWorkers > 0) {
      mWorkers.emplace_back(
          Worker{[&](Worker &worker) { this->workerLoop(worker); }});
      nWorkers--;
    }
    this->mNActiveWorkers = this->mWorkers.size();
    this->mNAvailableWorkers = this->mNActiveWorkers;
  }

  u64 nActiveWorkers() const { return this->mNActiveWorkers; }

  u64 nAvailableWorkers() const { return this->mNAvailableWorkers; }

  void assignJob(JobContract &&contract) {
    LockType lock{this->mMutex};
    this->mJobQueue.push(std::make_unique<JobContract>(std::move(contract)));
    mCondition.notify_one();
  }

  void clear() noexcept {
    {
      auto lock = LockType{this->mMutex};

      for (auto &worker : this->mWorkers) {
        worker.shouldTerminate = true;
      }
    }

    this->mCondition.notify_all();
    this->mWorkers.clear();
    this->mNActiveWorkers = 0;
    this->mNAvailableWorkers = 0;
  }

private:
  void workerLoop(Worker &worker) {
    while (true) {
      std::unique_ptr<JobContract> contract = nullptr;
      {
        auto lock = LockType{this->mMutex};

        this->mCondition.wait(lock, [&] {
          return !this->mJobQueue.empty() || worker.shouldTerminate;
        });

        if (worker.shouldTerminate) {
          --this->mNActiveWorkers;
          --this->mNAvailableWorkers;
          return;
        }

        contract = std::move(this->mJobQueue.front());
        this->mJobQueue.pop();
        --this->mNAvailableWorkers;
      }
      contract->start();
      //   {
      //     auto lock = LockType{this->mMutex};
      //     ++this->mNAvailableWorkers;
      //   }
    }
  }

private:
  std::mutex mMutex = {};
  std::condition_variable mCondition = {};
  std::queue<std::unique_ptr<JobContract>> mJobQueue = {};
  std::vector<Worker> mWorkers = {};
  u64 mNActiveWorkers = 0;
  u64 mNAvailableWorkers = 0;
};

} // namespace tpl

#endif // TPL_SOURCE_WORKER_MANAGER_HPP