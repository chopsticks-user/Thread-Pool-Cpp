#ifndef TPL_INCLUDE_TPL_JOB_CONTRACT_HPP
#define TPL_INCLUDE_TPL_JOB_CONTRACT_HPP

#include "Base.hpp"

#include <future>

namespace tpl {

class ContractManager;

class JobStatus {
  friend class ContractManager;

public:
  ~JobStatus() = default;
  JobStatus(const JobStatus &) = delete;
  JobStatus(JobStatus &&) = default;
  JobStatus &operator=(JobStatus &&) = default;
  JobStatus &operator=(const JobStatus &) = delete;

  bool shouldStopped() const noexcept { return this->mShouldStopped; }

  bool shouldTransferred() const noexcept { return this->mShouldTransferred; }

  bool isFinished() { return this->mFuture.get(); }

private:
  JobStatus() = default;

  void setFlag() { mPromise.set_value(true); }

private:
  bool mShouldStopped = false;
  bool mShouldTransferred = false;
  std::promise<bool> mPromise;
  std::future<bool> mFuture = mPromise.get_future();
  //   bool mFinished = false;
};

class JobContract;

typedef std::function<void(JobStatus &)> JobType;

class JobContract final {
  friend class ContractManager;

public:
  JobContract() = delete;
  ~JobContract() = default;
  JobContract(const JobContract &) = delete;
  JobContract(JobContract &&) = default;
  JobContract &operator=(JobContract &&) = default;
  JobContract &operator=(const JobContract &) = delete;

  void start() { this->mJob(this->mJobStatus); }

  //     bool wait() {
  //     if (!mFinished) {
  //       mFinished = mFuture.get();
  //     }
  //     return mFinished;
  //   }

private:
  JobContract(JobType job, JobStatus jobStatus)
      : mJob{std::move(job)}, mJobStatus{std::move(jobStatus)} {}

private:
  JobType mJob;
  JobStatus mJobStatus;
};

} // namespace tpl

#endif // TPL_INCLUDE_TPL_JOB_CONTRACT_HPP