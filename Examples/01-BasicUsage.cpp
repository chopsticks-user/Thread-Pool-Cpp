#include <TPL/TPL.hpp>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  //

  tpl::ThreadPool pool(4);

  auto start = std::chrono::high_resolution_clock::now();

  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 1: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 2: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 3: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 4: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 5: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 6: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 7: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 8: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 9: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 10: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 11: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 12: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 13: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 14: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 15: " << std::this_thread::get_id() << std::endl;
  });
  pool.submitJob([&](tpl::ContractStatus &) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Job 16: " << std::this_thread::get_id() << std::endl;
  });

  pool.clear();

  std::cout << sizeof(pool) << std::endl;
  std::cout << tpl::ThreadPool::nConcurrentThreads() << std::endl;

  std::cout << "Time elapsed: "
            << std::chrono::duration<double, std::milli>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms\n";

  return 0;
}