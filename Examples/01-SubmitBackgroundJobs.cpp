#include <Ushi/Thread/ThreadPool.hpp>

#include <cstdio>
#include <iostream>
#include <sstream>

ushi::u64 currentThreadIDU64() {
  // https://stackoverflow.com/questions/7432100/how-to-get-integer-thread-id-in-c11
  std::stringstream ss;
  ss << std::this_thread::get_id();
  return std::stoull(ss.str());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  ushi::ThreadPool pool(4);

  std::cout << "Main thread's ID: " << std::this_thread::get_id() << '\n';
  std::cout << "sizeof(ushi::ThreadPool) = " << sizeof(pool) << " bytes\n";
  std::cout << "ThreadPool has " << pool.nThreads() << " workers" << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 1 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 2 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 3 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 4 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 5 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 6 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 7 finished by worker %lu\n", currentThreadIDU64());
  });
  pool.submitJob([](ushi::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Job 8 finished by worker %lu\n", currentThreadIDU64());
  });

  pool.clear();

  std::cout << "Time elapsed: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n";

  return 0;
}