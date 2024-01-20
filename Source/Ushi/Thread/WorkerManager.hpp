#ifndef USHI_SOURCE_THREAD_WORKER_MANAGER_HPP
#define USHI_SOURCE_THREAD_WORKER_MANAGER_HPP

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

  explicit WorkerManager(u64 nWorkers);

  u64 nWorkers() const noexcept { return this->mWorkers.size(); }

  u64 nAvailableWorkers() const noexcept { return this->mNAvailableWorkers; }

  ContractStatusPtr assignJob(JobContractPtr pContract) noexcept;

  void clear() noexcept;

private:
  void workerLoop();

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

#endif // USHI_SOURCE_THREAD_WORKER_MANAGER_HPP