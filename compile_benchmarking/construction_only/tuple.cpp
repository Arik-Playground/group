#include <tuple>
#include <cstddef>
#include <utility>

template <size_t... Idxs>
auto create_giant_tuple(std::index_sequence<Idxs...>)
{
    return std::tuple{Idxs...};
}

int main()
{
    volatile auto testee = create_giant_tuple(std::make_index_sequence<300>());
}