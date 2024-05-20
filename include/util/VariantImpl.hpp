#ifndef VARIANT_IMPL_HPP
#define VARIANT_IMPL_HPP

#include "util/Variant.hpp"

template <typename... Types>
template <typename T>
void Variant<Types...>::destroy() {
    if (holds_alternative<T>()) {
        reinterpret_cast<T*>(&storage)->~T();
    }
}

template <typename... Types>
template <typename T>
void Variant<Types...>::construct(const T& value) {
    using DecayedT = std::decay_t<T>;
    new (&storage) DecayedT(value);
}

template <typename... Types>
template <typename T>
void Variant<Types...>::construct(T&& value) {
    using DecayedT = std::decay_t<T>;
    new (&storage) DecayedT(std::forward<T>(value));
}

template <typename... Types>
template <typename T, typename>
Variant<Types...>::Variant(T&& value)
    : type_index(detail::IndexOf<std::decay_t<T>, Types...>::value) {
    static_assert(detail::IndexOf<std::decay_t<T>, Types...>::value != static_cast<size_t>(-1), "Type not found in Variant");
    construct(std::forward<T>(value));
}

template <typename... Types>
Variant<Types...>::~Variant() {
    visit([](auto& value) { using T = std::decay_t<decltype(value)>; value.~T(); });
}

template <typename... Types>
Variant<Types...>::Variant(const Variant& other)
    : type_index(other.type_index) {
    other.visit([this](const auto& value) { using T = std::decay_t<decltype(value)>; construct(value); });
}

template <typename... Types>
Variant<Types...>::Variant(Variant&& other) noexcept
    : type_index(other.type_index) {
    other.visit([this](auto& value) { using T = std::decay_t<decltype(value)>; construct(std::move(value)); });
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(const Variant& other) {
    if (this != &other) {
        visit([](auto& value) { using T = std::decay_t<decltype(value)>; value.~T(); });
        type_index = other.type_index;
        other.visit([this](const auto& value) { using T = std::decay_t<decltype(value)>; construct(value); });
    }
    return *this;
}

template <typename... Types>
Variant<Types...>& Variant<Types...>::operator=(Variant&& other) noexcept {
    if (this != &other) {
        visit([](auto& value) { using T = std::decay_t<decltype(value)>; value.~T(); });
        type_index = other.type_index;
        other.visit([this](auto& value) { using T = std::decay_t<decltype(value)>; construct(std::move(value)); });
    }
    return *this;
}

template <typename... Types>
size_t Variant<Types...>::index() const {
    return type_index;
}

template <typename... Types>
template <typename T>
bool Variant<Types...>::holds_alternative() const {
    return type_index == detail::IndexOf<T, Types...>::value;
}

template <typename... Types>
template <typename T>
T& Variant<Types...>::get() {
    if (!holds_alternative<T>()) {
        throw std::bad_variant_access();
    }
    return *reinterpret_cast<T*>(&storage);
}

template <typename... Types>
template <typename T>
const T& Variant<Types...>::get() const {
    if (!holds_alternative<T>()) {
        throw std::bad_variant_access();
    }
    return *reinterpret_cast<const T*>(&storage);
}

template <typename... Types>
template <typename Visitor>
decltype(auto) Variant<Types...>::visit(Visitor&& visitor) {
    return detail::visit_impl<Visitor, Variant<Types...>, Types...>(
        std::forward<Visitor>(visitor), *this, std::index_sequence_for<Types...>{});
}

template <typename... Types>
template <typename Visitor>
decltype(auto) Variant<Types...>::visit(Visitor&& visitor) const {
    return detail::visit_impl<Visitor, const Variant<Types...>, Types...>(
        std::forward<Visitor>(visitor), *this, std::index_sequence_for<Types...>{});
}

#endif // VARIANT_IMPL_HPP
