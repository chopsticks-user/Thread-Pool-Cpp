#ifndef TPL_INCLUDE_TPL_JOB_CONTRACT_HPP
#define TPL_INCLUDE_TPL_JOB_CONTRACT_HPP

#if __cplusplus >= 201402L
#include "Base.hpp"

namespace ushi {

class ContractStatus {
  friend class JobContract;

public:
  ContractStatus() = default;
  ~ContractStatus() = default;
  ContractStatus(const ContractStatus &) = delete;
  ContractStatus(ContractStatus &&) = default;
  ContractStatus &operator=(ContractStatus &&) = delete;
  ContractStatus &operator=(const ContractStatus &) = default;

  void requestStop() noexcept { this->mShouldBeStopped = true; }

  bool stopRequested() const noexcept { return this->mShouldBeStopped; }

  // https://stackoverflow.com/questions/10890242/get-the-status-of-a-stdfuture
  bool finished() const noexcept {
    return this->mFuture.wait_for(std::chrono::microseconds(0)) ==
           std::future_status::ready;
  }

  bool wait() {
    if (this->finished()) {
      return true;
    }
    return mFuture.get();
  }

private:
  void signalFinished() { this->mPromise.set_value(true); }

private:
  bool mShouldBeStopped = false;
  std::promise<bool> mPromise;
  std::future<bool> mFuture = mPromise.get_future();
};

class JobContract {
  friend class ContractManager;

public:
  JobContract() = delete;
  ~JobContract() = default;
  JobContract(const JobContract &) = delete;
  JobContract(JobContract &&) = default;
  JobContract &operator=(JobContract &&) = default;
  JobContract &operator=(const JobContract &) = delete;

  void start() noexcept { this->mJob(*this->mpJobStatus); }

  void signalFinished() noexcept { this->mpJobStatus->signalFinished(); }

  ContractStatusPtr getStatusObserver() noexcept { return this->mpJobStatus; }

private:
  JobContract(JobType job, ContractStatusPtr jobStatus)
      : mJob{std::move(job)}, mpJobStatus{jobStatus} {}

private:
  JobType mJob;
  ContractStatusPtr mpJobStatus;
};

} // namespace ushi
#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Ushi library requires C++14 or newer");
#endif // C++14 or later

#endif // TPL_INCLUDE_TPL_JOB_CONTRACT_HPP