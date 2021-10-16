// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Defines streaming operators for expression classes. This header
 * is optional, it's needed only if one needs to print expression to
 * @em std::ostream.
 */

#ifndef CTAEB_PRINT_H
#define CTAEB_PRINT_H

// for std::string
#include <string>

// for std::ostream
#include <ostream>

// for std::stringstream
#include <sstream>

// for std::index_sequence
#include <utility>

#include "expression.h"

namespace ctaeb {

/**
 * Provides functionality for expression printing.
 */
namespace print {

/**
 * Returns string representation of an operation denoted by the template
 * template parameter. Due to generic nature of the algebraic operations,
 * operand types are not required for the visualization of operation classes.
 */
template<template<typename...> typename Operation>
std::string to_string();

template<typename X>
void emit(std::ostream &os, X &&x, bool last) {
    os << x;
    if (!last) {
        os << ", ";
    }
}

template<typename... Ts, std::size_t... I>
std::ostream &print(std::ostream &os,
                    const std::tuple<Ts...> &tuple,
                    std::index_sequence<I...>) {
    auto _ = {0, (emit(os, std::get<I>(tuple), I + 1 >= sizeof...(Ts)), 0)...};
    static_cast<void>(_);

    return os;
}

/**
 * Stringifies given variadic argument pack. The arguments are expected
 * to inherit from `Expression`.
 */
template<typename... Args>
std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &tuple) {
    return print(os, tuple, std::make_index_sequence<sizeof...(Args)>());
}

/**
 * `ctaeb::to_string()` specializations for algebraic operations.
 */
template<>
std::string to_string<std::plus>() {
    return "+";
}

template<>
std::string to_string<std::multiplies>() {
    return "*";
}

template<>
std::string to_string<std::divides>() {
    return "/";
}

template<>
std::string to_string<std::minus>() {
    return "-";
}

template<>
std::string to_string<std::negate>() {
    return "-";
}

template<>
std::string to_string<std::equal_to>() {
    return "==";
}

template<>
std::string to_string<std::not_equal_to>() {
    return "!=";
}

template<>
std::string to_string<std::less>() {
    return "<";
}

template<>
std::string to_string<std::less_equal>() {
    return "<=";
}

template<>
std::string to_string<std::greater_equal>() {
    return ">=";
}

template<>
std::string to_string<std::greater>() {
    return ">";
}

template<>
std::string to_string<std::logical_and>() {
    return "&&";
}

template<>
std::string to_string<std::logical_or>() {
    return "||";
}

template<>
std::string to_string<std::logical_not>() {
    return "not ";
}

template<>
std::string to_string<std::unary_negate>() {
    return "not ";
}

template<>
std::string to_string<std::bit_xor>() {
    return "^";
}

template<typename>
struct sfinae_true : std::true_type {
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"

/**
 * By default, all two-variable operations are printed in infix form.
 * If an operation type `Op` defines a member @em prefixed, it will be printed
 * in prefix form instead.
 *
 * Example:
 * @snippet example/prefixed_operator.cc full
 */
template<template <typename...> typename T>
struct prefixed {
    template<typename X>
    static auto check(X) -> typename sfinae_true<decltype(X::prefixed)>::type;

    static std::false_type check(...);

    using type = decltype(check(std::declval<T<>>()));
    constexpr static bool value = type::value;
};

#pragma clang diagnostic pop


} // print::

/**
 * Writes the constant's representation into the given output stream.
 */
template<class T>
std::ostream &operator<<(std::ostream &os, const Constant <T> &expr) {
    os << expr();

    return os;
}

/**
 * Writes the variable's representation into the given output stream.
 */
template<size_t N>
std::ostream &operator<<(std::ostream &os, const Variable <N> &expr) {
    os << expr.name();

    return os;
}

/**
 * Writes the compound's representation into the given output stream.
 */
template<template<typename...> typename Op, typename ...Ts>
std::ostream &operator<<(std::ostream &os, const Compound<Op, Ts...> &expr) {
    using namespace print;

    os << to_string<Op>() << "(" << expr.get_expressions() << ")";

    return os;
}

template<template<typename...> typename Op, typename T>
std::ostream &operator<<(std::ostream &os, const Compound <Op, T> &expr) {
    using namespace print;

    std::string op = to_string<Op>();
    constexpr bool use_prefix_form = prefixed<Op>::value;

    if (use_prefix_form) {
        os << op << "(" << std::get<0>(expr.get_expressions()) << ")";
    }
    else {
        os << op << std::get<0>(expr.get_expressions());
    }

    return os;
}

template<template<typename...> typename Op, typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const Compound <Op, T1, T2> &expr) {
    using namespace print;

    std::string op = to_string<Op>();
    constexpr bool use_prefix_form = prefixed<Op>::value;

    if (use_prefix_form) {
        os << op << "(" << expr.get_expressions() << ")";
    }
    else {
        os << std::get<0>(expr.get_expressions())
           << " "
           << op
           << " "
           << std::get<1>(expr.get_expressions());
    }

    return os;
}

template <typename T, typename = Expression<T>>
std::string to_string(T && expr) {
    std::stringstream str_stream;
    str_stream << std::forward<T>(expr);

    std::string invariant_str = str_stream.str();

    return invariant_str;
}

} // ctaeb::

#endif //CTAEB_PRINT_H
