// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates expression printing
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

using namespace ctaeb;

int main() {
    Variable<1> a("a");
    Variable<2> b("b");

    auto sum = a + b;

    std::cout << sum << std::endl;

    return 0;
}
//! [full]
