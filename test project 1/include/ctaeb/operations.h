// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Provide support for expression composition via algebraic operations,
 * such as @em std::plus, @em std::minus, etc. This header is not required, but
 * most likely needed for convenience.
 */

#ifndef CTAEB_OPERATIONS_H
#define CTAEB_OPERATIONS_H

#include <functional>
#include <type_traits>

#include "expression.h"

namespace ctaeb {

/**
 * Creates (E1 + E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator+(E1 &&x, E2 &&y) {
    return Compound<std::plus, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E + T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::plus, E, Constant<T>> operator+(E &&x, T &&y) {
    return ctaeb::Compound<std::plus, E, ctaeb::Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}


/**
 * Creates (T + E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::plus, Constant<T>, E> operator+(T &&x, E &&y) {
    return ctaeb::Compound<std::plus, ctaeb::Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 - E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator-(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::minus, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E - T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::minus, E, Constant<T>> operator-(E &&x, T &&y) {
    return Compound<std::minus, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T - E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::minus, Constant<T>, E> operator-(T &&x, E &&y) {
    return Compound<std::minus, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 * E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator*(E1 &&x, E2 &&y) {
    return Compound<std::multiplies, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E * T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::multiplies, E, Constant<T>> operator*(E &&x,  T &&y) {
    return Compound<std::multiplies, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}


/**
 * Creates (T * E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::multiplies, Constant<T>, E> operator*(T &&x, E &&y) {
    return Compound<std::multiplies, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 * E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator/(E1 &&x, E2 &&y) {
    return Compound<std::divides, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E * T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::divides, E, Constant<T>> operator/(E &&x, T &&y) {
    return Compound<std::divides, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}


/**
 * Creates (T * E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::divides, Constant<T>, E> operator/(T &&x, E &&y) {
    return Compound<std::divides, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 == E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator==(E1 &&x, E2 &&y) {
    return Compound<std::equal_to, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E == T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::equal_to, E, Constant<T>> operator==(E &&x,  T &&y) {
    return Compound<std::equal_to, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T == E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::equal_to, Constant<T>, E> operator==(T &&x, E &&y) {
    return Compound<std::equal_to, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 != E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator!=(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::not_equal_to, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::not_equal_to, E, Constant<T>> operator!=(E &&x, T &&y) {
    return Compound<std::not_equal_to, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T == E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::not_equal_to, Constant<T>, E> operator!=(T &&x, E &&y) {
    return Compound<std::not_equal_to, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 < E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator<(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::less, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E < T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::less, E, Constant<T>> operator<(E &&x,  T &&y) {
    return Compound<std::less, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T < E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::less, Constant<T>, E> operator<(T &&x, E &&y) {
    return Compound<std::less, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 <= E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator<=(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::less_equal, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E < T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::less_equal, E, Constant<T>> operator<=(E &&x,  T &&y) {
    return Compound<std::less_equal, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T < E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::less_equal, Constant<T>, E> operator<=(T &&x, E &&y) {
    return Compound<std::less_equal, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1> E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator>(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::greater, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E > T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::greater, E, Constant<T>> operator>(E &&x,  T &&y) {
    return Compound<std::greater, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T > E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::greater, Constant<T>, E> operator>(T &&x, E &&y) {
    return Compound<std::greater, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 >= E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator>=(E1 &&x, E2 &&y) {
    return Compound<std::greater_equal, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E >= T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::greater_equal, E, Constant<T>> operator>=(E &&x,  T &&y) {
    return Compound<std::greater_equal, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T >= E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::greater_equal, Constant<T>, E> operator>=(T &&x, E &&y) {
    return Compound<std::greater_equal, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 && E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator&&(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::logical_and, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E && T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::logical_and, E, Constant<T>> operator&&(E &&x,  T &&y) {
    return Compound<std::logical_and, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T && E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::logical_and, Constant<T>, E> operator&&(T &&x, E &&y) {
    return Compound<std::logical_and, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (E1 || E2) compound expression.
 */
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator||(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::logical_or, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}

/**
 * Creates (E || T) compound expression.
 */
template<typename E, typename T, typename = Expression<E>, typename = NonExpression<T>>
Compound<std::logical_or, E, Constant<T>> operator||(E &&x,  T &&y) {
    return Compound<std::logical_or, E, Constant<T>>(std::forward<E>(x), std::forward<T>(y));
}

/**
 * Creates (T || E) compound expression.
 */
template<typename T, typename E, typename = NonExpression<T>, typename = Expression<E>>
Compound<std::logical_or, Constant<T>, E> operator||(T &&x, E &&y) {
    return Compound<std::logical_or, Constant<T>, E>(std::forward<T>(x), std::forward<E>(y));
}

/**
 * Creates (!E) compound expression.
 */
template<typename E, typename = Expression<E> >
auto operator!(E &&x) {
    return Compound<std::logical_not, E>(x);
}

/**
 * Creates (E1 ^ E2) compound expression.
 */
/*
template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator ^(E1 &&x, E2 &&y) {
    return ctaeb::Compound<std::bit_xor, E1, E2>(std::forward<E1>(x), std::forward<E2>(y));
}
*/

template<typename E, typename = Expression<E> >
auto operator-(E &&x) {
    return Compound<std::negate, E>(x);
}

}

#endif //CTAEB_OPERATIONS_H
