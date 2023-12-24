#ifndef TPL_SOURCE_CONTRACT_MANAGER_HPP
#define TPL_SOURCE_CONTRACT_MANAGER_HPP

#include "Base.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"

#include <queue>

namespace tpl {

typedef std::unique_lock<std::mutex> UniqueLockType;

class ContractManager {
public:
  ContractManager() = default;
  ContractManager(const ContractManager &) = delete;
  ContractManager(ContractManager &&) noexcept = default;
  ContractManager &operator=(const ContractManager &) = delete;
  ContractManager &operator=(ContractManager &&) noexcept = default;
  ~ContractManager() noexcept { this->clear(); }

  u64 nContracts() const noexcept { return this->mContractQueue.size(); }

  void addContract(JobType job) noexcept {
    UniqueLockType lock{this->mMutex};
    this->mContractQueue.push(
        JobContract{std::move(job), std::make_shared<ContractStatus>()});
  }

  std::shared_ptr<JobContract> getContract() noexcept {
    UniqueLockType lock{this->mMutex};

    if (this->mContractQueue.empty()) {
      return {};
    }

    JobContract contract = std::move(this->mContractQueue.front());
    this->mContractQueue.pop();
    return std::make_shared<JobContract>(std::move(contract));
  }

  void clear() noexcept {
    UniqueLockType lock{this->mMutex};
    while (!this->mContractQueue.empty()) {
      this->mContractQueue.pop();
    }
  }

private:
  std::mutex mMutex = {};
  std::condition_variable mCondition = {};
  std::queue<JobContract> mContractQueue = {};
  //   Worker mContractWorker;
};

} // namespace tpl

#endif // TPL_SOURCE_CONTRACT_MANAGER_HPP