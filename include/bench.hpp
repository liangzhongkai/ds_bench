#pragma once

#include <chrono>

#include "demangle.hpp"
#include "graphs.hpp"

// chrono typedefs
using std::chrono::microseconds;
using std::chrono::milliseconds;

using Clock = std::chrono::steady_clock;

// Number of repetitions of each test
static constexpr std::size_t REPEAT_TEST_NUM = 7;

template <class Container> inline static void run(Container &, std::size_t) {}

template <template <class> class Test, template <class> class... Rest,
          class Container>
inline static void run(Container &container, std::size_t size) {
  Test<Container>::run(container, size);
  run<Rest...>(container, size);
}

// benchmarking procedure
template <typename Container, typename DurationUnit,
          template <class> class CreatePolicy,
          template <class> class... TestPolicy>
void bench(const std::string &type, const std::initializer_list<int> &sizes) {
  for (auto size : sizes) {
    std::size_t duration = 0;

    for (std::size_t i = 0; i < REPEAT_TEST_NUM; ++i) {
      auto container = CreatePolicy<Container>::make(size);

      Clock::time_point t0 = Clock::now();

      run<TestPolicy...>(container, size);

      Clock::time_point t1 = Clock::now();
      duration += std::chrono::duration_cast<DurationUnit>(t1 - t0).count();
    }

    graphs::new_result(type, std::to_string(size), duration / REPEAT_TEST_NUM);
  }

  CreatePolicy<Container>::clean();
}

template <template <class> class Benchmark> void bench_types() {
  // Recursion end
}

template <template <class> class Benchmark, typename T, typename... Types>
void bench_types() {
  Benchmark<T>::run();
  bench_types<Benchmark, Types...>();
}

bool is_tag(int c) { return std::isalnum(c) || c == '_'; }

std::string tag(std::string name) {
  std::replace_if(
      begin(name), end(name), [](char c) { return !is_tag(c); }, '_');
  std::string res;
  res.swap(name);
  return res;
}

template <typename T>
void new_graph(const std::string &testName, const std::string &unit) {
  std::string title(testName + " - " + demangle<T>());
  graphs::new_graph(tag(title), title, unit);
}
