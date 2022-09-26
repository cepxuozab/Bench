#include <benchmark/benchmark.h>

#include <ranges>
#include <vector>
#include <iostream>
#include <string>

using namespace std::string_literals;

void ConstChar(benchmark::State& state) {
	std::string created_string;
	std::string const added = "hello"s;
	for (auto _ : state) {
		created_string += added;
		// Make sure the variable is not optimized away by compiler
		benchmark::DoNotOptimize(created_string);
	}
}
void String(benchmark::State& state) {
	std::string x;
	const char* added{ "hello" };
	for (auto _ : state) {
		x += added;
		benchmark::DoNotOptimize(x);
	}
}

using namespace std::ranges;

namespace {

template <typename T>
auto make_sorted_vector(std::size_t size) {
  auto sorted = std::vector<T>{};
  sorted.reserve(size);

  auto sorted_view = views::iota(T{0}) | views::take(size);
  std::ranges::copy(sorted_view, std::back_inserter(sorted));
  return sorted;
}

constexpr auto MAX_HAYSTACK_SIZE = std::size_t{10'000'000};
constexpr auto NEEDLE = 2137;

void binary_search_in_sorted_vector(benchmark::State &state) {
  auto haystack = make_sorted_vector<int>(MAX_HAYSTACK_SIZE);
  for (auto _ : state) {
    benchmark::DoNotOptimize(lower_bound(haystack, NEEDLE));
  }
}

void linear_search_in_sorted_vector(benchmark::State &state) {
  auto haystack = make_sorted_vector<int>(MAX_HAYSTACK_SIZE);
  for (auto _ : state) {
    benchmark::DoNotOptimize(find(haystack, NEEDLE));
  }
}

}  // namespace
BENCHMARK(ConstChar);
BENCHMARK(String);


BENCHMARK_MAIN();