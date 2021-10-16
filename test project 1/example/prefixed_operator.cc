// Copyright (c) 2018 Igor Chalenko
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

/**
 * @file
 * @brief Demonstrates how to implement a custom operation
 */
//![full]
#include <iostream>
#include <functional>
#include <list>

#include <ctaeb/ctaeb.h>

using namespace ctaeb;

template <typename U = void>
struct CustomOperator {
    static constexpr bool prefixed = true;

    template <typename T>
    constexpr auto operator()(bool condition, T &&s1, T &&s2) const
    -> decltype(std::forward<T>(s1)) {
        return condition ? std::forward<T>(s1) : std::forward<T>(s2);
    }
};

namespace ctaeb { namespace print {
template<>
std::string to_string<::CustomOperator>() {
    return "?:";
}
} }

int main() {
    Variable<1> _1("x");
    Variable<2> _2("y");

    auto condition = _1 < _2;
    auto c9 = Compound<CustomOperator,
                       decltype(condition),
                       decltype(_1),
                       decltype(_2)>(condition, _1, _2);

    std::cout << c9 << std::endl;
}
//![full]