#include <Ushi/Ushi.hpp>

#include <cstdio>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  ushi::ThreadPool pool(4);

  auto job = [](ushi::ContractStatus &status) {
    // Simulate heavy computations
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (status.stopRequested()) {
      return;
    }
    // Another heavy computations
    std::this_thread::sleep_for(std::chrono::seconds(1));
  };

  // Submit and make no stop requests
  auto start = std::chrono::high_resolution_clock::now();
  auto promise1 = pool.submitJob(job);
  promise1->wait();

  std::cout << "Submit job and don't make a stop request (run 2 seconds): "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n"
            << std::flush;

  // Submit and make a stop request
  start = std::chrono::high_resolution_clock::now();
  auto promise2 = pool.submitJob(job);
  promise2->requestStop();
  promise2->wait();

  std::cout << "Submit the same job and make a stop request after 1 second: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n"
            << std::flush;

  return 0;
}