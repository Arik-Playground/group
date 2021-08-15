#ifndef NASSGROUP_BENCHMARK_TAO
#include <tuple>
#define NASSGROUP_TUPLE_NS std
#else 
#include <tao/tuple/tuple.hpp>
#define NASSGROUP_TUPLE_NS tao
#endif //NASSGROUP_BENCHMARK_TAO
#include <cstddef>
#include <utility>

template <size_t... Idxs>
auto create_giant_tuple(std::index_sequence<Idxs...>)
{
    return NASSGROUP_TUPLE_NS::tuple{Idxs...};
}

#ifndef NASSGROUP_BENCHMARK_GIANT_NUMBER
#define NASSGROUP_BENCHMARK_GIANT_NUMBER 300
#endif // NASSGROUP_BENCHMARK_GIANT_NUMBER

int main()
{
    volatile auto testee = create_giant_tuple(std::make_index_sequence<NASSGROUP_BENCHMARK_GIANT_NUMBER>());
}