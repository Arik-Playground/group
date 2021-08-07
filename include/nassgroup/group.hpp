#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

namespace nass
{
    template <typename T>
    struct holder_value
    {
        using item_t = T;
        item_t item;
        constexpr auto&& inner_ref()
        {
            if constexpr (std::is_same_v<std::remove_reference_t<item_t>, item_t>)
            {
                return item;
            }
            else
            {
                return std::forward<item_t>(item);
            }
        }
    };

    template <typename... Ts>
    struct group_descriptor {};

    template <typename... Ts, typename... ArgsTs>
    constexpr auto create_group_getter(group_descriptor<Ts...>, ArgsTs&&... args)
    {
        return [...vals = holder_value<Ts>{std::forward<ArgsTs>(args)}] <typename OpT> (OpT&& op) -> decltype(auto) 
        { 
            return std::forward<OpT>(op)(vals...); 
        };
    }

    template <typename GetterT>
    struct group;

    template <typename... Ts, typename... ArgsTs>
    constexpr auto make_group(ArgsTs&&... args)
    {
        static_assert(sizeof...(Ts) <= sizeof...(args), "Too many initializers passed to `make_group`");
        static_assert(sizeof...(Ts) >= sizeof...(args), "Not enough initializers passed to `make_group`");
        constexpr auto creator = [](auto&&... args)
        {
            return create_group_getter(group_descriptor<Ts...>{}, std::forward<ArgsTs>(args)...);
        };

        using getter_t = decltype(creator(args...));
        return group<getter_t> { creator(args...) };
    }

    template <typename... ArgsTs>
    constexpr auto make_value_group(ArgsTs&&... args)
    {
        constexpr auto creator = [](auto&&... args)
        {
            return create_group_getter(group_descriptor<std::remove_cvref_t<ArgsTs>...>{}, std::forward<ArgsTs>(args)...);
        };

        using getter_t = decltype(creator(args...));
        return group<getter_t> { creator(std::forward<ArgsTs>(args)...) };
    }

    template <typename... ArgsTs>
    constexpr auto make_forward_group(ArgsTs&&... args)
    {
        constexpr auto creator = [](auto&&... args)
        {
            return create_group_getter(group_descriptor<ArgsTs&&...>{}, std::forward<ArgsTs>(args)...);
        };

        using creator_t = decltype(creator(args...));
        return group<creator_t> { creator(args...) };
    }

    template <typename... Ts, typename... OpTs>
    constexpr auto create_group_getter_from(OpTs&&... ops)
    {
        return [...vals = holder_value<decltype(std::forward<OpTs>(ops)())>{std::forward<OpTs>(ops)()}] <typename OpT> (OpT&& op) -> decltype(auto) 
        { 
            return std::forward<OpT>(op)(vals...); 
        };
    }

    template <typename... OpTs>
    constexpr auto make_group_from(OpTs&&... ops)
    {
        constexpr auto creator = [](auto&&... ops)
        {
            return create_group_getter_from(std::forward<OpTs>(ops)...);
        };

        using getter_t = decltype(creator(ops...));
        return group<getter_t> { creator(ops...) };
    }

    template <typename GetterT>
    struct group
    {
        GetterT getter;

        static constexpr auto tuple_size_getter = [](auto&&... args) { return std::integral_constant<size_t, sizeof...(args)>{}; };
        using tuple_size_t = decltype(std::declval<GetterT>()(tuple_size_getter));
        static constexpr size_t tuple_size = tuple_size_t::value;

        template <typename T, bool SelectedV>
        struct item_retriever;

        template <typename T>
        struct item_retriever<T, false>
        {
            T& item;

            template <typename ThatT>
            constexpr ThatT operator| (ThatT that)
            {
                return that;
            }
        };

        template <typename T>
        struct item_retriever<T, true>
        {
            T& holder;

            template <typename ThatT>
            constexpr item_retriever<T, true> operator| (ThatT)
            {
                return *this;
            }
        };

        template <size_t IdxV, typename... ArgsTs, size_t... IdxsVs>
        static constexpr auto find(std::index_sequence<IdxsVs...>, ArgsTs&&... args)
        {
            return [](auto... args)
            { 
                return (args | ...); 
            }( item_retriever<ArgsTs, (IdxsVs - IdxV == 0)>{args}... );
        }

        template <size_t IdxV>
        constexpr auto&& get() const
        {
            return getter([](const auto&... args)
            {
                static_assert(sizeof...(args) > IdxV, "requested index is bigger than the group size");
                return find<IdxV>(std::make_index_sequence<sizeof...(args)>(), args...);
            }).holder.item;
        }

        template <size_t IdxV>
        auto&& get()
        {
            auto& cholder = getter([](const auto&... args)
            {
                static_assert(sizeof...(args) > IdxV, "requested index is bigger than the group size");
                return find<IdxV>(std::make_index_sequence<sizeof...(args)>(), args...);
            }).holder;

            auto& holder = const_cast<std::remove_cvref_t<decltype(cholder)>&>(cholder);
            return holder.inner_ref();
        }

        constexpr auto copy() const
        {
            return getter([](auto&... holders)
            {
                constexpr auto creator = [] (auto&... holders)
                {
                    return create_group_getter(
                        group_descriptor<decltype(holders.item)...>{}, 
                        holders.item...);
                };
                using getter_t = decltype(creator(holders...));
                return group<getter_t>{ creator(holders...) };
            });
        }

        constexpr auto duplicate()
        {
            return getter([](auto&... cholders)
            {
                constexpr auto creator = [] (auto&... holders)
                {
                    return create_group_getter(
                        group_descriptor<decltype(holders.item)...>{}, 
                        holders.inner_ref()...);
                };
                using getter_t = decltype(creator(const_cast<std::remove_cvref_t<decltype(cholders)>&>(cholders)...));
                return group<getter_t>{ creator(const_cast<std::remove_cvref_t<decltype(cholders)>&>(cholders)...) };
            });
        }

        constexpr auto steal()
        {
            return getter([](auto&... cholders)
            {
                constexpr auto creator = [] (auto&... holders)
                {
                    return create_group_getter(
                        group_descriptor<decltype(holders.item)...>{}, 
                        std::move(holders.inner_ref())...);
                };
                using getter_t = decltype(creator(const_cast<std::remove_cvref_t<decltype(cholders)>&>(cholders)...));
                return group<getter_t>{ creator(const_cast<std::remove_cvref_t<decltype(cholders)>&>(cholders)...) };
            });
        }

        template <typename OpT>
        constexpr decltype(auto) operator>> (OpT&& op) const
        {
            return getter([&op] (auto&... holders)
            {
                return std::forward<OpT>(op)(holders.item...);
            });
        }

        template <typename OpT>
        constexpr decltype(auto) operator>> (OpT&& op)
        {
            return getter([&op] (auto&... cholders)
            {
                return std::forward<OpT>(op)(const_cast<std::remove_cvref_t<decltype(cholders)>&>(cholders).inner_ref()...);
            });
        }
    };
}