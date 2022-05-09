//
// Copyright (c) Joshua Higginbotham, 2022
//

#include <test/test.hpp>
#include <telescope.hpp>

int main()
{
    Test::initialize();

    Test::testset("CLAMP", [](){
    });

    Test::testset("TS_NDCX", [](){
    });

    Test::testset("TS_NDCY", [](){
    });

    Test::testset("TS_NDCRect", [](){
    });

    Test::testset("TS_NTCU", [](){
    });

    Test::testset("TS_NTCV", [](){
    });

    Test::testset("TS_NTCRect", [](){
    });

    return Test::conclude();
}

