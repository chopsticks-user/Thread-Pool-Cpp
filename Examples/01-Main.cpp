#include <TPL/TPL.hpp>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  //

  tpl::WorkerManager wManager(6);
  tpl::ContractManager cManager;
  std::cout << "Main thread: " << std::this_thread::get_id() << '\n';

  auto start = std::chrono::high_resolution_clock::now();

  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 1: " << std::this_thread::get_id() << std::endl;
  });
  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 2: " << std::this_thread::get_id() << std::endl;
  });
  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 3: " << std::this_thread::get_id() << std::endl;
  });
  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 4: " << std::this_thread::get_id() << std::endl;
  });
  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 5: " << std::this_thread::get_id() << std::endl;
  });
  cManager.addContract([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Job 6: " << std::this_thread::get_id() << std::endl;
  });

  auto s1 = wManager.assignJob(cManager.getContract());
  // wManager.assignJob(cManager.getContract());
  wManager.assignJob(cManager.getContract());
  wManager.assignJob(cManager.getContract());
  wManager.assignJob(cManager.getContract());
  wManager.assignJob(cManager.getContract());
  wManager.assignJob(cManager.getContract());

  s1->wait();

  std::cout << "nWorkers = " << wManager.nActiveWorkers() << '\n';

  wManager.clear();

  std::cout << "Time elapsed: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n";

  return 0;
}