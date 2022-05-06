//
// Copyright (c) Joshua Higginbotham, 2022
//

//
// this file is intended to be a showcase of how to use
// Telescopes test environment. It is not testing any
// actual functionality. It is intended for **developers** of Telescope
//
// To add a test to the CMake test environment, in Telescope/CMakeLists.txt
// add the following line:
//
// ```
// if (BUILD_TESTING)
//      (...)
//
//      declare_test(test_name) # newly added line
// endif()
// ```
//
// Where `test_name` is the name of the .cpp. For example,
// if the file inside Telescope/test is called
// `test_initialization.cpp`, then the following line should
// be added to the CMakeLists.txt:
//
// declare_test(test_initialization)
//
// All tests need to be located in Telescope/test.
//
// Note that the Test:: module aims to follow Julias `@test` and
// `@testset` syntax closely.
//

#include <test/test.hpp>
#include <telescope.h>

// each individual test should be its own main, this is required by CMakes CTest
int main()
{
    // initialize test environment
    Test::initialize();

    // actual test behavior, multiple testsets are allowed
    Test::testset("template testset", [](){

        // type 1: assertion basic boolean
        Test::test(1 + 1 == 2);

        // type 2: assert that function throws
        Test::test_throws([](){
            throw std::invalid_argument("always throws");
        });

        // nested sets are allowed
        Test::testset("nested set", [](){
            Test::test(true);
        });

        // you can name individual assertions for better debug output
        bool trigger_fail = true;
        Test::test(not trigger_fail, "always failed assertion");
    });

    // always return main like this
    return Test::conclude();
}
