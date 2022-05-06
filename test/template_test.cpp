//
// Copyright (c) Joshua Higginbotham, 2022
//

//
// this file is intended to be a showcase of how to use
// Telescopes test environment. It is not testing any
// actual functionality.
//

#include <test/test.hpp>

int main()
{
    // initialize test environment
    Test::initialize();

    // actual test behavior, can have more than one testset
    Test::testset("template_test_set", [](){

        // type 1: assertion basic boolean
        Test::test(1 + 1 == 2);

        // type 2: assert that function throws
        Test::test_throws([](){
            throw std::invalid_argument("always throws");
        });

        // you can name individual assertions for better debug output
        bool trigger_fail = false;
        Test::test(not trigger_fail, "always failed assertion");
    });

    // always return main like this
    return Test::conclude();
}
