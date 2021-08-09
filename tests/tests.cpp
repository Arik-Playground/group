#include "nass/group.hpp"
#include <catch2/catch.hpp>

// used to test the address of rvalues
template<typename T> T& unmove(T&& t) { return t; }

TEST_CASE("Constexpr make_group", "[constexpr][make_group]")
{
    SECTION("empty")
    {
        constexpr auto g = nass::make_group();
        static_assert(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        static constexpr value_t value{};
        constexpr auto g = nass::make_group<value_t>(value);
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        static_assert(&g.get<0>() != &value);
    }

    SECTION("single ref")
    {
        struct value_t {};
        static constexpr value_t value{};
        constexpr auto g = nass::make_group<const value_t&>(value);
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        static_assert(&g.get<0>() == &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        static constexpr value_a_t value_a{ 4 };
        static constexpr value_b_t value_b{ 5 };
        constexpr auto g = nass::make_group<value_a_t, const value_b_t&>(value_a, value_b);
        static_assert(g.tuple_size == 2);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_a_t&>);
        static_assert(std::is_same_v<decltype(g.get<1>()), const value_b_t&>);
        static_assert(&g.get<0>() != &value_a);
        static_assert(&g.get<1>() == &value_b);
    }

    SECTION("conversion")
    {
        struct converted
        {
            constexpr converted(int) {}
        };

        struct convertible
        {
            constexpr operator converted() { return converted{0}; }
        };
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif // __GNUC__
        constexpr auto g = nass::make_group<converted, converted>(int(0), convertible{});
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__

    }
}

TEST_CASE("make_group", "[make_group]")
{
    SECTION("empty")
    {
        auto g = nass::make_group();
        REQUIRE(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group<value_t>(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
        REQUIRE(&g.get<0>() != &value);
    }

    SECTION("single ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group<value_t&>(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single const ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group<const value_t&>(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single tref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group<value_t&&>(std::move(value));
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&&>);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        struct value_c_t { int inner; };
        struct value_d_t { int inner; };
        value_a_t value_a{ 5 };
        value_b_t value_b{ 6 };
        value_c_t value_c{ 7 };
        const value_d_t value_d{ 8 };
        auto g = nass::make_group<value_a_t, value_b_t&, const value_c_t&, const value_d_t&>(value_a, value_b, value_c, value_d);
        REQUIRE(g.tuple_size == 4);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_a_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<1>()), value_b_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<2>()), const value_c_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<3>()), const value_d_t&>);
        REQUIRE(&g.get<0>() != &value_a);
        REQUIRE(&g.get<1>() == &value_b);
        REQUIRE(&g.get<2>() == &value_c);
        REQUIRE(&g.get<3>() == &value_d);
    }

    SECTION("conversion")
    {
        struct converted
        {
            converted(int) {}
        };

        struct convertible
        {
            operator converted() { return converted{0}; }
        };

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif // __GNUC__
        auto g = nass::make_group<converted, converted>(int(0), convertible{});
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__
    }
}

TEST_CASE("Constexpr make_value_group", "[constexpr][make_value_group]")
{
    SECTION("empty")
    {
        constexpr auto g = nass::make_value_group();
        static_assert(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        static constexpr value_t value{};
        constexpr auto g = nass::make_value_group(value);
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        static_assert(&g.get<0>() != &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        static constexpr value_a_t value_a{ 4 };
        static constexpr value_b_t value_b{ 5 };
        constexpr auto g = nass::make_value_group(value_a, value_b);
        static_assert(g.tuple_size == 2);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_a_t&>);
        static_assert(std::is_same_v<decltype(g.get<1>()), const value_b_t&>);
        static_assert(&g.get<0>() != &value_a);
        static_assert(&g.get<1>() != &value_b);
    }
}

TEST_CASE("make_value_group", "[make_value_group]")
{
    SECTION("empty")
    {
        auto g = nass::make_value_group();
        REQUIRE(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        auto g = nass::make_value_group(value_t{});
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
    }

    SECTION("single from ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_value_group(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
        REQUIRE(&g.get<0>() != &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        struct value_c_t { int inner; };
        value_a_t value_a{ 4 };
        value_b_t value_b{ 5 };
        const value_c_t value_c{ 6 };
        auto g = nass::make_value_group(value_a, value_b, value_c);
        REQUIRE(g.tuple_size == 3);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_a_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<1>()), value_b_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<2>()), value_c_t&>);
        REQUIRE(&g.get<0>() != &value_a);
        REQUIRE(&g.get<1>() != &value_b);
        REQUIRE(&g.get<2>() != &value_c);
    }
}

TEST_CASE("Constexpr make_forward_group", "[constexpr][make_forward_group]")
{
    SECTION("empty")
    {
        constexpr auto g = nass::make_forward_group();
        static_assert(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        static constexpr value_t value{};
        constexpr auto g = nass::make_forward_group(value);
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        static_assert(&g.get<0>() == &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        static constexpr value_a_t value_a{ 4 };
        static constexpr value_b_t value_b{ 5 };
        constexpr auto g = nass::make_forward_group(value_a, value_b);
        static_assert(g.tuple_size == 2);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_a_t&>);
        static_assert(std::is_same_v<decltype(g.get<1>()), const value_b_t&>);
        static_assert(&g.get<0>() == &value_a);
        static_assert(&g.get<1>() == &value_b);
    }
}

TEST_CASE("make_forward_group", "[make_forward_group]")
{
    SECTION("empty")
    {
        auto g = nass::make_forward_group();
        REQUIRE(g.tuple_size == 0);
    }

    SECTION("single ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_forward_group(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single const ref")
    {
        struct value_t {};
        const value_t value{};
        auto g = nass::make_forward_group(value);
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single tref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_forward_group(std::move(value));
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&&>);
        REQUIRE(&unmove(g.get<0>()) == &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        struct value_c_t { int inner; };
        value_a_t value_a{ 4 };
        const value_b_t value_b{ 5 };
        value_c_t value_c{ 6 };
        auto g = nass::make_forward_group(value_a, value_b, std::move(value_c));
        REQUIRE(g.tuple_size == 3);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_a_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<1>()), const value_b_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<2>()), value_c_t&&>);
        REQUIRE(&g.get<0>() == &value_a);
        REQUIRE(&g.get<1>() == &value_b);
        REQUIRE(&unmove(g.get<2>()) == &value_c);
    }
}

TEST_CASE("Constexpr make_group_from", "[constexpr][make_group_from]")
{
    SECTION("empty")
    {
        constexpr auto g = nass::make_group_from();
        static_assert(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        constexpr auto g = nass::make_group_from([]{ return value_t{}; });
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
    }

    SECTION("single ref")
    {
        struct value_t {};
        static constexpr value_t value{};
        constexpr auto g = nass::make_group_from([]() -> const value_t& { return value; });
        static_assert(g.tuple_size == 1);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        static_assert(&g.get<0>() == &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        static constexpr value_a_t value_a{ 4 };
        static constexpr value_b_t value_b{ 5 };
        constexpr auto g = nass::make_group_from
        (
            [] { return value_a; },
            []() -> const value_b_t& { return value_b; }
        );
        static_assert(g.tuple_size == 2);
        static_assert(std::is_same_v<decltype(g.get<0>()), const value_a_t&>);
        static_assert(std::is_same_v<decltype(g.get<1>()), const value_b_t&>);
        static_assert(&g.get<0>() != &value_a);
        static_assert(&g.get<1>() == &value_b);
    }

    SECTION("single init")
    {
        struct no_duplicates 
        {
            constexpr no_duplicates() = default;
            no_duplicates(const no_duplicates&) = delete;
            no_duplicates(no_duplicates&&) = delete;
        };
        constexpr auto g = nass::make_group_from([]{ return no_duplicates{}; });
        static_assert(std::is_same_v<decltype(g.get<0>()), const no_duplicates&>);
    }
}

TEST_CASE("make_group_from", "[make_group_from]")
{
    SECTION("empty")
    {
        auto g = nass::make_group_from();
        REQUIRE(g.tuple_size == 0);
    }

    SECTION("single")
    {
        struct value_t {};
        auto g = nass::make_group_from([]{ return value_t{}; });
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
    }

    SECTION("single ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group_from([&value]() -> value_t& { return value; });
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single const ref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group_from([&value]() -> const value_t& { return value; });
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), const value_t&>);
        REQUIRE(&g.get<0>() == &value);
    }

    SECTION("single tref")
    {
        struct value_t {};
        value_t value{};
        auto g = nass::make_group_from([&value]() -> value_t&& { return std::move(value); });
        REQUIRE(g.tuple_size == 1);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_t&&>);
        REQUIRE(&unmove(g.get<0>()) == &value);
    }

    SECTION("multiple")
    {
        struct value_a_t { int inner; };
        struct value_b_t { int inner; };
        struct value_c_t { int inner; };
        struct value_d_t { int inner; };
        value_a_t value_a{ 4 };
        value_b_t value_b{ 5 };
        value_c_t value_c{ 5 };
        value_d_t value_d{ 5 };
        auto g = nass::make_group_from
        (
            [&] { return value_a; },
            [&]() -> value_b_t& { return value_b; },
            [&]() -> const value_c_t& { return value_c; },
            [&]() -> value_d_t&& { return std::move(value_d); }
        );
        REQUIRE(g.tuple_size == 4);
        REQUIRE(std::is_same_v<decltype(g.get<0>()), value_a_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<1>()), value_b_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<2>()), const value_c_t&>);
        REQUIRE(std::is_same_v<decltype(g.get<3>()), value_d_t&&>);
        REQUIRE(&g.get<0>() != &value_a);
        REQUIRE(&g.get<1>() == &value_b);
        REQUIRE(&g.get<2>() == &value_c);
        REQUIRE(&unmove(g.get<3>()) == &value_d);
    }

    SECTION("single init")
    {
        struct no_duplicates 
        {
            no_duplicates() = default;
            no_duplicates(const no_duplicates&) = delete;
            no_duplicates(no_duplicates&&) = delete;
        };
        auto g = nass::make_group_from([]{ return no_duplicates{}; });
        REQUIRE(std::is_same_v<decltype(g.get<0>()), no_duplicates&>);
    }
}

TEST_CASE("Constexpr copy", "[constexpr][copy]")
{
    struct value_t {};
    static constexpr value_t value{};
    static constexpr auto ga = nass::make_group<value_t, const value_t&>(value, value);
    static constexpr auto gb = ga.copy();
    static_assert(std::is_same_v<decltype(gb.get<0>()), const value_t&>);
    static_assert(std::is_same_v<decltype(gb.get<1>()), const value_t&>);
    static_assert(&ga.get<1>() == &gb.get<1>());
    // The following assert fails in gcc (but works on clang), so i ignore it
    // static_assert(&ga.get<0>() != &gb.get<0>());
}

TEST_CASE("copy", "[copy]")
{
    struct value_t {};
    value_t value{};
    auto ga = nass::make_group<value_t, value_t&, const value_t&>(value, value, value);
    auto gb = ga.copy();
    REQUIRE(std::is_same_v<decltype(gb.get<0>()), value_t&>);
    REQUIRE(std::is_same_v<decltype(gb.get<1>()), value_t&>);
    REQUIRE(std::is_same_v<decltype(gb.get<2>()), const value_t&>);
    REQUIRE(&ga.get<0>() != &gb.get<0>());
    REQUIRE(&ga.get<1>() == &gb.get<1>());
    REQUIRE(&ga.get<2>() == &gb.get<2>());
}

TEST_CASE("duplicate", "[duplicate]")
{
    struct value_t {};
    value_t value{};
    auto ga = nass::make_group<value_t, value_t&, const value_t&, value_t&&>(value, value, value, std::move(value));
    auto gb = ga.duplicate();
    REQUIRE(std::is_same_v<decltype(gb.get<0>()), value_t&>);
    REQUIRE(std::is_same_v<decltype(gb.get<1>()), value_t&>);
    REQUIRE(std::is_same_v<decltype(gb.get<2>()), const value_t&>);
    REQUIRE(std::is_same_v<decltype(gb.get<3>()), value_t&&>);
    REQUIRE(&ga.get<0>() != &gb.get<0>());
    REQUIRE(&ga.get<1>() == &gb.get<1>());
    REQUIRE(&ga.get<2>() == &gb.get<2>());
    REQUIRE(&unmove(ga.get<3>()) == &unmove(gb.get<3>()));
}

TEST_CASE("steal", "[steal]")
{
    struct value_t 
    { 
        value_t() = default;
        value_t(const value_t&) = delete;
        value_t(value_t&&) = default;
    };
    auto ga = nass::make_group_from([]{ return value_t{}; });
    auto gb = ga.steal();
    REQUIRE(&ga.get<0>() != &gb.get<0>());
}

TEST_CASE("Constexpr continuation", "[constexpr][continuation][cont]")
{
    struct value_t {};
    static constexpr value_t value{};
    static constexpr auto g = nass::make_group<value_t, const value_t&>(value, value);
    g >> [&](auto&& arga, auto&& argb)
    {
        static_assert(std::is_same_v<const value_t&, decltype(arga)>);
        static_assert(std::is_same_v<const value_t&, decltype(argb)>);
    };
}

TEST_CASE("continuation", "[continuation][cont]")
{
    struct value_t {};
    value_t value{};
    auto g = nass::make_group<value_t, value_t&, const value_t&, value_t&&>(value, value, value, std::move(value));
    g >> [&](auto&& arga, auto&& argb, auto&& argc, auto&& argd)
    {
        REQUIRE(std::is_same_v<value_t&, decltype(arga)>);
        REQUIRE(std::is_same_v<value_t&, decltype(argb)>);
        REQUIRE(std::is_same_v<const value_t&, decltype(argc)>);
        REQUIRE(std::is_same_v<value_t&&, decltype(argd)>);
        REQUIRE(&arga != &value);
        REQUIRE(&argb == &value);
        REQUIRE(&argc == &value);
        REQUIRE(&argd == &value);
    };
}