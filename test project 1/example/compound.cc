// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates usage of the class `Compound`
 */

//! [full]
#include <iostream>
#include <ctaeb/ctaeb.h>

using namespace ctaeb;

template <typename T = void>
struct identity {
    template <typename T1>
    constexpr auto operator()(T1 && x) const {
        return std::forward<T1>(x);
    }
};
int main() {
    ctaeb::Variable<1> _1;
    ctaeb::Variable<2> _2;

    auto commutativity_check = (_1 + _2) == (_2 + _1);

    // prints:
    // 1
    std::cout << commutativity_check(1, 2) << std::endl;

    auto lambda = [](int x) {
        return x + 42;
    };

    auto x = Compound<identity, decltype(lambda)>(lambda);

    std::cout << x(3) << std::endl;

    return 0;
}
//! [full]
