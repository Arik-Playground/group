#include <tuple>
#include <utility>

#include "nass/group.hpp"

#include <benchmark/benchmark.h>

#ifndef BENCH_NO_INLINE

#ifdef __clang__
#define BENCH_NO_INLINE __attribute__ ((noinline))
#elif __GNUC__ 
#define BENCH_NO_INLINE __attribute__ ((noinline))
#endif //__GNUC__

#endif // BENCH_NO_INLINE

namespace group_utils
{
    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make(std::index_sequence<Idxs...>)
    {
        return nass::make_group<decltype(Idxs)...>(Idxs...);
    }

    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make_value(std::index_sequence<Idxs...>)
    {
        return nass::make_value_group(Idxs...);
    }

    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make_forward(std::index_sequence<Idxs...>)
    {
        return [](auto... vals)
        {        
            return nass::make_forward_group(vals...);
        }(Idxs...);
    }

    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make_from(std::index_sequence<Idxs...>)
    {
        return nass::make_group_from((Idxs, []{ return 5; })...);
    }
}

namespace tuple_utils
{
    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make(std::index_sequence<Idxs...>)
    {
        return std::tuple{Idxs...};
    }

    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make_value(std::index_sequence<Idxs...>)
    {
        return std::make_tuple(Idxs...);
    }

    template <size_t... Idxs>
    BENCH_NO_INLINE
    auto make_forward(std::index_sequence<Idxs...>)
    {
        return [](auto... vals)
        {        
            return std::forward_as_tuple(vals...);
        }(Idxs...);
    }
}

auto BM_make_group = [](auto& bstate) { for (auto b: bstate) group_utils::make(std::make_index_sequence<300>()); };
auto BM_make_value_group = [](auto& bstate) { for (auto b: bstate) group_utils::make_value(std::make_index_sequence<300>()); };
auto BM_make_forward_group = [](auto& bstate) { for (auto b: bstate) group_utils::make_forward(std::make_index_sequence<300>()); };
auto BM_make_group_from = [](auto& bstate) { for (auto b: bstate) group_utils::make_from(std::make_index_sequence<300>()); };
BENCHMARK(BM_make_group);
BENCHMARK(BM_make_value_group);
BENCHMARK(BM_make_forward_group);
BENCHMARK(BM_make_group_from);

auto BM_make_tuple = [](auto& bstate) { for (auto b: bstate) tuple_utils::make(std::make_index_sequence<300>()); };
auto BM_make_value_tuple = [](auto& bstate) { for (auto b: bstate) tuple_utils::make_value(std::make_index_sequence<300>()); };
auto BM_make_forward_tuple = [](auto& bstate) { for (auto b: bstate) tuple_utils::make_forward(std::make_index_sequence<300>()); };
BENCHMARK(BM_make_tuple);
BENCHMARK(BM_make_value_tuple);
BENCHMARK(BM_make_forward_tuple);