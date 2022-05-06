//
// Copyright (c) Joshua Higginbotham, 2022
//

#include <test/test.hpp>
#include <telescope.h>
#include <telescope.cpp> // ew but necessary

int main()
{
    Test::initialize();

    Test::testset("TS_VkCmdDrawRect", [](){

    });

    Test::testset("TS_VkCmdDrawSprite", [](){

    });

    Test::testset("TS_VkCmdClearColorImage", [](){

    });

    Test::testset("TS_Vk draw pass", [](){
        // test TS_VkBeginDrawPass
        // test TS_VkEndDrawPass
    });

    return Test::conclude();
}

