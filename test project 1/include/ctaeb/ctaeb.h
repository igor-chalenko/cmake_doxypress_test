// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Entry point into ctaeb, a library for building algebraic
 * expressions at compile time. This file should be the only header necessary
 * to include to start using it.
 */

#ifndef CTAEB_CTAEB_H
#define CTAEB_CTAEB_H

/**
 * @mainpage CTAEB - compile-time algebraic expression library
 * @section description Description
 * Algebraic expression is a formula that contains symbols and rules that
 * manipulate those symbols. In C++, @em symbols mean @em variables, and
 * @em rules mean operators and functions. Compile-time algebraic expression
 * is a special (C++) variable that holds the definition of such a formula.
 * Finally, @em CTAEB is a small header-only library that may be used to define
 * such variables and then evaluate them at run time. From now on, these
 * variables will be called @em expressions. Once an expression is built, it
 * may be evaluated many times with different values of participating variables.
 * It could be compared to @em std::function in this regard, except that:
 * - expressions are truly generic - the same expression accepts values of any
 * types;
 * - different expressions always have different types, even if they have the
 * same inputs and return type - this is a direct consequence of the previous
 * observation;
 * - expressions are stateless - variable binding is not supported.
 *
 * The library is not capable of any symbolic manipulations (aside from simple
 * variable substitution). If a CTAEB expression compiles, it evaluates to
 * a constant value. In other words, with this
 * library one can implement a calculator with re-usable compound expressions.
 * Consider an example:
 * @snippet example/overview1.cc full
 * First, we define variables @em x and @em y. They are the building blocks of
 * the expressions we are going to build:
 * @snippet example/overview1.cc variables
 * Then we define the expression we want to evaluate:
 * @snippet example/overview1.cc compound
 * Next, we evaluate this expression using values of different types:
 * @snippet example/overview1.cc evaluation
 * The program produces:
 * @code
 * 7
 * concatenation
 * (5, 7, 9)
 * @endcode
 * The first evaluation sums two integers. The second one concatenates two
 * strings since that's how `operator +` is defined for @em std::string. The
 * third one sums two vectors of length 3, for which two required functions are
 * implemented: `operator+` for summation, and `operator<<` for printing.
 *
 * Consider another example:
 * @snippet example/overview2.cc full
 * The following lines won't compile if uncommented:
 * @snippet example/overview2.cc error
 * Note that `1` in the example above is *not* converted to UDT even though
 * the corresponding constructor is implicit. Since the same expression
 * may be evaluated using the data of different types, any constant in that
 * expression has to have its original type. (This also comes from the compiled
 * nature of the expressions). Thus, any type conversion must be performed
 * outside the expressions; the library doesn't do it by itself.
 * @section tested_platforms_section Tested platforms and compilers
 * The library is known to work in the following environments:
 * - Cygwin with GCC 6.4.0
 * - Windows with MinGW-provided GCC 6.4.0 or higher
 * - Ubuntu 14.02 or higher with GCC 6.4.0 or higher
 * The library uses some C++17 features and therefore requires a modern
 * compiler (as of 2018, "modern" GCC is 7.2.0).
 * @section installation_section Build and install
 * Building @em CTAEB requires @em cmake 3.0 or higher.
 * - Go to the project root directory, create a build sub-directory and run
 * @em cmake in it:
 * @code
 * mkdir build
 * cd build
 * cmake ../
 * @endcode
 * - if the previous step succeeds, run @em make:
 * @code
 * make check
 * # all the tests must pass
 * sudo make install
 * @endcode
 * @section usage_section Usage
 * Once installation completes, @em find_package(ctaeb) should work in your
 * cmake-based projects. The library consists of the following include files:
 * - `ctaeb.h` - library's main header file
 * - `expression.h` - defines the library core abstractions
 * - `operations.h` - defines the convenience operators (`+`, `-`, `<`, etc.)
 * - `print.h` - defines functions for expression printing
 *
 * In order to use the library, include the library's main header `ctaeb.h`:
 * @code
 * // for ctaeb::Variable and ctaeb::Compound
 * #include <ctaeb/ctaeb.h>
 * @endcode
 * It in turn includes all the necessary header files. It's possible to use
 * the library without `operations.h` or `print.h` included: just include
 * `expression.h` instead of `ctaeb.h`.
 * @section expressions_section Expressions
 * An expression is either a `Constant` expression, a `Variable`, or a
 * `Compound`. Compound expression joins one or more expressions by a function
 * (operation) that has a corresponding arity. This vocabulary lets us build
 * expressions of elementary algebra and evaluate them using some values as
 * variable substitutions. Variables are typeless; substitution is only possible
 * for the values of types that support participating operations.
 * For example, evaluation of the expression `a + b` compiles for any integral
 * type but doesn't for enums.
 * @subsection nesting_subsection Nesting
 * Compounds may nest inside compounds, thus we may build expressions of
 * arbitrary complexity as long as our C++ compiler is able to handle them.
 * An exact type of an expression may become quite complex, but the clients
 * use automatic type deduction and mostly do not need
 * to know anything about it. Consider an example:
 * @snippet example/expression.cc full
 * We use explicit type declarations instead of @em auto to demonstrate how
 * the expression composition works. First, there's a sum of two variables:
 * @snippet example/expression.cc first
 * The compound's type captures the operator and the types of the arguments.
 * The same holds for the second compound:
 * @snippet example/expression.cc second
 * Then, we combine the two compounds into yet another sum:
 * @snippet example/expression.cc third
 * Unabbreviated type of this third compound is
 * @code
 * Compound<std::plus,
 *          Compound<std::plus, Variable<1>, Variable<2>>,
 *          Compound<std::minus, Variable<1>, Variable<2>>
 * >
 * @endcode
 * From the standpoint of algebra, we wrote `(a + b) + (a - b)`. CTAEB can't
 * do simplification at this time, therefore evaluation of this expression
 * is performed "as is": the first sub-expression is evaluated, then the second
 * one, and then two sub-results are summed. However, evaluation of logical
 * "or" and "and" operators is different: it behaves in the same way as their
 * C++ counterpart. If the first operand's value defines the logical operator's
 * value, the second operand stays unevaluated.
 * @anchor printing_subsection_anchor
 * @subsection printing_subsection Printing
 * Expressions are printable in a natural way - one only needs to give
 * meaningful names to the variables in the printed expression:
 * @snippet example/print.cc full
 * This example produces:
 *
 * @code
 * a + b
 * @endcode
 *
 * Operations used in CTAEB expressions use the following function to provide
 * their string representation:
 * @code
 * namespace ctaeb { namespace print {
 * // return a string that prints the operation
 * template <>
 * std::string to_string<Operation>();
 * } //::print } //::ctaeb
 * @endcode
 * Infix and prefix operations are distinguished; see
 * `ctaeb::print::to_string()` and `ctaeb::print::prefixed` for further
 * information.
 *
 * @section evaluation Evaluation
 * In order to evaluate an expression, its `operator()` is called with
 * a corresponding number of arguments. On the level of compound expression
 * being evaluated this substitution works because of the way @em operator() is
 * implemented. It's called for every nested expression; the input values are
 * propagated to each call without changes. Depending on the sub-expression
 * class, one of the following takes places in each nested expression:
 * - if an expression is a compound, the propagation repeats recursively. This
 * is how compound expressions are built.
 * - if an expression is a variable, an index of an argument in the input tuple
 * corresponds to a variable's index (the template parameter `N`). This is
 * exactly what happens in the variable's @em operator():
 * @code
 * template <typename... Args>
 * const auto& operator()(Args&&... args) const {
 *     return std::get<N - 1>(std::tie(args...));
 * }
 * @endcode
 * Consider an example:
 * @snippet example/expression.cc evaluation
 * In the code above the first variable gets the value of 10, and the second
 * one the value of -5. Every occurrence of _1 (of type `Variable<1>`) is
 * replaced by 10, and every occurrence of _2 (of type `Variable<2>`) is
 * replaced by -5.
 * - if an expression is a constant, constant's value is returned.
 * This process continues until all sub-expressions are evaluated. As can be
 * seen from the description above, recursion stops when it encounters a
 * constant or a variable.
 * @section motivation_section Motivation
 * This library was created as a side development of a larger project dedicated
 * to container class testing. In the standard C++ library, container behavior
 * is described in terms of the semantic requirements that are re-used across
 * different classes and their member functions. After a little trial and error,
 * it became obvious that these requirements *may* and *should* be specified
 * declaratively by the code that intends to test such a class. Let's consider
 * a test of a container class, written in a straightforward manner:
 * @code
 * void test_insert(Container &c, const T &element) {
 *     size_type size = c.size();
 *
 *     c.insert(element);
 *     assert(c.contains(element));
 *     assert(c.size() == size + 1);
 * }
 *
 * @endcode
 * In the above code, assertions about effects of 'insert' member function
 * are coupled with the test code. This limits its re-usability, and, when
 * needed, it would just be copy-pasted from one test to another with small
 * alterations, leading to the code duplication. Most importantly, these
 * assertions capture @em invariants that actually do not belong to any
 * particular test and thus should not be defined by one. Invariants must be
 * captured by some code between the actual implementation and the tests.
 * It means that the assertion code must be decoupled and attached in some
 * manner to a given member function of a given class, so that *any* test
 * executing that member function will implicitly test these assertions as well.
 * If we follow this idea and change our test accordingly, its code might look
 * something like this:
 * @code
 * void test_insert(Container &c, const T &element) {
 *     call(c, [&] () { c.insert(element) }, {
 *             _A == A + x,
 *             size(A) == size(_A) + 1,
 *         }
 *     );
 * }
 * @endcode
 * In this example, `A` is a container `c` before insert invocation, `_A` is
 * the same container after it, `x` is the `element` being inserted. Neither
 * of these variables have any value at the time `call(...)` is executed. Once
 * the tested member function completes, they are evaluated by the newly
 * introduced `call`, which is re-usable and essentially does the same work
 * as does the code from the previous example. But now `call` receives
 * assertions as parameters, and `test_insert` is now in an intermediate layer
 * between the library code and the tests. It's reusable by any test that
 * needs to test `insert` member function.
 *
 * Essentially, the code above uses its own DSL to define invariants in a form
 * close to the wording of the C++ standard. But how this kind of notation might
 * be implemented? These invariants might be sufficiently complex and generic
 * (this is important for a comprehensive test of any class template), thus
 * the need arises to implement a compact and easy-to-read DSL that would permit
 * writing expressions such as above. The first step to understand how this DSL
 * may be built is to capture the test logic into semi-reusable sub-functions:
 * @code
 * template<typename... Args>
 * void call(Container &c, Operation operation, Args&&... args) {
 *     State before = save_state(c);
 *     typename Operation::result_type r = operation(args);
 *     State after = save_state(c);
 *     check_invariants<Operation>(before, after, r, args...);
 * }
 *
 * template<template <typename...> class Operation, typename... Args>
 * void check_invariants(const State &, const State &, Args &&... args);
 *
 * template<>
 * void check_invariants<insert>(const State & s1, const State &s2, T x) {
 *     assert(s2 - s1 == x, "in 'insert': A' - A == x");
 * }
 * @endcode
 * This requires writing invariant checking function for every member being
 * tested (the last function in the code above). Code would be duplicated, too,
 * because many functions might have similar assertions with only subtle
 * differences between them. To improve the above code, we may use a bit of
 * functional programming: write invariants as lambda functions and pass them
 * to a generic implementation of @em check_invariants. This generic version
 * would then evaluate each lambda substituting the values from the context into
 * variables (via @em std::bind). @em CTAEB helps to implement these lambdas
 * as we've seen above. There are a few benefits to get from this approach:
 * - the expression code is compiled using C++ rules, therefore the built DSL
 * is largely self-descriptive
 * - validity of the built expressions is checked by the C++ compiler rather
 * than by some custom code - this helps to discover errors early
 * - the invariant expressions may naturally contain compile-time checks
 * in the form of @em static_assert or @em concept checks
 *
 * @section memory_usage_section Memory usage
 * The following rules apply:
 * - constants of trivially destructible types are trivially destructible;
 * - variables are not trivially destructible because they have a name of
 * the type @em std::string;
 * - compounds inherit their destructibility from the nested sub-expressions,
 * because @em std::tuple is used internally by the `Compound` class, and since
 * C++17, the language specification requires that if all tuple types are
 * trivially destructible, than the tuple is trivially destructible too.
 *
 * Since all expressions are stateless, at compile time the only cause of
 * dynamic memory allocations is a name string inside each variable. At
 * evaluation time, new objects may be constructed as a result of
 * intermediate operations, and that amy or may not lead to dynamic allocations
 * depending on the participating types.
 * @section supported_data_types Supported data types
 * An expression may be built from any data types as long as the corresponding
 * operation's result type is @em Constructible from the corresponding
 * arguments. If you need to print an expression, ensure its participant types
 * implement streaming operator.
 * @section supported_operations Supported operations
 * Currently compound expressions support only operations with a single template
 * parameter. The arity of an operation may be arbitrary, but the parameters
 * must have the same type, which is then substituted as a template parameter
 * in the operation's instantiation. Out of the box the library supports
 * infix form for all the arithmetic operators (via @em std::plus,
 * @em std::less, etc). If you need to implement a new operation, follow these
 * steps:
 * - create a new class template
 * - provide at least one specialization for @em void template arguments with
 * parameter and return type deduced; in case of a single template parameter it
 * might look like this:
 * @code
 *  template <typename T>
 *   constexpr auto operator()(T &&s1, T &&s2) const -> std::decay_t<T> {
 *       ...
 *   }
 * @endcode
 * - if your code intends to print the expressions as strings, also implement
 * printing as described @ref printing_subsection_anchor "above".
 *
 * Below is an example of a class that implements logical xor:
 * @snippet example/custom_operator.cc full
 *
 * The following arithmetic operations are supported by the library:
 * @code
 * X + Y
 * X - Y
 * X * Y
 * X / Y
 * X && Y
 * X || Y
 * X == Y
 * X != Y
 * !X
 * -X
 * @endcode
 *
 * For the binary operations, combinations ('expression', 'expression'),
 * ('expression', 'value'), and ('value', 'expression') of @em X and @em Y are
 * implemented. For unary ones there's only variant, because the corresponding
 * operation is already defined for the value's type (or the code is incorrect).
 * In template parameters, @em E, @em E1, and @em E2 correspond to expression
 * types; @em T corresponds to the C++ value types.
 * @section glossary_section Glossary
 * - @em Expression - syntactic construction in a traditional sense of
 * programming languages.
 * - @em Operation - algebraic operation, defined on a set of objects of certain
 * type. Within the context of this library, a C++ function that implements
 * an algebraic operation is also called @em operation.

 * @see ctaeb::Expression
 * @see ctaeb::Variable
 * @see ctaeb::Compound
 */
#include "expression.h"
#include "operations.h"
#include "print.h"

#endif //CTAEB_CTAEB_H
