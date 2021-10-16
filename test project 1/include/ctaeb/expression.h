// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Defines a number of expression classes: constant, variable, and
 * compound expression.
 */

#ifndef CTAEB_EXPRESSION_H
#define CTAEB_EXPRESSION_H

#include <string>
#include <functional>
#include <tuple>

/**
 * Defines expression classes: `Constant`, `Variable`, and `Compound`.
 */
namespace ctaeb {

/**
 * Represents a constant value. Holds an immutable value of type `T`. Every
 * time a sub-expression appears in a compound expression that is not a variable
 * nor a compound, it's wrapped into `Constant` object that  permits application
 * of CTAEB machinery to arbitrary types.
 *
 * Example below demonstrates this implicit conversion:
 * @snippet example/constant.cc full
 */
template<typename T>
class Constant {
  public:
    /**
     * Constructs a new constant expression. Creates a copy of the given value;
     * callers may dispense the original object afterwards. Constructor is
     * intentionally implicit; C++ constants are automatically wrapped when
     * necessary to avoid writing @em Constant(x) every time (this is currently
     * a practice in `operations.h`).
     *
     * @param value constant's value object
     */
    Constant(const T &value) : value_(value) { // NOLINT
    }

    /**
     * Returns the held constant's value. Template arguments are not used and
     * are for signature compatibility only.
     *
     * @return this constant's value
     */
    template <typename... Args>
    const T &operator()(Args &&...) const {
        return value_;
    }

    /**
     * Stored constant value.
     */
    const T value_;
};

/**
 * Unevaluated expression. Identified by its index `N`, which is the index in
 * the tuple of values that is supplied during expression evaluation. Because
 * variables have no state, it's not important whether a single or multiple
 * instances of `Variable<N>` exist for any given `N`: they all evaluate to
 * the same value. A variable may have an optional name that helps to identify
 * it in a printed expression. If a name is not explicitly given, it's generated
 * automatically as concatenation of an underscore symbol and the variable's
 * index:
 * @code
 * _1
 * _2
 * ...
 * @endcode
 *
 * Equality comparison for variables results in creation of the corresponding
 * compound expression, rather than a boolean value.
 * Example:
 * @snippet example/variable.cc full
 */
template <size_t N>
class Variable {
  public:
    /**
     * Constructs a nameless variable.
     */
    Variable() : name_(std::string("_") + std::to_string(N)) {
    }

    /**
     * Constructs a named variable.
     */
    explicit Variable(const char* name) : name_(name) {
    }

    /**
     * Constructs a named variable.
     */
    explicit Variable(const std::string &name) : name_(name) {
    }

    const std::string& name() const {
        return name_;
    }

    /**
     * Combination of `Args` and `N` gives a unique `auto` type that equals
     * `decltype(Args[N-1])`.
     */
    template <typename... Args>
    decltype(auto) operator()(Args &&... args) const {
        // this will not compile if not enough arguments are given
        // to an expression that contains the variable
        return std::get<N - 1>(std::forward_as_tuple(args...));
    }

  private:
    /**
     * Variable's name.
     */
    std::string name_;

};

namespace detail {

/**
 * Performs variable substitution and then invokes a given operation. There are
 * several specializations that implement invocation differently, thus the need
 * for a separate abstraction (to avoid specializing `Compound`). Invoker would
 * be an integral part of `Compound` otherwise.
 */
template<template<typename ...> typename>
class Invoker;

} //::detail

/**
 * Binds together one or more expressions and an operation that takes
 * those expressions as arguments. The nested expressions may have different
 * expression types, and may evaluate to the values of the different C++ types,
 * as long as these value types match the expectations of the operations that
 * are applied to them. During evaluation, the operation template template
 * parameter is instantiated using @em void template argument. It's expected
 * that every operation `Op` supports type deduction at least for this
 * particular specialization (@em std::plus and other functional types defined
 * in @em functional, all support this type deduction starting with C++14).
 * Although this class may be used directly by the client code, it's expected
 * that it won't be necessary (at least in the area of intended application).
 * Instead, `auto` type and combining operators (`+`, `-`, etc.) do all the hard
 * work of building the compound expressions.
 *
 * Example:
 * @snippet example/compound.cc full
 */
template <template <typename...> typename Op, typename ...Nested>
class Compound {
  public:
    /**
     * Constructs a compound expression from the given sub-expressions.
     */
    explicit Compound(const Nested &... nested) : expressions_(nested...) {
    }

    const std::tuple<Nested...> & get_expressions() const {
        return expressions_;
    }

    template <typename ...Args>
    decltype(auto) operator()(Args&&... args) const {
        return invoker_(expressions_, std::forward<Args>(args)...);
    }

  private:

    // defined in print.h
    template <template <typename...> typename Op1, typename ...Nested1>
    friend std::ostream & operator<< (
        std::ostream &os,
        const Compound<Op1, Nested1...> & expr
    );

    /**
     * A tuple that holds compound sub-expressions.
     */
    std::tuple<Nested...> expressions_;

    detail::Invoker<Op> invoker_;
};

namespace detail {

/**
 * Common-case specialization propagates input arguments to the nested
 * expressions, and then feeds results into `Operation`.
 */
template <template <typename ...> typename Operation>
class Invoker {
    Operation<void> op;

    /**
     * Additional level of indirection to unfold the sequence from `operator()`.
     */
    template <typename Tuple, typename... Args, std::size_t... I>
    decltype(auto) eval(const Tuple &tuple, std::index_sequence<I...>, Args&&... args) const {
        return op(std::get<I>(tuple)(std::forward<Args>(args)...)...);
        //return op( (std::forward<decltype(std::get<I>(tuple)(std::forward<Args>(args)...))>(
        //    std::get<I>(tuple)(std::forward<Args>(args)...)))...
        //);
    }

  public:
    /**
     * Applies evaluation to the nested expressions, and then applies the
     * compound's operation on the resulting tuple. The process recurs
     * from the compounds down to variables and constants. This is the reason
     * for all expression types to implement `operator()`.
     */
    template <typename Tuple, typename ...Args>
    decltype(auto) operator()(const Tuple &tuple, Args &&... args) const {
        return eval(tuple, std::make_index_sequence<std::tuple_size<Tuple>::value>(), std::forward<Args>(args)...);
    }
};

/**
 * Implements compound expression evaluation when the top-level nested
 * expressions are joined by @em std::logical_and. This operation is evaluated
 * lazily, therefore the common case is not applicable.
 */
template<>
class Invoker<std::logical_and> {
    std::logical_and<void> operation;
  public:
    /**
     * Applies evaluation to the first nested expression; if the result is
     * convertible to @em false, returns @em false, leaving the second expression
     * unevaluated. Otherwise, evaluates the second expression and combines
     * the two sub-results via @em std::logical_and.
     * @tparam T1 the type of the first nested expressions
     * @tparam T2 the type of the second nested expressions
     * @tparam Args types of the input values
     * @param tuple nested expressions as captured by the corresponding Compound
     * @param args input values
     * @return Operation(e1(v1, v2...), e2(v1, v2, ...), ...)
     */
    template <typename T1, typename T2, typename ...Args>
    decltype(auto) operator()(const std::tuple<T1, T2> &tuple, Args &&... args) const {
        auto res1 = std::get<0>(tuple)(std::forward<Args>(args)...);
        if (!res1) {
            return false;
        }
        auto res2 = std::get<1>(tuple)(std::forward<Args>(args)...);
        return operation(res1, res2);
    }

};

/**
 * Implements compound expression evaluation when the top-level nested
 * expressions are joined by @em std::logical_or. This operation is evaluated
 * lazily, therefore the common case is not applicable.
 */
template<>
class Invoker<std::logical_or> {
    std::logical_or<void> operation;
  public:
    /**
     * Applies evaluation to the first nested expression; if the result is
     * convertible to @em true, returns @em true, leaving the second expression
     * unevaluated. Otherwise, evaluates the second expression and combines
     * the two sub-results via @em std::logical_or.
     * @tparam T1 the type of the first nested expressions
     * @tparam T2 the type of the second nested expressions
     * @tparam Args types of the input values
     * @param tuple nested expressions as captured by the corresponding Compound
     * @param args input values
     * @return Operation(e1(v1, v2...), e2(v1, v2, ...), ...)
     */
    template <typename T1, typename T2, typename ...Args>
    decltype(auto) operator()(const std::tuple<T1, T2> &tuple, Args &&... args) const {
        auto res1 = std::get<0>(tuple)(std::forward<Args>(args)...);
        if (res1) {
            return true;
        }
        auto res2 = std::get<1>(tuple)(std::forward<Args>(args)...);
        return operation(res1, res2);
    }

};

template<typename>
struct is_variable : std::false_type {
};

template<std::size_t N>
struct is_variable<ctaeb::Variable<N>> : std::true_type {
};

template<typename>
struct is_constant : std::false_type {
};

template<typename T>
struct is_constant<ctaeb::Constant<T>> : std::true_type {
};

template<typename>
struct is_compound : std::false_type {
};

template<template<typename...> typename Op, typename ...Args>
struct is_compound<ctaeb::Compound<Op, Args...>> : std::true_type {
};

template <typename T>
struct is_expression : std::disjunction<is_variable<T>,
                                        is_constant<T>,
                                        is_compound<T>> {
};

} //::detail

template<typename T>
using Expression = std::enable_if_t<detail::is_expression<std::decay_t<T>>::value>;

template<typename T>
using NonExpression = std::enable_if_t<not detail::is_expression<std::decay_t<T>>::value>;

template<typename E1, typename E2>
using Expressions = std::enable_if_t<
    detail::is_expression<std::decay_t<E1>>::value && detail::is_expression<std::decay_t<E2>>::value
>;

} //::ctaeb

#endif //CTAEB_EXPRESSION_H
