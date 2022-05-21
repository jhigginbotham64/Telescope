// 
// Copyright 2022 Clemens Cords
// Created on 21.05.22 by clem (mail@clemens-cords.com)
//

#include <test/test.hpp>
#include <include/time.hpp>
#include <cmath>

int main()
{
    Test::initialize();

    auto test_equals = [](auto x, auto value) -> void
    {
        double eps = 1e-6;
        Test::test(abs(x - value) < eps);
    };

    auto duration = ts::seconds(0);

    test_equals(duration.as_seconds(), 0);
    test_equals(duration.as_milliseconds(), 0);
    test_equals(duration.as_microseconds(), 0);
    test_equals(duration.as_nanoseconds(), 0);

    duration = ts::seconds(M_PI);

    test_equals(duration.as_seconds(), M_PI);
    test_equals(duration.as_milliseconds(), M_PI / 1e+3);
    test_equals(duration.as_microseconds(), M_PI / 1e+6);
    test_equals(duration.as_nanoseconds(), M_PI / 1e+9);

    for (double factor : {1.0, 1e+5, 1e+6})
    {
        auto value = M_PI * factor;
        test_equals(ts::seconds(value).as_seconds(), value);
        test_equals(ts::milliseconds(value).as_milliseconds(), value);
        test_equals(ts::microseconds(value).as_microseconds(), value);
        test_equals(ts::nanoseconds(value).as_nanoseconds(), value);
    }

    return Test::conclude();
}
