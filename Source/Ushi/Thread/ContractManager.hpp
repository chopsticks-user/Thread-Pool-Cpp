#ifndef USHI_SOURCE_THREAD_CONTRACT_MANAGER_HPP
#define USHI_SOURCE_THREAD_CONTRACT_MANAGER_HPP

#include "Base.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"

#include <queue>

namespace ushi {

class ContractManager {
public:
  ContractManager() = default;
  ContractManager(const ContractManager &) = delete;
  ContractManager(ContractManager &&) noexcept = default;
  ContractManager &operator=(const ContractManager &) = delete;
  ContractManager &operator=(ContractManager &&) noexcept = default;
  ~ContractManager() noexcept { this->clear(); }

  u64 nContracts() const noexcept { return this->mContractQueue.size(); }

  void addContract(JobType job) noexcept;

  JobContractPtr getContract() noexcept;

  void clear() noexcept;

private:
  std::mutex mMutex = {};
  std::condition_variable mCondition = {};
  std::queue<JobContract> mContractQueue = {};
};

} // namespace ushi

#endif // USHI_SOURCE_THREAD_CONTRACT_MANAGER_HPP