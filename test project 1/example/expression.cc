// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Example demonstrates progressive composition of expressions.
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

using namespace ctaeb;

struct A {

};

A operator +(const A &, const A&) {
    return {};
}

int main() {
    Variable<1> _1;
    Variable<2> _2;

//! [first]
    Compound<std::plus, Variable<1>&, Variable<2>&> sum = _1 + _2;
    typedef decltype(sum) first_compound_t;
//! [first]

//! [second]
    Compound<std::minus, Variable<1>&, Variable<2>&> difference = _1 - _2;
    typedef decltype(difference) second_compound_t;
//! [second]

//! [third]
    Compound<std::plus, first_compound_t &, second_compound_t &> sum2 = sum + difference;
//! [third]

//! [evaluation]
    std::cout << sum2(10, -5) << std::endl;
//! [evaluation]

    auto x = _1 + 1;
    auto y = 1 + _1;

    std::cout << x(3) << std::endl;
    std::cout << y(4) << std::endl;

    return 0;
}
//! [full]
