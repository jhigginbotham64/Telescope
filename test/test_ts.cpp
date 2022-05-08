//
// Copyright (c) Joshua Higginbotham, 2022
//

#include <test/test.hpp>
#include <src/src.cpp>

int main()
{
    Test::initialize();

    Test::testset("TS_Init", [](){
    });

    Test::testset("TS_PlaySound", [](){
    });

    Test::testset("TS_PhysicsObject: ctor & dtor", [](){
    });

    Test::testset("TS_Quit", [](){
    });

    return Test::conclude();
}

