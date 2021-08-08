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

// extern const auto testee = create_giant_tuple(std::make_index_sequence<NASSGROUP_BENCHMARK_GIANT_NUMBER>());

struct value_t
{
    static size_t created;
    static size_t copied;
    static size_t moved;

    size_t value;

    value_t():
    value{ created++ + (copied << 16) + (moved << 32) }
    {}

    value_t(const value_t&):
    value{ created + ((copied++) << 16) + (moved << 32) }
    {}

    value_t(value_t&&):
    value{ created + (copied << 16) + ((moved++) << 32) }
    {}
};

size_t value_t::created = 0;
size_t value_t::copied = 0;
size_t value_t::moved = 0;

static value_t dummy{};
auto make()
{
    return std::tuple<value_t,value_t, value_t&, const value_t&, value_t&&>(dummy, std::move(dummy), dummy, dummy, std::move(dummy));
}