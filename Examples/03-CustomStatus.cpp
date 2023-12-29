#include <Ushi/Ushi.hpp>

#include <cstdio>
#include <iostream>

//! Feature is not yet implemented

struct CustomStatus : public ushi::ContractStatus {
  // add a custom method
  void onFinished() const { printf("Finished!\n"); }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  ushi::ThreadPool pool(4);

  auto start = std::chrono::high_resolution_clock::now();

  // pool.submitJob([](ushi::ContractStatus *pStatusBase) {
  //   CustomStatus* pStatus = dynamic_cast
  //   std::this_thread::sleep_for(std::chrono::seconds(1));
  //   status->onFinished();
  // });

  std::cout << "Time elapsed: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n";

  return 0;
}