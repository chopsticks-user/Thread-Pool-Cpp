#ifndef USHI_INCLUDE_USHI_BASE_HPP
#define USHI_INCLUDE_USHI_BASE_HPP

#if __cplusplus >= 201402L
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <stdexcept>
#include <thread>

// #if __cplusplus >= 201703L
// #include <any>
// #include <optional>
// #endif // C++17 or later

// #if __cplusplus >= 202002L
// #include <barrier>
// #include <latch>
// #include <stop_token>
// #endif // C++20 or later

namespace ushi {

// https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
#ifdef USHI_BUILDING_DLL
#ifdef __GNUC__
#define USHI_PUBLIC __attribute__((dllexport))
#else
#define USHI_PUBLIC                                                            \
  __declspec(                                                                  \
      dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define USHI_PUBLIC __attribute__((dllimport))
#else
#define USHI_PUBLIC                                                            \
  __declspec(                                                                  \
      dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define USHI_PRIVATE
#else
#if __GNUC__ >= 4
#define USHI_PUBLIC __attribute__((visibility("default")))
#define USHI_PRIVATE __attribute__((visibility("hidden")))
#else
#define USHI_PUBLIC
#define USHI_PRIVATE
#endif
#endif

// Compile with fvisibility=hidden
#ifndef USHI_EXPORT
#define USHI_EXPORT USHI_PUBLIC
#endif

constexpr bool cppAtLeast14 = __cplusplus >= 201402L;
constexpr bool cppAtLeast17 = __cplusplus >= 201703L;
constexpr bool cppAtLeast20 = __cplusplus >= 202002L;
constexpr bool cpp14 = cppAtLeast14 && !cppAtLeast17;
constexpr bool cpp17 = cppAtLeast17 && !cppAtLeast20;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef bool b8;
typedef uint32_t b32;
typedef const char *cString;

class Worker;
class WorkerManager;
class ContractStatus;
class JobContract;
class ContractManager;
class USHI_EXPORT ThreadPool;

typedef std::unique_lock<std::mutex> UniqueLockType;
typedef std::lock_guard<std::mutex> ScopedLockType;
typedef std::function<void()> WorkerFuncType;
typedef std::function<void(ContractStatus &)> JobType;
typedef std::shared_ptr<ContractStatus> ContractStatusPtr;
typedef std::weak_ptr<ContractStatus> StatusObserverPtr;
typedef std::shared_ptr<JobContract> JobContractPtr;

} // namespace ushi
#else  // C++11 or older
static_assert(__cplusplus >= 201402L, "Ushi library requires C++14 or newer");
#endif // C++14 or later

#endif // USHI_INCLUDE_USHI_BASE_HPP