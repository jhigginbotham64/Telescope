//
// Copyright (c) Joshua Higginbotham, 2022
//

#include <test/test.hpp>
#include <telescope.hpp>

int main()
{
    Test::initialize();

    Test::testset("TS_BtInit", [](){
    });

    Test::testset("TS_BtAddRigidBox", [](){
    });

    Test::testset("TS_BtAddStaticBox", [](){
    });

    Test::testset("TS_BtAddTriggerBox", [](){
    });

    Test::testset("TS_BtRemovePhysicsObject", [](){
    });

    Test::testset("TS_BtSetLinearVelocity", [](){
    });

    Test::testset("TS_BtGetLinearVelocity", [](){
    });

    Test::testset("TS_BtStepSimulation", [](){
    });

    Test::testset("TS_BtGetNextCollision", [](){
    });

    Test::testset("TS_BtGetPosition", [](){
    });

    Test::testset("TS_BtSetGravity", [](){
    });

    Test::testset("TS_BtSetCollisionMargin", [](){
    });

    Test::testset("TS_BtQuit", [](){
    });

    return Test::conclude();
}

