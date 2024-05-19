#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <iostream>
#include <stdexcept>
#include <utility>
#include <type_traits>

template <typename... Types>
class Variant;

namespace detail {

    template <typename T, typename... Types>
    struct IndexOf;

    template <typename T, typename First, typename... Rest>
    struct IndexOf<T, First, Rest...> {
        static constexpr size_t value = std::is_same<T, First>::value ? 0 : 1 + IndexOf<T, Rest...>::value;
    };

    template <typename T>
    struct IndexOf<T> {
        static constexpr size_t value = static_cast<size_t>(-1);
    };

    template <size_t Index, typename... Types>
    struct TypeAt;

    template <size_t Index, typename First, typename... Rest>
    struct TypeAt<Index, First, Rest...> {
        using type = typename TypeAt<Index - 1, Rest...>::type;
    };

    template <typename First, typename... Rest>
    struct TypeAt<0, First, Rest...> {
        using type = First;
    };

    template <typename Visitor, typename Variant, typename... Types, size_t... Is>
    decltype(auto) visit_impl(Visitor&& visitor, Variant& var, std::index_sequence<Is...>) {
        using ReturnType = decltype(visitor(std::declval<typename TypeAt<0, Types...>::type&>()));
        using Functor = ReturnType(*)(Visitor&&, Variant&);

        Functor funcs[] = {
            [](Visitor&& vis, Variant& v) -> ReturnType {
                return std::forward<Visitor>(vis)(v.template get<typename TypeAt<Is, Types...>::type>());
            }...
        };

        return funcs[var.index()](std::forward<Visitor>(visitor), var);
    }

} // namespace detail

template <typename... Types>
class Variant {
    static_assert(sizeof...(Types) > 0, "Variant must have at least one type");

    using Storage = std::aligned_union_t<0, Types...>;
    Storage storage;
    size_t type_index;

    template <typename T>
    void destroy();

    template <typename T>
    void construct(const T& value);

    template <typename T>
    void construct(T&& value);

public:
    template <typename T, typename = std::enable_if_t<(std::is_same_v<std::decay_t<T>, Types> || ...)>>
    Variant(T&& value);

    ~Variant();

    Variant(const Variant& other);

    Variant(Variant&& other) noexcept;

    Variant& operator=(const Variant& other);

    Variant& operator=(Variant&& other) noexcept;

    size_t index() const;

    template <typename T>
    bool holds_alternative() const;

    template <typename T>
    T& get();

    template <typename T>
    const T& get() const;

    template <typename Visitor>
    decltype(auto) visit(Visitor&& visitor);

    template <typename Visitor>
    decltype(auto) visit(Visitor&& visitor) const;
};

#include "util/VariantImpl.hpp"

#endif // VARIANT_HPP
