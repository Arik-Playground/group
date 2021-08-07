#include <tuple>
#include <cstddef>
#include <utility>

template <size_t... Idxs>
auto create_giant_tuple(std::index_sequence<Idxs...>)
{
    return std::tuple{Idxs...};
}

#ifndef NASSGROUP_BENCHMARK_GIANT_NUMBER
#define NASSGROUP_BENCHMARK_GIANT_NUMBER 1000
#endif // NASSGROUP_BENCHMARK_GIANT_NUMBER

extern const auto testee = create_giant_tuple(std::make_index_sequence<NASSGROUP_BENCHMARK_GIANT_NUMBER>());