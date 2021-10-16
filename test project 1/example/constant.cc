// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates usage of the class `Constant`
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

using namespace ctaeb;

int main() {
    Variable<1> a;

    Compound<std::plus, Variable<1>&, Constant<int>> incrementer = a + 1;
    std::cout << incrementer.operator()(3) << std::endl;

    return 0;
}
//! [full]
