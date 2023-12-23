#ifndef TPL_SOURCE_THREAD_POOL_HPP
#define TPL_SOURCE_THREAD_POOL_HPP

#include "Base.hpp"
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

// class ThreadPool {
//   typedef std::function<void()> JobType;
//   typedef std::unique_lock<std::mutex> MutexLock_T;

// public:
//   ThreadPool() {
//     auto threadCount = std::thread::hardware_concurrency();
//     if (threadCount < 4) {
//       throw std::runtime_error("std::thread::hardware_concurrency() < 4");
//     }
//     mShouldTerminate = false;
//     mThreads.resize(threadCount);
//     for (auto &thread : mThreads) {
//       thread = std::thread{ThreadPool::threadLoop, this};
//     }
//   }

//   ThreadPool(const ThreadPool &) = delete;

//   ThreadPool(ThreadPool &&) = delete;

//   ThreadPool &operator=(const ThreadPool &) = delete;

//   ThreadPool &operator=(ThreadPool &&) = delete;

//   ~ThreadPool() { release(); }

//   size_t threadCount() const noexcept { return mThreads.size(); }

//   /**
//    * @brief
//    * ! The caller must ensure that {job} is alive until the worker thread
//    * ! finishes using it.
//    *
//    * @param job
//    * @return std::shared_ptr<JobPromise>
//    */
//   std::shared_ptr<JobContract> submitJob(const JobType &job);

//   bool busy();

//   void forceRelease();

//   void release();

// private:
//   std::vector<std::thread> mThreads;
//   std::queue<JobType> mJobs;
//   std::mutex mQueueMutex;
//   std::condition_variable mMutexCondition;
//   bool mShouldTerminate;

//   static void threadLoop(ThreadPool *pool);
// };

// std::shared_ptr<JobContract> ThreadPool::submitJob(const JobType &job) {
//   std::shared_ptr<JobContract> jobReady;
//   {
//     MutexLock_T lock{mQueueMutex};
//     jobReady = std::make_shared<JobContract>();
//     mJobs.push([=] {
//       job();
//       jobReady->setFlag();
//     });
//   }
//   mMutexCondition.notify_one();
//   return jobReady;
// }

// bool ThreadPool::busy() {
//   bool poolbusy;
//   {
//     MutexLock_T lock{mQueueMutex};
//     poolbusy = mJobs.empty();
//   }
//   return poolbusy;
// }

// void ThreadPool::forceRelease() {
//   {
//     MutexLock_T lock{mQueueMutex};
//     mShouldTerminate = true;
//   }
//   mMutexCondition.notify_all();
//   for (auto &activeThread : mThreads) {
//     activeThread.join();
//   }
//   mThreads.clear();
// }

// void ThreadPool::release() {
//   while (!busy()) {
//   }
//   forceRelease();
// }

// void ThreadPool::threadLoop(ThreadPool *pool) {
//   while (true) {
//     JobType job;
//     {
//       MutexLock_T lock{pool->mQueueMutex};
//       pool->mMutexCondition.wait(lock, [pool] {
//         return !pool->mJobs.empty() || pool->mShouldTerminate;
//       });
//       if (pool->mShouldTerminate) {
//         return;
//       }
//       job = pool->mJobs.front();
//       pool->mJobs.pop();
//     }
//     job();
//   }
// }

#else // C++20 or later

#endif // C++14 or later

} // namespace tpl

#endif // TPL_SOURCE_THREAD_POOL_HPP