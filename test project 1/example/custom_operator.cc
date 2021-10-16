// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates how to add a custom operation
 */
//![full]
#include <iostream>
#include <functional>
#include <list>

#include <ctaeb/ctaeb.h>

using namespace ctaeb;

template <typename U = void>
struct XOR {
    template <typename T>
    constexpr auto operator()(T &&s1, T &&s2) const -> std::decay_t<T> {
        return std::forward<T>(s1) ^ std::forward<T>(s2);
    }

    //constexpr static bool prefixed = true;
};

namespace ctaeb { namespace print {
template<>
std::string to_string<XOR>() {
    return "^";
}
} }

template<typename E1, typename E2, typename = Expressions<E1, E2>>
auto operator ^(E1 &&x, E2 &&y) -> Compound<XOR, E1, E2> {
    return Compound<XOR, E1, E2>(x, y);
}

int main() {
    Variable<1> _1("x");
    Variable<2> _2("y");

    auto x = _1 ^ _2;
    std::cout << x << std::endl;
    std::cout << x(1, 3) << std::endl;

    return 0;
}
//![full]