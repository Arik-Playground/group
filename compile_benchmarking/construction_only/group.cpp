#include <cstddef>
#include <utility>

#include "../../include/nass/group.hpp"

template <size_t... Idxs>
auto create_giant_group(std::index_sequence<Idxs...>)
{
    return nass::make_value_group(Idxs...);
}

#ifndef NASSGROUP_BENCHMARK_GIANT_NUMBER
#define NASSGROUP_BENCHMARK_GIANT_NUMBER 300
#endif // NASSGROUP_BENCHMARK_GIANT_NUMBER

int main()
{
    volatile auto testee = create_giant_group(std::make_index_sequence<NASSGROUP_BENCHMARK_GIANT_NUMBER>());
}