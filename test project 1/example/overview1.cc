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

using namespace std::string_literals;

struct vector3 {
    int x, y, z;

    vector3(int val1, int val2, int val3) : x(val1), y(val2), z(val3) {}

};

std::ostream &operator << (std::ostream &os, vector3 const &v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

vector3 operator + (vector3 const & v1, vector3 const & v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

int main() {
//! [variables]
    ctaeb::Variable<1> x("x");
    ctaeb::Variable<2> y("y");
//! [variables]

//! [compound]
    auto sum = x + y;
//! [compound]

//! [evaluation]
    std::cout << sum(3, 4) << std::endl;
    std::cout << sum("concat"s, "enation"s) << std::endl;
    std::cout << sum(vector3(1, 2, 3), vector3(4, 5, 6)) << std::endl;
//! [evaluation]

    return 0;
}
//! [full]
