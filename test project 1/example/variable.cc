// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates the usage of the class `Variable`
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

using namespace ctaeb;

int main() {
    Variable<1> _1;
    Variable<2> _2;

    // equality comparison for variables evaluates to bool only after
    // variable substitution:
    Compound<std::equal_to, Variable<1>&, Variable<2>&> equality = _1 == _2;
    // output:
    // 1
    std::cout << equality.operator()(3, 3) << std::endl;

    return 0;
}
//! [full]
