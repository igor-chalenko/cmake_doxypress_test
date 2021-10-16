// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief A simple usage example
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

struct UDT {
    int member1;
    int member2;

    explicit UDT(int n) : member1(n), member2(0) {
    }

    UDT(int n1, int n2) : member1(n1), member2(n2) {
    }
};

int main() {
    ctaeb::Variable<1> a;
    ctaeb::Variable<2> b;

    auto p = a + 1;
    auto q = a + b;
//! [error]
    // p(UDT(1, 2)); // can't sum `UDT` and `int`
    // q(UDT(1, 2)); // not enough arguments for the evaluation
//! [error]

    return 0;
}
//! [full]
