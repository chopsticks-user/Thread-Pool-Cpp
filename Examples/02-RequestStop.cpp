#include <TPL/TPL.hpp>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  //

  tpl::WorkerManager wManager(4);
  tpl::ContractManager cManager;

  auto start = std::chrono::high_resolution_clock::now();

  std::cout << "Time elapsed: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n";

  return 0;
}