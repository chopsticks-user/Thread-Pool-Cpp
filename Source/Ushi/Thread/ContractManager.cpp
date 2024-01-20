#include "ContractManager.hpp"

namespace ushi {

void ContractManager::addContract(JobType job) noexcept {
  UniqueLockType lock{this->mMutex};
  this->mContractQueue.push(
      JobContract{std::move(job), std::make_shared<ContractStatus>()});
}

JobContractPtr ContractManager::getContract() noexcept {
  UniqueLockType lock{this->mMutex};

  if (this->mContractQueue.empty()) {
    return {};
  }

  JobContract contract = std::move(this->mContractQueue.front());
  this->mContractQueue.pop();
  return std::make_shared<JobContract>(std::move(contract));
}

void ContractManager::clear() noexcept {
  UniqueLockType lock{this->mMutex};
  while (!this->mContractQueue.empty()) {
    this->mContractQueue.pop();
  }
}

} // namespace ushi