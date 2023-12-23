#include <TPL/TPL.hpp>

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  // tpl::ThreadPool pool;
  tpl::WorkerManager manager(6);
  manager.clear();
  return 0;
}