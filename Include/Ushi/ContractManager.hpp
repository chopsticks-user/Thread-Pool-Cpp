#ifndef USHI_INCLUDE_USHI_CONTRACT_MANAGER_HPP
#define USHI_INCLUDE_USHI_CONTRACT_MANAGER_HPP

#if __cplusplus >= 201402L
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

  void addContract(JobType job) noexcept {
    UniqueLockType lock{this->mMutex};
    this->mContractQueue.push(
        JobContract{std::move(job), std::make_shared<ContractStatus>()});
  }

  JobContractPtr getContract() noexcept {
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

} // namespace ushi
#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Ushi library requires C++14 or newer");
#endif // C++14 or later

#endif // USHI_INCLUDE_USHI_CONTRACT_MANAGER_HPP