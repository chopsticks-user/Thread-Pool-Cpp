#ifndef TPL_SOURCE_THREAD_POOL_HPP
#define TPL_SOURCE_THREAD_POOL_HPP

#include "Base.hpp"
#include "ContractManager.hpp"
#include "JobContract.hpp"
#include "Worker.hpp"
#include "WorkerManager.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

namespace tpl {

#if __cplusplus >= 201402L && __cplusplus < 202002L

class ThreadPool;

class JobPromise {
public:
  friend class ThreadPool;

  bool wait() {
    if (!mFinished) {
      mFinished = mFuture.get();
    }
    return mFinished;
  }

private:
  std::promise<bool> mPromise;
  std::future<bool> mFuture = mPromise.get_future();
  bool mFinished = false;

  void setFlag() { mPromise.set_value(true); }
};

class ThreadPool {
  typedef std::function<void()> Job_T;
  typedef std::unique_lock<std::mutex> MutexLock_T;

public:
  ThreadPool() {
    auto supportedThreadCount = std::thread::hardware_concurrency();
    if (supportedThreadCount < 4) {
      throw std::runtime_error("std::thread::hardware_concurrency() < 4");
    }
    mShouldTerminate = false;
    mThreads.resize(supportedThreadCount);
    for (auto &thread : mThreads) {
      thread = std::thread{ThreadPool::threadLoop, this};
    }
  }

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;
  ~ThreadPool() { release(); }

  size_t threadCount() const noexcept { return mThreads.size(); }

  std::shared_ptr<JobPromise> submitJob(Job_T job);

  bool busy();

  void force_release();

  void release();

private:
  std::vector<std::thread> mThreads = {};
  std::queue<Job_T> mJobs = {};
  std::mutex mQueueMutex = {};
  std::condition_variable mMutexCondition = {};
  bool mShouldTerminate = {};

  static void threadLoop(ThreadPool *pool);
};

std::shared_ptr<JobPromise> ThreadPool::submitJob(Job_T job) {
  std::shared_ptr<JobPromise> jobReady;
  {
    MutexLock_T lock{this->mQueueMutex};
    jobReady = std::make_shared<JobPromise>();
    mJobs.push([=] {
      job();
      jobReady->setFlag();
    });
  }
  mMutexCondition.notify_one();
  return jobReady;
}

bool ThreadPool::busy() {
  bool poolbusy;
  {
    MutexLock_T lock{this->mQueueMutex};
    poolbusy = mJobs.empty();
  }
  return poolbusy;
}

void ThreadPool::force_release() {
  {
    MutexLock_T lock{this->mQueueMutex};
    mShouldTerminate = true;
  }
  mMutexCondition.notify_all();
  for (auto &activeThread : mThreads) {
    activeThread.join();
  }
  mThreads.clear();
}

void ThreadPool::release() {
  while (!busy()) {
  }
  force_release();
}

void ThreadPool::threadLoop(ThreadPool *pool) {
  while (true) {
    Job_T job;
    {
      MutexLock_T lock{pool->mQueueMutex};
      pool->mMutexCondition.wait(lock, [pool] {
        return !pool->mJobs.empty() || pool->mShouldTerminate;
      });
      if (pool->mShouldTerminate) {
        return;
      }
      job = pool->mJobs.front();
      pool->mJobs.pop();
    }
    job();
  }
}

#else // C++20 or later

#endif // C++14 or later

} // namespace tpl

#endif // TPL_SOURCE_THREAD_POOL_HPP