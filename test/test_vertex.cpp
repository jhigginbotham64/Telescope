//
// Copyright (c) Joshua Higginbotham, 2022
//

#include <test/test.hpp>
#include <telescope.h>
#include <telescope.cpp> // ew but necessary

int main()
{
    Test::initialize();

    Test::testset("TS_Vertex: ctor", [](){
    });

    Test::testset("TS_Vertex: getBindingDescription", [](){
    });

    Test::testset("TS_Vertex: getAttributeDescriptions", [](){
    });

    Test::testset("TS_Vertex: getTransform", [](){
    });

    return Test::conclude();
}

