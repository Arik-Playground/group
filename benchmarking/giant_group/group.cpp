#include <cstddef>
#include <utility>

#include "../../include/nassgroup/group.hpp"

template <size_t... Idxs>
auto create_giant_group(std::index_sequence<Idxs...>)
{
    return nass::make_value_group(Idxs...);
}

#ifndef NASSGROUP_BENCHMARK_GIANT_NUMBER
#define NASSGROUP_BENCHMARK_GIANT_NUMBER 1000
#endif // NASSGROUP_BENCHMARK_GIANT_NUMBER

extern const auto testee = create_giant_group(std::make_index_sequence<NASSGROUP_BENCHMARK_GIANT_NUMBER>());