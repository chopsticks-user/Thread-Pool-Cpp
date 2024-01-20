#include "ThreadPool.hpp"

namespace ushi {

ContractStatusPtr ThreadPool::submitJob(JobType job) noexcept {
  ScopedLockType lock{this->mMutex};

  this->mContractManager->addContract(std::move(job));

  return this->mWorkerManager->assignJob(this->mContractManager->getContract());
}

// TODO: submitJob with a user-defined contract status
// template <typename CustomStatusType> //
// ContractStatusPtr
// submitJob(std::function<void(CustomStatusType &)> job) noexcept {
//   ScopedLockType lock{this->mMutex};

//   this->mContractManager->addContract([&job](ContractStatus &status) {
//     return job(static_cast<CustomStatusType &>(status));
//   });

//   return this->mWorkerManager->assignJob(
//       this->mContractManager->getContract());
// }

} // namespace ushi